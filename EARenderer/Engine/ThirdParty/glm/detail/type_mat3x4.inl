/// @ref core
/// @file glm/detail/type_mat3x4.inl

namespace glm {
    // -- Constructors --

#	if !GLM_HAS_DEFAULTED_FUNCTIONS || !defined(GLM_FORCE_NO_CTOR_INIT)

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4() {
#			ifndef GLM_FORCE_NO_CTOR_INIT
        this->value[0] = col_type(1, 0, 0, 0);
        this->value[1] = col_type(0, 1, 0, 0);
        this->value[2] = col_type(0, 0, 1, 0);
#			endif
    }

#	endif

#	if !GLM_HAS_DEFAULTED_FUNCTIONS
    template <typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat3x4<T, P> const & m)
    {
        this->value[0] = m.value[0];
        this->value[1] = m.value[1];
        this->value[2] = m.value[2];
    }
#	endif//!GLM_HAS_DEFAULTED_FUNCTIONS

    template<typename T, precision P>
    template<precision Q>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat3x4 <T, Q> const &m) {
        this->value[0] = m.value[0];
        this->value[1] = m.value[1];
        this->value[2] = m.value[2];
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR_CTOR tmat3x4<T, P>::tmat3x4(ctor) {
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(T scalar) {
        this->value[0] = col_type(scalar, 0, 0, 0);
        this->value[1] = col_type(0, scalar, 0, 0);
        this->value[2] = col_type(0, 0, scalar, 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4
            (
                    T x0, T y0, T z0, T w0,
                    T x1, T y1, T z1, T w1,
                    T x2, T y2, T z2, T w2
            ) {
        this->value[0] = col_type(x0, y0, z0, w0);
        this->value[1] = col_type(x1, y1, z1, w1);
        this->value[2] = col_type(x2, y2, z2, w2);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4
            (
                    col_type const &v0,
                    col_type const &v1,
                    col_type const &v2
            ) {
        this->value[0] = v0;
        this->value[1] = v1;
        this->value[2] = v2;
    }

    // -- Conversion constructors --

    template<typename T, precision P>
    template<
            typename X1, typename Y1, typename Z1, typename W1,
            typename X2, typename Y2, typename Z2, typename W2,
            typename X3, typename Y3, typename Z3, typename W3>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4
            (
                    X1 x1, Y1 y1, Z1 z1, W1 w1,
                    X2 x2, Y2 y2, Z2 z2, W2 w2,
                    X3 x3, Y3 y3, Z3 z3, W3 w3
            ) {
        this->value[0] = col_type(static_cast<T>(x1), value_type(y1), value_type(z1), value_type(w1));
        this->value[1] = col_type(static_cast<T>(x2), value_type(y2), value_type(z2), value_type(w2));
        this->value[2] = col_type(static_cast<T>(x3), value_type(y3), value_type(z3), value_type(w3));
    }

    template<typename T, precision P>
    template<typename V1, typename V2, typename V3>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4
            (
                    tvec4 <V1, P> const &v1,
                    tvec4 <V2, P> const &v2,
                    tvec4 <V3, P> const &v3
            ) {
        this->value[0] = col_type(v1);
        this->value[1] = col_type(v2);
        this->value[2] = col_type(v3);
    }

    // -- Matrix conversions --

    template<typename T, precision P>
    template<typename U, precision Q>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat3x4 <U, Q> const &m) {
        this->value[0] = col_type(m[0]);
        this->value[1] = col_type(m[1]);
        this->value[2] = col_type(m[2]);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat2x2 <T, P> const &m) {
        this->value[0] = col_type(m[0], 0, 0);
        this->value[1] = col_type(m[1], 0, 0);
        this->value[2] = col_type(0, 0, 1, 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat3x3 <T, P> const &m) {
        this->value[0] = col_type(m[0], 0);
        this->value[1] = col_type(m[1], 0);
        this->value[2] = col_type(m[2], 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat4x4 <T, P> const &m) {
        this->value[0] = col_type(m[0]);
        this->value[1] = col_type(m[1]);
        this->value[2] = col_type(m[2]);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat2x3 <T, P> const &m) {
        this->value[0] = col_type(m[0], 0);
        this->value[1] = col_type(m[1], 0);
        this->value[2] = col_type(0, 0, 1, 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat3x2 <T, P> const &m) {
        this->value[0] = col_type(m[0], 0, 0);
        this->value[1] = col_type(m[1], 0, 0);
        this->value[2] = col_type(m[2], 1, 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat2x4 <T, P> const &m) {
        this->value[0] = col_type(m[0]);
        this->value[1] = col_type(m[1]);
        this->value[2] = col_type(0, 0, 1, 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat4x2 <T, P> const &m) {
        this->value[0] = col_type(m[0], 0, 0);
        this->value[1] = col_type(m[1], 0, 0);
        this->value[2] = col_type(m[2], 1, 0);
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>::tmat3x4(tmat4x3 <T, P> const &m) {
        this->value[0] = col_type(m[0], 0);
        this->value[1] = col_type(m[1], 0);
        this->value[2] = col_type(m[2], 0);
    }

    // -- Accesses --

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER typename tmat3x4<T, P>::col_type &tmat3x4<T, P>::operator[](typename tmat3x4<T, P>::length_type i) {
        assert(i < this->length());
        return this->value[i];
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER typename tmat3x4<T, P>::col_type const &tmat3x4<T, P>::operator[](typename tmat3x4<T, P>::length_type i) const {
        assert(i < this->length());
        return this->value[i];
    }

    // -- Unary updatable operators --

#	if !GLM_HAS_DEFAULTED_FUNCTIONS
    template <typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4<T, P>& tmat3x4<T, P>::operator=(tmat3x4<T, P> const & m)
    {
        this->value[0] = m[0];
        this->value[1] = m[1];
        this->value[2] = m[2];
        return *this;
    }
#	endif//!GLM_HAS_DEFAULTED_FUNCTIONS

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator=(tmat3x4 <U, P> const &m) {
        this->value[0] = m[0];
        this->value[1] = m[1];
        this->value[2] = m[2];
        return *this;
    }

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator+=(U s) {
        this->value[0] += s;
        this->value[1] += s;
        this->value[2] += s;
        return *this;
    }

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator+=(tmat3x4 <U, P> const &m) {
        this->value[0] += m[0];
        this->value[1] += m[1];
        this->value[2] += m[2];
        return *this;
    }

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator-=(U s) {
        this->value[0] -= s;
        this->value[1] -= s;
        this->value[2] -= s;
        return *this;
    }

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator-=(tmat3x4 <U, P> const &m) {
        this->value[0] -= m[0];
        this->value[1] -= m[1];
        this->value[2] -= m[2];
        return *this;
    }

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator*=(U s) {
        this->value[0] *= s;
        this->value[1] *= s;
        this->value[2] *= s;
        return *this;
    }

    template<typename T, precision P>
    template<typename U>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator/=(U s) {
        this->value[0] /= s;
        this->value[1] /= s;
        this->value[2] /= s;
        return *this;
    }

    // -- Increment and decrement operators --

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator++() {
        ++this->value[0];
        ++this->value[1];
        ++this->value[2];
        return *this;
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> &tmat3x4<T, P>::operator--() {
        --this->value[0];
        --this->value[1];
        --this->value[2];
        return *this;
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> tmat3x4<T, P>::operator++(int) {
        tmat3x4<T, P> Result(*this);
        ++*this;
        return Result;
    }

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> tmat3x4<T, P>::operator--(int) {
        tmat3x4<T, P> Result(*this);
        --*this;
        return Result;
    }

    // -- Unary arithmetic operators --

    template<typename T, precision P>
    GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator+(tmat3x4 < T, P >
    const & m) {
    return
    m;
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator-(tmat3x4 < T, P >
const & m)
{
return tmat3x4<T, P>(
-m[0],
-m[1],
-m[2]);
}

// -- Binary arithmetic operators --

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator+(tmat3x4 < T, P >
const & m,
T scalar
)
{
return
tmat3x4<T, P>(
        m[0]
+ scalar,
m[1] + scalar,
m[2] + scalar);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator+(tmat3x4 < T, P >
const & m1,
tmat3x4 <T, P> const &m2
)
{
return
tmat3x4<T, P>(
        m1[0]
+ m2[0],
m1[1] + m2[1],
m1[2] + m2[2]);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator-(tmat3x4 < T, P >
const & m,
T scalar
)
{
return
tmat3x4<T, P>(
        m[0]
- scalar,
m[1] - scalar,
m[2] - scalar);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator-(tmat3x4 < T, P >
const & m1,
tmat3x4 <T, P> const &m2
)
{
return
tmat3x4<T, P>(
        m1[0]
- m2[0],
m1[1] - m2[1],
m1[2] - m2[2]);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator*(tmat3x4 < T, P >
const & m,
T scalar
)
{
return
tmat3x4<T, P>(
        m[0]
* scalar,
m[1] * scalar,
m[2] * scalar);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator*(T scalar, tmat3x4 <T, P> const &m) {
    return tmat3x4<T, P>(
            m[0] * scalar,
            m[1] * scalar,
            m[2] * scalar);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER typename tmat3x4<T, P>::col_type operator*
        (
                tmat3x4 < T, P >
const & m,
typename tmat3x4<T, P>::row_type const &v
)
{
return
typename tmat3x4<T, P>::col_type(
        m[0][0]
* v.x + m[1][0] * v.y + m[2][0] * v.z,
m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z,
m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER typename tmat3x4<T, P>::row_type operator*
        (
                typename tmat3x4<T, P>::col_type const &v,
                tmat3x4 <T, P> const &m
        ) {
    return typename tmat3x4<T, P>::row_type(
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3]);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat4x4 <T, P> operator*(tmat3x4 < T, P >
const & m1,
tmat4x3 <T, P> const &m2
)
{
const T SrcA00 = m1[0][0];
const T SrcA01 = m1[0][1];
const T SrcA02 = m1[0][2];
const T SrcA03 = m1[0][3];
const T SrcA10 = m1[1][0];
const T SrcA11 = m1[1][1];
const T SrcA12 = m1[1][2];
const T SrcA13 = m1[1][3];
const T SrcA20 = m1[2][0];
const T SrcA21 = m1[2][1];
const T SrcA22 = m1[2][2];
const T SrcA23 = m1[2][3];

const T SrcB00 = m2[0][0];
const T SrcB01 = m2[0][1];
const T SrcB02 = m2[0][2];
const T SrcB10 = m2[1][0];
const T SrcB11 = m2[1][1];
const T SrcB12 = m2[1][2];
const T SrcB20 = m2[2][0];
const T SrcB21 = m2[2][1];
const T SrcB22 = m2[2][2];
const T SrcB30 = m2[3][0];
const T SrcB31 = m2[3][1];
const T SrcB32 = m2[3][2];

tmat4x4 <T, P> Result(uninitialize);
Result[0][0] =
SrcA00 *SrcB00
+
SrcA10 *SrcB01
+
SrcA20 *SrcB02;
Result[0][1] =
SrcA01 *SrcB00
+
SrcA11 *SrcB01
+
SrcA21 *SrcB02;
Result[0][2] =
SrcA02 *SrcB00
+
SrcA12 *SrcB01
+
SrcA22 *SrcB02;
Result[0][3] =
SrcA03 *SrcB00
+
SrcA13 *SrcB01
+
SrcA23 *SrcB02;
Result[1][0] =
SrcA00 *SrcB10
+
SrcA10 *SrcB11
+
SrcA20 *SrcB12;
Result[1][1] =
SrcA01 *SrcB10
+
SrcA11 *SrcB11
+
SrcA21 *SrcB12;
Result[1][2] =
SrcA02 *SrcB10
+
SrcA12 *SrcB11
+
SrcA22 *SrcB12;
Result[1][3] =
SrcA03 *SrcB10
+
SrcA13 *SrcB11
+
SrcA23 *SrcB12;
Result[2][0] =
SrcA00 *SrcB20
+
SrcA10 *SrcB21
+
SrcA20 *SrcB22;
Result[2][1] =
SrcA01 *SrcB20
+
SrcA11 *SrcB21
+
SrcA21 *SrcB22;
Result[2][2] =
SrcA02 *SrcB20
+
SrcA12 *SrcB21
+
SrcA22 *SrcB22;
Result[2][3] =
SrcA03 *SrcB20
+
SrcA13 *SrcB21
+
SrcA23 *SrcB22;
Result[3][0] =
SrcA00 *SrcB30
+
SrcA10 *SrcB31
+
SrcA20 *SrcB32;
Result[3][1] =
SrcA01 *SrcB30
+
SrcA11 *SrcB31
+
SrcA21 *SrcB32;
Result[3][2] =
SrcA02 *SrcB30
+
SrcA12 *SrcB31
+
SrcA22 *SrcB32;
Result[3][3] =
SrcA03 *SrcB30
+
SrcA13 *SrcB31
+
SrcA23 *SrcB32;
return
Result;
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat2x4 <T, P> operator*(tmat3x4 < T, P >
const & m1,
tmat2x3 <T, P> const &m2
)
{
return
tmat2x4<T, P>(
        m1[0][0]
* m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2],
m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2]);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator*(tmat3x4 < T, P >
const & m1,
tmat3x3 <T, P> const &m2
)
{
return
tmat3x4<T, P>(
        m1[0][0]
* m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2],
m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2],
m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2],
m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2]);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator/(tmat3x4 < T, P >
const & m,
T scalar
)
{
return
tmat3x4<T, P>(
        m[0]
/ scalar,
m[1] / scalar,
m[2] / scalar);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER tmat3x4 <T, P> operator/(T scalar, tmat3x4 <T, P> const &m) {
    return tmat3x4<T, P>(
            scalar / m[0],
            scalar / m[1],
            scalar / m[2]);
}

// -- Boolean operators --

template<typename T, precision P>
GLM_FUNC_QUALIFIER bool operator==(tmat3x4 < T, P >
const & m1,
tmat3x4 <T, P> const &m2
)
{
return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
}

template<typename T, precision P>
GLM_FUNC_QUALIFIER bool operator!=(tmat3x4 < T, P >
const & m1,
tmat3x4 <T, P> const &m2
)
{
return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
}
} //namespace glm
