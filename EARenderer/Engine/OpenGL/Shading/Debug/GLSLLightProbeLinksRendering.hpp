//
//  GLSLLightProbeLinksRendering.hpp
//  EARenderer
//
//  Created by Pavel Muratov on 4/3/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#ifndef GLSLLightProbeLinksRendering_hpp
#define GLSLLightProbeLinksRendering_hpp

#include "GLProgram.hpp"
#include "GLHDRTexture3D.hpp"
#include "Camera.hpp"

#include <glm/vec3.hpp>

namespace EARenderer {

    class GLSLLightProbeLinksRendering: public GLProgram {
    public:
        GLSLLightProbeLinksRendering();

        void setCamera(const Camera& camera);
        void setWorldBoundingBox(const AxisAlignedBox3D& box);
        void setProbesPerGridDimensionCount(size_t count);
        void setProjectionClusterIndices(const GLUIntegerBufferTexture<uint32_t>& indices);
        void setProbeProjectionsMetadata(const GLUIntegerBufferTexture<uint32_t>& metadata);
        void setSurfelClusterCenters(const GLFloat3BufferTexture<glm::vec3>& centers);
    };

}


#endif /* GLSLLightProbeLinksRendering_hpp */
