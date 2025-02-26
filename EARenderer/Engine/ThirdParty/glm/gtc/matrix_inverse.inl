/// @ref gtc_matrix_inverse
/// @file glm/gtc/matrix_inverse.inl

namespace glm {
    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x3<T, P>
    affineInverse(tmat3x3<T, P>
    const & m) {
    tmat2x2 <T, P> const Inv(inverse(tmat2x2<T, P>(m)));

    return
    tmat3x3<T, P>(
            tvec3<T, P>(Inv[0], static_cast<T>(0)),
            tvec3<T, P>(Inv[1], static_cast<T>(0)),
            tvec3<T, P>(-Inv * tvec2<T, P>(m[2]), static_cast<T>(1))
    );
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4<T, P>
affineInverse(tmat4x4<T, P>
const & m)
{
tmat3x3 <T, P> const Inv(inverse(tmat3x3<T, P>(m)));

return
tmat4x4<T, P>(
        tvec4<T, P>(Inv[0], static_cast<T>(0)),
        tvec4<T, P>(Inv[1], static_cast<T>(0)),
        tvec4<T, P>(Inv[2], static_cast<T>(0)),
        tvec4<T, P>(-Inv * tvec3<T, P>(m[3]), static_cast<T>(1))
);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat2x2<T, P>
inverseTranspose(tmat2x2<T, P>
const & m)
{
T Determinant = m[0][0] * m[1][1] - m[1][0] * m[0][1];

tmat2x2<T, P> Inverse(
        +m[1][1] / Determinant,
        -m[0][1] / Determinant,
        -m[1][0] / Determinant,
        +m[0][0] / Determinant);

return
Inverse;
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x3<T, P>
inverseTranspose(tmat3x3<T, P>
const & m)
{
T Determinant =
        +m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
                - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
                + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

tmat3x3<T, P> Inverse(uninitialize);
Inverse[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
Inverse[0][1] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]);
Inverse[0][2] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
Inverse[1][0] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]);
Inverse[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]);
Inverse[1][2] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]);
Inverse[2][0] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
Inverse[2][1] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]);
Inverse[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
Inverse /=
Determinant;

return
Inverse;
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4<T, P>
inverseTranspose(tmat4x4<T, P>
const & m)
{
T SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
T SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
T SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
T SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
T SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
T SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
T SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
T SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
T SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
T SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
T SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
T SubFactor11 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
T SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
T SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
T SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
T SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
T SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
T SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
T SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

tmat4x4<T, P> Inverse(uninitialize);
Inverse[0][0] = + (m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02);
Inverse[0][1] = - (m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04);
Inverse[0][2] = + (m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05);
Inverse[0][3] = - (m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05);

Inverse[1][0] = - (m[0][1] * SubFactor00 - m[0][2] * SubFactor01 + m[0][3] * SubFactor02);
Inverse[1][1] = + (m[0][0] * SubFactor00 - m[0][2] * SubFactor03 + m[0][3] * SubFactor04);
Inverse[1][2] = - (m[0][0] * SubFactor01 - m[0][1] * SubFactor03 + m[0][3] * SubFactor05);
Inverse[1][3] = + (m[0][0] * SubFactor02 - m[0][1] * SubFactor04 + m[0][2] * SubFactor05);

Inverse[2][0] = + (m[0][1] * SubFactor06 - m[0][2] * SubFactor07 + m[0][3] * SubFactor08);
Inverse[2][1] = - (m[0][0] * SubFactor06 - m[0][2] * SubFactor09 + m[0][3] * SubFactor10);
Inverse[2][2] = + (m[0][0] * SubFactor11 - m[0][1] * SubFactor09 + m[0][3] * SubFactor12);
Inverse[2][3] = - (m[0][0] * SubFactor08 - m[0][1] * SubFactor10 + m[0][2] * SubFactor12);

Inverse[3][0] = - (m[0][1] * SubFactor13 - m[0][2] * SubFactor14 + m[0][3] * SubFactor15);
Inverse[3][1] = + (m[0][0] * SubFactor13 - m[0][2] * SubFactor16 + m[0][3] * SubFactor17);
Inverse[3][2] = - (m[0][0] * SubFactor14 - m[0][1] * SubFactor16 + m[0][3] * SubFactor18);
Inverse[3][3] = + (m[0][0] * SubFactor15 - m[0][1] * SubFactor17 + m[0][2] * SubFactor18);

T Determinant =
        +m[0][0] * Inverse[0][0]
                + m[0][1] * Inverse[0][1]
                + m[0][2] * Inverse[0][2]
                + m[0][3] * Inverse[0][3];

Inverse /=
Determinant;

return
Inverse;
}
}//namespace glm
