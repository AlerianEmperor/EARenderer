//
//  GLSkyboxProgram.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 22.04.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "GLSLSkybox.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace EARenderer {

#pragma mark - Lifecycle

    GLSLSkybox::GLSLSkybox() : GLProgram("Skybox.vert", "Skybox.frag", "") {}

#pragma mark - Setters

    void GLSLSkybox::setViewMatrix(const glm::mat4 &matrix) {
        glUniformMatrix4fv(uniformByNameCRC32(ctcrc32("uProjectionMatrix")).location(), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void GLSLSkybox::setProjectionMatrix(const glm::mat4 &matrix) {
        glUniformMatrix4fv(uniformByNameCRC32(ctcrc32("uViewMatrix")).location(), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void GLSLSkybox::setExposure(float exposure) {
        glUniform1f(uniformByNameCRC32(ctcrc32("uExposure")).location(), exposure);
    }

    void GLSLSkybox::setEquirectangularMap(const GLFloatTexture2D<GLTexture::Float::RGB16F> &equireqMap) {
        setUniformTexture(ctcrc32("uEquirectangularMap"), equireqMap);
        glUniform1i(uniformByNameCRC32(ctcrc32("uIsCube")).location(), GL_FALSE);
    }

}
