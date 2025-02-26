//
//  GLSLGaussianBlur.cpp
//  EARenderer
//
//  Created by Pavel Muratov on 6/13/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GLSLGaussianBlur.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace EARenderer {

#pragma mark - Lifecycle

    GLSLGaussianBlur::GLSLGaussianBlur()
            :
            GLProgram("FullScreenQuad.vert", "GaussianBlur.frag", "") {
    }

#pragma mark - Lifecycle

    void GLSLGaussianBlur::setBlurDirection(BlurDirection direction) {
        glm::vec2 dir;
        switch (direction) {
            case BlurDirection::Horizontal:
                dir = glm::vec2(1.0, 0.0);
                break;
            case BlurDirection::Vertical:
                dir = glm::vec2(0.0, 1.0);
                break;
        }
        glUniform2fv(uniformByNameCRC32(ctcrc32("uBlurDirection")).location(), 1, glm::value_ptr(dir));
    }

    void GLSLGaussianBlur::setKernelWeights(const std::vector<float> &weights) {
        glUniform1fv(uniformByNameCRC32(ctcrc32("uKernelWeights[0]")).location(), (GLint) weights.size(), reinterpret_cast<const GLfloat *>(weights.data()));
        glUniform1i(uniformByNameCRC32(ctcrc32("uKernelSize")).location(), (GLint) weights.size());
    }

    void GLSLGaussianBlur::setTextureOffsets(const std::vector<float> &offsets) {
        glUniform1fv(uniformByNameCRC32(ctcrc32("uTextureOffsets[0]")).location(), (GLint) offsets.size(), reinterpret_cast<const GLfloat *>(offsets.data()));
        glUniform1i(uniformByNameCRC32(ctcrc32("uKernelSize")).location(), (GLint) offsets.size());
    }

    void GLSLGaussianBlur::setRenderTargetSize(const Size2D &RTSize) {
        glm::vec2 size(RTSize.width, RTSize.height);
        glUniform2fv(uniformByNameCRC32(ctcrc32("uRenderTargetSize")).location(), 1, glm::value_ptr(size));
    }

}
