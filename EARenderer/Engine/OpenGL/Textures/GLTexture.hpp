//
//  GLTexture.hpp
//  EARenderer
//
//  Created by Pavlo Muratov on 06.08.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#ifndef GLTexture_hpp
#define GLTexture_hpp

#include "GLNamedObject.hpp"
#include "GLBindable.hpp"
#include "Size2D.hpp"
#include "Color.hpp"

namespace EARenderer {
    
    class GLTexture: public GLNamedObject, public GLBindable {
    private:
        GLenum mBindingPoint;
        
    protected:
        Size2D mSize;
        uint16_t mMipMapsCount = 0;
    
        GLTexture(GLenum bindingPoint, GLint minFilter, GLint magFilter, GLint wrapModeS, GLint wrapModeT);
        GLTexture(const Size2D& size, GLenum bindingPoint, GLint minFilter, GLint magFilter, GLint wrapModeS, GLint wrapModeT);
        
    public:
        GLTexture(const GLTexture& that) = default;
        GLTexture& operator=(const GLTexture& rhs) = default;
        GLTexture(GLTexture&& that) = default;
        GLTexture& operator=(GLTexture&& rhs) = default;
        ~GLTexture() override = 0;
        
        const Size2D& size() const;
        uint16_t mipMapsCount() const;
        
        void bind() const override;
        void generateMipmaps();
    };
    
}

#endif /* GLTexture_hpp */
