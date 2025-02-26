//
//  IndirectLightAccumulator.cpp
//  EARenderer
//
//  Created by Pavel Muratov on 9/5/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "IndirectLightAccumulator.hpp"
#include "Drawable.hpp"

namespace EARenderer {

#pragma mark - Lifecycle

    IndirectLightAccumulator::IndirectLightAccumulator(
            const Scene *scene,
            const GPUResourceController *gpuResourceController,
            const SceneGBuffer *gBuffer,
            const SurfelData *surfelData,
            const DiffuseLightProbeData *probeData,
            const ShadowMapper *shadowMapper)
            :
            mScene(scene),
            mGPUResourceController(gpuResourceController),
            mGBuffer(gBuffer),
            mSurfelData(surfelData),
            mProbeData(probeData),
            mShadowMapper(shadowMapper),
            mFramebuffer(framebufferResolution()),
            mGridProbeSHMaps(gridProbeSHMaps()),
            mSurfelsLuminanceMap(surfelData->surfelsGBuffer()->size(), nullptr, Sampling::Filter::None),
            mSurfelClustersLuminanceMap(surfelData->surfelClustersGBuffer()->size(), nullptr, Sampling::Filter::None) {
    }

    Size2D IndirectLightAccumulator::framebufferResolution() {
        Size2D probeGridResolution(mProbeData->gridResolution().x, mProbeData->gridResolution().y);
        Size2D surfelLuminanceMapResolution(mSurfelData->surfelsGBuffer()->size());
        Size2D clusterLuminanceMapResolution(mSurfelData->surfelClustersGBuffer()->size());
        return probeGridResolution.makeUnion(surfelLuminanceMapResolution).makeUnion(clusterLuminanceMapResolution);
    }

    std::array<GLLDRTexture3D, 4> IndirectLightAccumulator::gridProbeSHMaps() {
        auto resolution = mProbeData->gridResolution();
        return std::array<GLLDRTexture3D, 4>{
                GLLDRTexture3D(Size2D(resolution.x, resolution.y), resolution.z),
                GLLDRTexture3D(Size2D(resolution.x, resolution.y), resolution.z),
                GLLDRTexture3D(Size2D(resolution.x, resolution.y), resolution.z),
                GLLDRTexture3D(Size2D(resolution.x, resolution.y), resolution.z)
        };
    }

#pragma mark - Getters / Setters

    void IndirectLightAccumulator::setRenderingSettings(const RenderingSettings &settings) {
        mSettings = settings;
    }

    const std::array<GLLDRTexture3D, 4> &IndirectLightAccumulator::gridProbesSphericalHarmonics() const {
        return mGridProbeSHMaps;
    }

    const GLFloatTexture2D<GLTexture::Float::R16F> &IndirectLightAccumulator::surfelsLuminanceMap() const {
        return mSurfelsLuminanceMap;
    }

    const GLFloatTexture2D<GLTexture::Float::R16F> &IndirectLightAccumulator::surfelClustersLuminanceMap() const {
        return mSurfelClustersLuminanceMap;
    }

#pragma mark - Private Helpers

    void IndirectLightAccumulator::relightSurfels() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        const DirectionalLight &directionalLight = mScene->sun();

        mFramebuffer.redirectRenderingToTextures(GLViewport(mSurfelsLuminanceMap.size()),
                GLFramebuffer::UnderlyingBuffer::Color | GLFramebuffer::UnderlyingBuffer::Depth,
                &mSurfelsLuminanceMap);

        mSurfelLightingShader.bind();
        mSurfelLightingShader.setSettings(mSettings);
        mSurfelLightingShader.ensureSamplerValidity([&]() {
            mSurfelLightingShader.setDirectionalShadowMapArray(mShadowMapper->directionalShadowMapArray());
            mSurfelLightingShader.setSurfelsGBuffer(*mSurfelData->surfelsGBuffer());
            mSurfelLightingShader.setGridProbesSHTextures(mGridProbeSHMaps);
            mSurfelLightingShader.setProbePositions(*mProbeData->probePositionsBufferTexture());
        });

        mSurfelLightingShader.setLightType(LightType::Directional);

        mSurfelLightingShader.setLight(directionalLight);
        mSurfelLightingShader.setShadowCascades(mShadowMapper->cascades());
        mSurfelLightingShader.setWorldBoundingBox(mScene->lightBakingVolume());

        Drawable::TriangleStripQuad::Draw();

