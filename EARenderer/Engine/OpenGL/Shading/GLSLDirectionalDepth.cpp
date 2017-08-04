//
//  GLDepthFillerProgram.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 30.04.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "GLSLDirectionalDepth.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace EARenderer {
    
#pragma mark - Lifecycle
    
    GLSLDirectionalDepth::GLSLDirectionalDepth()
    :
    GLProgram("DirectionalDepth.vert", "DirectionalDepth.frag", "")
    { }
    
#pragma mark - Setters
    
    void GLSLDirectionalDepth::setViewProjectionMatrix(const glm::mat4& matrix) {
        glUniformMatrix4fv(uniformLocation("uViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    void GLSLDirectionalDepth::setModelMatrix(const glm::mat4& matrix) {
        glUniformMatrix4fv(uniformLocation("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
}
