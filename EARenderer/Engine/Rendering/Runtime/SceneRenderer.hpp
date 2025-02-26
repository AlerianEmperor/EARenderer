////
////  Renderer.hpp
////  EARenderer
////
////  Created by Pavlo Muratov on 29.03.17.
////  Copyright © 2017 MPO. All rights reserved.
////
//
//#ifndef Renderer_hpp
//#define Renderer_hpp
//
//#include <unordered_set>
//#include <array>
//#include <memory>
//
//#include <glm/vec2.hpp>
//
//#include "Scene.hpp"
//#include "GLFramebuffer.hpp"
//#include "DefaultRenderComponentsProviding.hpp"
//#include "FrustumCascades.hpp"
//#include "Ray3D.hpp"
//#include "SurfelData.hpp"
//#include "DiffuseLightProbeData.hpp"
//#include "RenderingSettings.hpp"
//#include "GaussianBlurEffect.hpp"
//#include "BloomEffect.hpp"
//#include "ToneMappingEffect.hpp"
//
//#include "GLSLDepthPrepass.hpp"
//#include "GLSLCookTorrance.hpp"
//#include "GLSLFullScreenQuad.hpp"
//#include "GLSLDirectionalDepth.hpp"
//#include "GLSLOmnidirectionalDepth.hpp"
//#include "GLSLSkybox.hpp"
//#include "GLSLGenericGeometry.hpp"
//#include "GLSLEquirectangularMapConversion.hpp"
//#include "GLSLSpecularRadianceConvolution.hpp"
//#include "GLSLBRDFIntegration.hpp"
//#include "GLSLSurfelLighting.hpp"
//#include "GLSLSurfelClusterAveraging.hpp"
//#include "GLSLGridLightProbesUpdate.hpp"
//#include "GLSLGridLightProbeRendering.hpp"
//#include "GLSLLightProbeLinksRendering.hpp"
//#include "GLSLProbeOcclusionRendering.hpp"
//#include "GLSLDirectionalExponentialShadowMap.hpp"
//
//#include "GLDepthTextureCubemap.hpp"
//#include "GLTexture2DArray.hpp"
//#include "GLDepthTexture2DArray.hpp"
//
//#include "GLHDRTextureCubemap.hpp"
//#include "GLTexture2DArray.hpp"
//#include "GLLDRTexture3D.hpp"
//#include "GLHDRTexture3D.hpp"
//#include "GLBufferTexture.hpp"
//#include "GLDepthRenderbuffer.hpp"
//
//namespace EARenderer {
//    
//    class SceneRenderer {
//    private:
//        uint8_t mNumberOfCascades = 1;
//        uint8_t mNumberOfIrradianceMips = 5;
//        glm::ivec3 mProbeGridResolution;
//        
//        const Scene *mScene = nullptr;
//
//        RenderingSettings mSettings;
//
//        std::shared_ptr<const SurfelData> mSurfelData;
//        std::shared_ptr<const DiffuseLightProbeData> mDiffuseProbeData;
//
//        DefaultRenderComponentsProviding *mDefaultRenderComponentsProvider = nullptr;
//        GLVertexArray<DiffuseLightProbe> mDiffuseProbesVAO;
//        FrustumCascades mShadowCascades;
//
//        GLSLDepthPrepass mDepthPrepassShader;
//        GLSLDirectionalExponentialShadowMap mDirectionalESMShader;
//        GLSLDirectionalDepth mDirectionalDepthShader;
//        GLSLOmnidirectionalDepth mOmnidirectionalDepthShader;
//        GLSLSkybox mSkyboxShader;
//        GLSLCookTorrance mLightEvaluationShader;
//        GLSLEquirectangularMapConversion mEqurectangularMapConversionShader;
//        GLSLSpecularRadianceConvolution mRadianceConvolutionShader;
//        GLSLBRDFIntegration mBRDFIntegrationShader;
//        GLSLSurfelLighting mSurfelLightingShader;
//        GLSLSurfelClusterAveraging mSurfelClusterAveragingShader;
//        GLSLGridLightProbesUpdate mGridProbesUpdateShader;
//
//        GLSLLightProbeLinksRendering mLightProbeLinksRenderingShader;
//        GLSLProbeOcclusionRendering mProbeOcclusionRenderingShader;
//        
//        GLHDRTextureCubemap mEnvironmentMapCube;
//        GLHDRTextureCubemap mDiffuseIrradianceMap;
//        GLHDRTextureCubemap mSpecularIrradianceMap;
//        GLHDRTexture2D mBRDFIntegrationMap;
//        GLFramebuffer mIBLFramebuffer;
//
//        GLHDRTexture2D mSurfelsLuminanceMap;
//        GLHDRTexture2D mSurfelClustersLuminanceMap;
//        GLFramebuffer mSurfelsLuminanceFramebuffer;
//        GLFramebuffer mSurfelClustersLuminanceFramebuffer;
//
//        std::array<GLLDRTexture3D, 4> mGridProbesSHMaps;
//        GLFramebuffer mGridProbesSHFramebuffer;
//
//        GLDepthRenderbuffer mDepthRenderbuffer;
//        GLHDRTexture2D mDirectionalExponentialShadowMap;
//        GLFramebuffer mDirectionalShadowFramebuffer;
//
//        GLSLGridLightProbeRendering mGridProbeRenderingShader;
//        GLSLFullScreenQuad mFSQuadShader;
//        GLSLGenericGeometry mGenericShader;
//
//        GLHDRTexture2D mOutputFrame;
//        GLHDRTexture2D mThresholdFilteredOutputFrame;
//        GLDepthRenderbuffer mOutputDepthRenderbuffer;
//        GLFramebuffer mOutputFramebuffer;
//
//        GaussianBlurEffect mShadowBlurEffect;
//        BloomEffect mBloomEffect;
//        ToneMappingEffect mToneMappingEffect;
//
//        void setupGLState();
//        void setupTextures();
//        void setupFramebuffers();
//
//        void bindDefaultFramebuffer();
//
//        void performDepthPrepass();
//        void renderExponentialShadowMapsForDirectionalLight();
//        void relightSurfels();
//        void averageSurfelClusterLuminances();
//        void updateGridProbes();
//
//        void convertEquirectangularMapToCubemap();
//        void buildDiffuseIrradianceMap();
//        void buildSpecularIrradianceMap();
//        void buildBRDFIntegrationMap();
//
//    public:
//        SceneRenderer(const Scene* scene, std::shared_ptr<const SurfelData> surfelData, std::shared_ptr<const DiffuseLightProbeData> diffuseProbeData);
//
//        const FrustumCascades& shadowCascades() const;
//
//        void setDefaultRenderComponentsProvider(DefaultRenderComponentsProviding *provider);
//        void setRenderingSettings(const RenderingSettings& settings);
//        bool raySelectsMesh(const Ray3D& ray, ID& meshID);
//
//        void prepareFrame();
//
//        void renderMeshes();
//        void renderSkybox();
//        void renderSurfelsGBuffer();
//        void renderSurfelLuminances();
//        void renderSurfelClusterLuminances();
//        void renderDiffuseGridProbes(float radius);
//        void renderLinksForDiffuseProbe(size_t probeIndex);
//        void renderProbeOcclusionMap(size_t probeIndex);
//    };
//    
//}
//
//#endif /* Renderer_hpp */
