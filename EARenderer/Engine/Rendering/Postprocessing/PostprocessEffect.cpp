//
//  PostprocessingEffect.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 29.06.2018.
//  Copyright © 2018 MPO. All rights reserved.
//

#include "PostprocessEffect.hpp"

namespace EARenderer {

#pragma mark - Lifecycle

    PostprocessEffect::PostprocessEffect(std::shared_ptr<PostprocessTexturePool> texturePool)
    :
    mTexturePool(texturePool)
    { }

}
