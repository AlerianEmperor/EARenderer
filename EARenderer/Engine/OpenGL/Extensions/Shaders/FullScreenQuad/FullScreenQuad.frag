#version 400 core

#include "ColorSpace.glsl"
#include "Constants.glsl"

uniform sampler2D uTexture;
uniform sampler2DArray uTextureArray;
uniform sampler3D uTexture3D;
uniform int uIndex;
uniform float uDepth;
uniform bool uShouldSampleArray;
uniform bool uShouldSample3DTexture;
uniform bool uShouldApplyToneMapping;

in vec2 vTexCoords;

out vec4 oFragColor;

vec3 ReinhardToneMap(vec3 color) {
    return color / (color + vec3(1.0));
}

void main()
{
    if (uShouldSampleArray) {
        vec3 coords = vec3(vTexCoords, uIndex);
        oFragColor = vec4(texture(uTextureArray, coords).rgb, 1.0);
    } else if (uShouldSample3DTexture) {
        oFragColor = vec4(texture(uTexture3D, vec3(vTexCoords, uDepth)).rgb, 1.0);
    } else {
        vec3 color = textureLod(uTexture, vTexCoords, 0).rgb;
        oFragColor = vec4(color, 1.0);
    }

    if (uShouldApplyToneMapping) {
        oFragColor = vec4(ReinhardToneMap(oFragColor.rgb), oFragColor.a);
    }
    
    oFragColor = vec4(SRGB_From_RGB(oFragColor.rgb), 1.0);
}
