//
//  GLSLScreenSpaceReflections.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 15.07.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "GLSLScreenSpaceReflections.hpp"

namespace EARenderer {

#pragma mark - Lifecycle

    GLSLScreenSpaceReflections::GLSLScreenSpaceReflections()
    :
    GLProgram("FullScreenQuad.vert", "SSR.frag", "")
    { }

#pragma mark - Lifecycle

    void GLSLScreenSpaceReflections::setCamera(const Camera& camera) {
        glm::vec2 nearFar(camera.nearClipPlane(), camera.farClipPlane());
        glUniform2fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraNearFarPlanes")>).location(), 1, glm::value_ptr(nearFar));

        glUniform3fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraPosition")>).location(), 1, glm::value_ptr(camera.position()));
        glUniformMatrix4fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraViewMat")>).location(), 1, GL_FALSE,
                           glm::value_ptr(camera.viewMatrix()));
        glUniformMatrix4fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraProjectionMat")>).location(), 1, GL_FALSE,
                           glm::value_ptr(camera.projectionMatrix()));
        glUniformMatrix4fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraViewInverse")>).location(), 1, GL_FALSE,
                           glm::value_ptr(camera.inverseViewMatrix()));
        glUniformMatrix4fv(uniformByNameCRC32(uint32_constant<ctcrc32("uCameraProjectionInverse")>).location(), 1, GL_FALSE,
                           glm::value_ptr(camera.inverseProjectionMatrix()));
    }

    void GLSLScreenSpaceReflections::setGBuffer(const SceneGBuffer& GBuffer) {
        setUniformTexture(uint32_constant<ctcrc32("uGBufferAlbedoRoughnessMetalnessAONormal")>, GBuffer.albedoRoughnessMetalnessAONormal);
        setUniformTexture(uint32_constant<ctcrc32("uGBufferLinearDepth")>, GBuffer.linearDepth);
        setUniformTexture(uint32_constant<ctcrc32("uGBufferHyperbolicDepth")>, GBuffer.hyperbolicDepth);
    }

    void GLSLScreenSpaceReflections::setFrame(const GLFloatTexture2D& frame) {
        setUniformTexture(uint32_constant<ctcrc32("uFrame")>, frame);
    }

}

