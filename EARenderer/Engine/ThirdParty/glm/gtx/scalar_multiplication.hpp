/// @ref gtx
/// @file glm/gtx/scalar_multiplication.hpp
/// @author Joshua Moerman
///
/// @brief Enables scalar multiplication for all types
///
/// Since GLSL is very strict about types, the following (often used) combinations do not work:
///    double * vec4
///    int * vec4
///    vec4 / int
/// So we'll fix that! Of course "float * vec4" should remain the same (hence the enable_if magic)

#pragma once

#include "../detail/setup.hpp"

#if !GLM_HAS_TEMPLATE_ALIASES && !(GLM_COMPILER & GLM_COMPILER_GCC)
#	error "GLM_GTX_scalar_multiplication requires C++11 support or alias templates and if not support for GCC"
#endif

#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../mat2x2.hpp"
#include <type_traits>

namespace glm {
    template<typename T, typename Vec>
    using return_type_scalar_multiplication = typename std::enable_if<
            !std::is_same<T, float>::value       // T may not be a float
                    && std::is_arithmetic<T>::value, Vec // But it may be an int or double (no vec3 or mat3, ...)
    >::type;

#define GLM_IMPLEMENT_SCAL_MULT(Vec) \
    template <typename T> \
    return_type_scalar_multiplication<T, Vec> \
    operator*(T const & s, Vec rh){ \
        return rh *= static_cast<float>(s); \
    } \
     \
    template <typename T> \
    return_type_scalar_multiplication<T, Vec> \
    operator*(Vec lh, T const & s){ \
        return lh *= static_cast<float>(s); \
    } \
     \
    template <typename T> \
    return_type_scalar_multiplication<T, Vec> \
    operator/(Vec lh, T const & s){ \
        return lh *= 1.0f / s; \
    }

    GLM_IMPLEMENT_SCAL_MULT(vec2)

    GLM_IMPLEMENT_SCAL_MULT(vec3)

    GLM_IMPLEMENT_SCAL_MULT(vec4)

    GLM_IMPLEMENT_SCAL_MULT(mat2)

    GLM_IMPLEMENT_SCAL_MULT(mat2x3)

    GLM_IMPLEMENT_SCAL_MULT(mat2x4)

    GLM_IMPLEMENT_SCAL_MULT(mat3x2)

    GLM_IMPLEMENT_SCAL_MULT(mat3)

    GLM_IMPLEMENT_SCAL_MULT(mat3x4)

    GLM_IMPLEMENT_SCAL_MULT(mat4x2)

    GLM_IMPLEMENT_SCAL_MULT(mat4x3)

    GLM_IMPLEMENT_SCAL_MULT(mat4)

#undef GLM_IMPLEMENT_SCAL_MULT
} // namespace glm
