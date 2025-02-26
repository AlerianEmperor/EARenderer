//
//  GLSLProbeOcclusionRendering.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 20.05.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GLSLProbeOcclusionRendering.hpp"

namespace EARenderer {

#pragma mark - Lifecycle

    GLSLProbeOcclusionRendering::GLSLProbeOcclusionRendering()
            :
            GLProgram("ProbeOcclusionRendering.vert", "ProbeOcclusionRendering.frag", "ProbeOcclusionRendering.geom") {
    }

#pragma mark - Setters

    void GLSLProbeOcclusionRendering::setCamera(const Camera &camera) {
        glUniformMatrix4fv(uniformByNameCRC32(ctcrc32("uCameraSpaceMat")).location(), 1, GL_FALSE, glm::value_ptr(camera.viewProjectionMatrix()));
    }

//    void GLSLProbeOcclusionRendering::setDiffuseProbeOcclusionMapsAtlas(const GLHDRTexture2D& atlas) {
//        setBufferTexture(ctcrc32("uProbeOcclusionMapsAtlas"), atlas);
//    }

//    void GLSLProbeOcclusionRendering::setProbeOcclusionMapAtlasOffsets(const GLUInteger2BufferTexture<glm::uvec2> &offsets) {
//        setBufferTexture(ctcrc32("uProbeOcclusionMapAtlasOffsets"), offsets);
//    }

    void GLSLProbeOcclusionRendering::setProbeIndex(size_t index) {
        glUniform1i(uniformByNameCRC32(ctcrc32("uProbeIndex")).location(), (GLint) index);
    }

}