        mSurfelLightingShader.setLightType(LightType::Point);

        for (ID lightID : mScene->pointLights()) {
            const PointLight &light = mScene->pointLights()[lightID];

            mSurfelLightingShader.setUniformBuffer(
                    ctcrc32("PointLightUBO"),
                    *mGPUResourceController->uniformBuffer(),
                    mGPUResourceController->pointLightUBODataLocation(lightID)
            );
            mSurfelLightingShader.ensureSamplerValidity([&]() {
                mSurfelLightingShader.setOmnidirectionalShadowMap(mShadowMapper->shadowMapForPointLight(lightID));
            });

            Drawable::TriangleStripQuad::Draw();
        }

        glDisable(GL_BLEND);
    }

    void IndirectLightAccumulator::averageSurfelClusterLuminances() {
        mSurfelClusterAveragingShader.bind();
        mFramebuffer.redirectRenderingToTextures(GLViewport(mSurfelClustersLuminanceMap.size()),
                GLFramebuffer::UnderlyingBuffer::Color | GLFramebuffer::UnderlyingBuffer::Depth,
                &mSurfelClustersLuminanceMap);

        mSurfelClusterAveragingShader.ensureSamplerValidity([&]() {
            mSurfelClusterAveragingShader.setSurfelClustersGBuffer(*mSurfelData->surfelClustersGBuffer());
            mSurfelClusterAveragingShader.setSurfelsLuminaceMap(mSurfelsLuminanceMap);
        });

        Drawable::TriangleStripQuad::Draw();
    }

    void IndirectLightAccumulator::updateGridProbes() {
        float weight = 2.0 * M_PI;
        Color color = mScene->skybox()->ambientColor().convertedTo(EARenderer::Color::Space::YCoCg);

        SphericalHarmonics skySH;
        skySH.contribute(glm::vec3(1.0, 0.0, 0.0), color, weight);
        skySH.contribute(glm::vec3(-1.0, 0.0, 0.0), color, weight);
        skySH.convolve();

        GLViewport viewport(Size2D(mProbeData->gridResolution().x, mProbeData->gridResolution().y));
        mFramebuffer.redirectRenderingToTextures(viewport,
                GLFramebuffer::UnderlyingBuffer::Color | GLFramebuffer::UnderlyingBuffer::Depth,
                &(mGridProbeSHMaps)[0], &(mGridProbeSHMaps)[1], &(mGridProbeSHMaps)[2], &(mGridProbeSHMaps)[3]);

        mGridProbesUpdateShader.bind();
        mGridProbesUpdateShader.ensureSamplerValidity([&] {
            mGridProbesUpdateShader.setProbeProjectionsMetadata(*mProbeData->probeClusterProjectionsMetadataBufferTexture());
            mGridProbesUpdateShader.setProjectionClusterIndices(*mProbeData->projectionClusterIndicesBufferTexture());
            mGridProbesUpdateShader.setProjectionClusterSphericalHarmonics(*mProbeData->projectionClusterSHsBufferTexture());
            mGridProbesUpdateShader.setSurfelClustersLuminaceMap(mSurfelClustersLuminanceMap);
            mGridProbesUpdateShader.setSkySphericalHarmonics(*mProbeData->skySHsBufferTexture());
            mGridProbesUpdateShader.setProbesGridResolution(mProbeData->gridResolution());
            mGridProbesUpdateShader.setSkyColorSphericalHarmonics(skySH);
        });

        Drawable::TriangleStripQuad::Draw(mProbeData->gridResolution().z);
    }

#pragma mark - Public Interface

    void IndirectLightAccumulator::updateProbes() {
        relightSurfels();
        averageSurfelClusterLuminances();
        updateGridProbes();
    }

    void IndirectLightAccumulator::render() {
        mLightEvaluationShader.bind();
        mLightEvaluationShader.setCamera(*(mScene->camera()));
        mLightEvaluationShader.setWorldBoundingBox(mScene->lightBakingVolume());
        mLightEvaluationShader.setSettings(mSettings);
        mLightEvaluationShader.ensureSamplerValidity([&]() {
            mLightEvaluationShader.setGBuffer(*mGBuffer);
            mLightEvaluationShader.setProbePositions(*mProbeData->probePositionsBufferTexture());
            mLightEvaluationShader.setGridProbesSHTextures(mGridProbeSHMaps);
        });

        Drawable::TriangleStripQuad::Draw();
    }

}
