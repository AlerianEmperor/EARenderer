//
//  SurfelsRenderer.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 24.12.2017.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "SurfelRenderer.hpp"
#include "SurfelGenerator.hpp"
#include "WavefrontMeshLoader.hpp"
#include "FileManager.hpp"
#include "GLVertexAttribute.hpp"
#include "Drawable.hpp"

#include <glm/gtx/transform.hpp>

#include <random>

namespace EARenderer {

#pragma mark - Lifecycle

    SurfelRenderer::SurfelRenderer(
            const Scene *scene,
            const SurfelData *surfelData,
            const DiffuseLightProbeData *probeData,
            const GLFloatTexture2D<GLTexture::Float::R16F> *surfelLuminances)
            :
            mScene(scene),
            mSurfelData(surfelData),
            mProbeData(probeData),
            mSurfelLuminances(surfelLuminances) {

        std::random_device device;
        std::mt19937 engine(device());
        std::uniform_real_distribution<float> distribution(0.3, 1.0);

        for (auto &cluster : surfelData->surfelClusters()) {
            std::array<GLVertexAttribute, 4> attributes{
                    GLVertexAttribute::UniqueAttribute(sizeof(glm::vec3), glm::vec3::length()),
                    GLVertexAttribute::UniqueAttribute(sizeof(glm::vec3), glm::vec3::length()),
                    GLVertexAttribute::UniqueAttribute(sizeof(Color), 4),
                    GLVertexAttribute::UniqueAttribute(sizeof(float), 1)
            };
            mSurfelClusterVAOs.emplace_back(surfelData->surfels().data() + cluster.surfelOffset, cluster.surfelCount, attributes.data(), attributes.size());
            mSurfelClusterColors.emplace_back(Color(distribution(engine), distribution(engine), distribution(engine)));
        }
    }

#pragma mark - Public interface

    void SurfelRenderer::render(Mode renderingMode, float surfelRadius, size_t probeIndex) {
        mSurfelRenderingShader.bind();

        switch (renderingMode) {
            case Mode::Default:
                mSurfelRenderingShader.setShouldUseExternalColor(false);
                break;
            case Mode::Clusters:
                mSurfelRenderingShader.setShouldUseExternalColor(true);
                break;
        }

        auto vp = mScene->camera()->viewProjectionMatrix();
        mSurfelRenderingShader.setViewProjectionMatrix(vp);
        mSurfelRenderingShader.setSurfelRadius(surfelRadius);
        mSurfelRenderingShader.ensureSamplerValidity([this]() {
            mSurfelRenderingShader.setSurfelLuminances(*mSurfelLuminances);
//            mSurfelRenderingShader.setSurfelsGBuffer(*mSurfelData->surfelsGBuffer());
        });

        if (probeIndex != -1) {
            const DiffuseLightProbe &probe = mProbeData->probes()[probeIndex];
            size_t projectionGroupCount = probe.surfelClusterProjectionGroupSize;
            size_t projectionGroupOffset = probe.surfelClusterProjectionGroupOffset;

            for (size_t i = projectionGroupOffset; i < projectionGroupOffset + projectionGroupCount; i++) {
                const SurfelClusterProjection &projection = mProbeData->surfelClusterProjections()[i];
                mSurfelClusterVAOs[projection.surfelClusterIndex].bind();
                mSurfelRenderingShader.setExternalColor(mSurfelClusterColors[projection.surfelClusterIndex]);
                const SurfelCluster &cluster = mSurfelData->surfelClusters()[projection.surfelClusterIndex];
                Drawable::Point::Draw(cluster.surfelCount);
            }
        } else {
            for (size_t i = 0; i < mSurfelClusterVAOs.size(); i++) {
                mSurfelClusterVAOs[i].bind();
                const SurfelCluster &cluster = mSurfelData->surfelClusters()[i];
                mSurfelRenderingShader.setSurfelGroupOffset(cluster.surfelOffset);
                mSurfelRenderingShader.setExternalColor(mSurfelClusterColors[i]);
                Drawable::Point::Draw(cluster.surfelCount);
            }
        }
    }

}
