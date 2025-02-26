//
//  TexturePool.hpp
//  EARenderer
//
//  Created by Pavel Muratov on 7/3/18.
//  Copyright © 2018 MPO. All rights reserved.
//

#ifndef TexturePool_hpp
#define TexturePool_hpp

#include "GLTexture2D.hpp"
#include "GLFramebuffer.hpp"
#include "GLDepthRenderbuffer.hpp"

#include <unordered_set>
#include <memory>

namespace EARenderer {

    class PostprocessTexturePool {
    public:
        using PostprocessTexture = GLFloatTexture2D<GLTexture::Float::RGBA16F>;

    private:
        using TexturePointerSet = std::unordered_set<std::shared_ptr<PostprocessTexture>>;

        TexturePointerSet mFreeTextures;
        TexturePointerSet mClaimedTextures;

        Size2D mTextureResolution;

    public:
        PostprocessTexturePool(const Size2D& resolution);

        std::shared_ptr<PostprocessTexture> claim();
        void putBack(std::shared_ptr<PostprocessTexture> texture);
    };

}

#endif /* TexturePool_hpp */
