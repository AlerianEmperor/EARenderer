//
//  SurfelsRenderer.hpp
//  EARenderer
//
//  Created by Pavlo Muratov on 24.12.2017.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef SurfelRenderer_hpp
#define SurfelRenderer_hpp

#include "Scene.hpp"
#include "GLSLSurfelRendering.hpp"
#include "SurfelData.hpp"
#include "DiffuseLightProbeData.hpp"
#include "GLTexture2D.hpp"

#include <vector>

namespace EARenderer {

    class SurfelRenderer {
    private:
        const Scene *mScene = nullptr;
        const SurfelData *mSurfelData;
        const DiffuseLightProbeData *mProbeData;
        const GLFloatTexture2D<GLTexture::Float::R16F> *mSurfelLuminances;

        std::vector<GLVertexArray<Surfel>> mSurfelClusterVAOs;
        std::vector<Color> mSurfelClusterColors;
        GLSLSurfelRendering mSurfelRenderingShader;

    public:
        enum class Mode {
            Default, Clusters
        };

        SurfelRenderer(
                const Scene *scene,
                const SurfelData *surfelData,
                const DiffuseLightProbeData *probeData,
                const GLFloatTexture2D<GLTexture::Float::R16F> *surfelLuminances
        );

        void render(Mode renderingMode, float surfelRadius, size_t probeIndex = -1);
    };

}

#endif /* SurfelRenderer_hpp */
