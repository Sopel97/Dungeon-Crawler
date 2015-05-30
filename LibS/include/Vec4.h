#ifndef VEC4_H
#define VEC4_H

template <class T>
class Vec4;

template <class T, size_t X, size_t Y, size_t Z, size_t W>
class Vec4Proxy : public Shape4<T>
{
public:
    static constexpr bool isWritable = ___internal::are_unique<size_t, X, Y, Z, W>::value;

    virtual ~Vec4Proxy(){}

    template <class U>
    Vec4Proxy<T, X, Y, Z, W>& operator=(const Vec4<U>& v1);

    Vec4Proxy<T, X, Y, Z, W>& operator=(const Vec4<T>& v1);
    Vec4Proxy<T, X, Y, Z, W>& operator=(Vec4<T> && v1);

    operator Vec4<T>() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    Vec4<T> operator+(const Vec4<T>& v1) const;
    Vec4<T> operator-(const Vec4<T>& v1) const;
    Vec4<T> operator*(const T scalar) const;
    Vec4<T> operator*(const Vec4<T>& v1) const;
    Vec4<T> operator/(const T scalar) const;
    Vec4<T> operator/(const Vec4<T>& v1) const;

    Vec4<T> operator-() const;

    Vec4Proxy<T, X, Y, Z, W>& operator+=(const Vec4<T>& v1);
    Vec4Proxy<T, X, Y, Z, W>& operator-=(const Vec4<T>& v1);
    Vec4Proxy<T, X, Y, Z, W>& operator*=(const T scalar);
    Vec4Proxy<T, X, Y, Z, W>& operator*=(const Vec4<T>& v1);
    Vec4Proxy<T, X, Y, Z, W>& operator/=(const T scalar);
    Vec4Proxy<T, X, Y, Z, W>& operator/=(const Vec4<T>& v1);

    T magnitude() const;
    T distanceTo(const Vec4<T>& v1) const;
    void normalize();
    Vec4<T> normalized() const;

    void fill(T value);

    virtual void translate(const Vec4<T>& v) {};
    virtual void scale(const Vec4<T>& c, const Vec4<T>& s) {};
    virtual void scale(const Vec4<T>& c, const T s) {};
    virtual void scale(const Vec4<T>& s) {};
    virtual void scale(const T s) {};

    template <class Transformation>
    void transform(Transformation&& func);
    template<class S>
    bool intersects(const S& b) const;
protected:
    T& getX();
    const T& getX() const;
    T& getY();
    const T& getY() const;
    T& getZ();
    const T& getZ() const;
    T& getW();
    const T& getW() const;
};

#include "Vec2.h"
#include "Vec3.h"

template <class T>
class Vec4 : public Shape4<T>
{
public:
    union
    {
        struct {T x, y, z, w;};
        struct {T v[4];};

        Vec2Proxy<T, 0, 0> xx;
        Vec2Proxy<T, 0, 1> xy;
        Vec2Proxy<T, 0, 2> xz;
        Vec2Proxy<T, 0, 3> xw;
        Vec2Proxy<T, 1, 0> yx;
        Vec2Proxy<T, 1, 1> yy;
        Vec2Proxy<T, 1, 2> yz;
        Vec2Proxy<T, 1, 3> yw;
        Vec2Proxy<T, 2, 0> zx;
        Vec2Proxy<T, 2, 1> zy;
        Vec2Proxy<T, 2, 2> zz;
        Vec2Proxy<T, 2, 3> zw;
        Vec2Proxy<T, 3, 0> wx;
        Vec2Proxy<T, 3, 1> wy;
        Vec2Proxy<T, 3, 2> wz;
        Vec2Proxy<T, 3, 3> ww;

        Vec3Proxy<T, 0, 0, 0> xxx;
        Vec3Proxy<T, 0, 0, 1> xxy;
        Vec3Proxy<T, 0, 0, 2> xxz;
        Vec3Proxy<T, 0, 0, 3> xxw;
        Vec3Proxy<T, 0, 1, 0> xyx;
        Vec3Proxy<T, 0, 1, 1> xyy;
        Vec3Proxy<T, 0, 1, 2> xyz;
        Vec3Proxy<T, 0, 1, 3> xyw;
        Vec3Proxy<T, 0, 2, 0> xzx;
        Vec3Proxy<T, 0, 2, 1> xzy;
        Vec3Proxy<T, 0, 2, 2> xzz;
        Vec3Proxy<T, 0, 2, 3> xzw;
        Vec3Proxy<T, 0, 3, 0> xwx;
        Vec3Proxy<T, 0, 3, 1> xwy;
        Vec3Proxy<T, 0, 3, 2> xwz;
        Vec3Proxy<T, 0, 3, 3> xww;
        Vec3Proxy<T, 1, 0, 0> yxx;
        Vec3Proxy<T, 1, 0, 1> yxy;
        Vec3Proxy<T, 1, 0, 2> yxz;
        Vec3Proxy<T, 1, 0, 3> yxw;
        Vec3Proxy<T, 1, 1, 0> yyx;
        Vec3Proxy<T, 1, 1, 1> yyy;
        Vec3Proxy<T, 1, 1, 2> yyz;
        Vec3Proxy<T, 1, 1, 3> yyw;
        Vec3Proxy<T, 1, 2, 0> yzx;
        Vec3Proxy<T, 1, 2, 1> yzy;
        Vec3Proxy<T, 1, 2, 2> yzz;
        Vec3Proxy<T, 1, 2, 3> yzw;
        Vec3Proxy<T, 1, 3, 0> ywx;
        Vec3Proxy<T, 1, 3, 1> ywy;
        Vec3Proxy<T, 1, 3, 2> ywz;
        Vec3Proxy<T, 1, 3, 3> yww;
        Vec3Proxy<T, 2, 0, 0> zxx;
        Vec3Proxy<T, 2, 0, 1> zxy;
        Vec3Proxy<T, 2, 0, 2> zxz;
        Vec3Proxy<T, 2, 0, 3> zxw;
        Vec3Proxy<T, 2, 1, 0> zyx;
        Vec3Proxy<T, 2, 1, 1> zyy;
        Vec3Proxy<T, 2, 1, 2> zyz;
        Vec3Proxy<T, 2, 1, 3> zyw;
        Vec3Proxy<T, 2, 2, 0> zzx;
        Vec3Proxy<T, 2, 2, 1> zzy;
        Vec3Proxy<T, 2, 2, 2> zzz;
        Vec3Proxy<T, 2, 2, 3> zzw;
        Vec3Proxy<T, 2, 3, 0> zwx;
        Vec3Proxy<T, 2, 3, 1> zwy;
        Vec3Proxy<T, 2, 3, 2> zwz;
        Vec3Proxy<T, 2, 3, 3> zww;
        Vec3Proxy<T, 3, 0, 0> wxx;
        Vec3Proxy<T, 3, 0, 1> wxy;
        Vec3Proxy<T, 3, 0, 2> wxz;
        Vec3Proxy<T, 3, 0, 3> wxw;
        Vec3Proxy<T, 3, 1, 0> wyx;
        Vec3Proxy<T, 3, 1, 1> wyy;
        Vec3Proxy<T, 3, 1, 2> wyz;
        Vec3Proxy<T, 3, 1, 3> wyw;
        Vec3Proxy<T, 3, 2, 0> wzx;
        Vec3Proxy<T, 3, 2, 1> wzy;
        Vec3Proxy<T, 3, 2, 2> wzz;
        Vec3Proxy<T, 3, 2, 3> wzw;
        Vec3Proxy<T, 3, 3, 0> wwx;
        Vec3Proxy<T, 3, 3, 1> wwy;
        Vec3Proxy<T, 3, 3, 2> wwz;
        Vec3Proxy<T, 3, 3, 3> www;

        Vec4Proxy<T, 0, 0, 0, 0> xxxx;
        Vec4Proxy<T, 0, 0, 0, 1> xxxy;
        Vec4Proxy<T, 0, 0, 0, 2> xxxz;
        Vec4Proxy<T, 0, 0, 0, 3> xxxw;
        Vec4Proxy<T, 0, 0, 1, 0> xxyx;
        Vec4Proxy<T, 0, 0, 1, 1> xxyy;
        Vec4Proxy<T, 0, 0, 1, 2> xxyz;
        Vec4Proxy<T, 0, 0, 1, 3> xxyw;
        Vec4Proxy<T, 0, 0, 2, 0> xxzx;
        Vec4Proxy<T, 0, 0, 2, 1> xxzy;
        Vec4Proxy<T, 0, 0, 2, 2> xxzz;
        Vec4Proxy<T, 0, 0, 2, 3> xxzw;
        Vec4Proxy<T, 0, 0, 3, 0> xxwx;
        Vec4Proxy<T, 0, 0, 3, 1> xxwy;
        Vec4Proxy<T, 0, 0, 3, 2> xxwz;
        Vec4Proxy<T, 0, 0, 3, 3> xxww;
        Vec4Proxy<T, 0, 1, 0, 0> xyxx;
        Vec4Proxy<T, 0, 1, 0, 1> xyxy;
        Vec4Proxy<T, 0, 1, 0, 2> xyxz;
        Vec4Proxy<T, 0, 1, 0, 3> xyxw;
        Vec4Proxy<T, 0, 1, 1, 0> xyyx;
        Vec4Proxy<T, 0, 1, 1, 1> xyyy;
        Vec4Proxy<T, 0, 1, 1, 2> xyyz;
        Vec4Proxy<T, 0, 1, 1, 3> xyyw;
        Vec4Proxy<T, 0, 1, 2, 0> xyzx;
        Vec4Proxy<T, 0, 1, 2, 1> xyzy;
        Vec4Proxy<T, 0, 1, 2, 2> xyzz;
        Vec4Proxy<T, 0, 1, 2, 3> xyzw;
        Vec4Proxy<T, 0, 1, 3, 0> xywx;
        Vec4Proxy<T, 0, 1, 3, 1> xywy;
        Vec4Proxy<T, 0, 1, 3, 2> xywz;
        Vec4Proxy<T, 0, 1, 3, 3> xyww;
        Vec4Proxy<T, 0, 2, 0, 0> xzxx;
        Vec4Proxy<T, 0, 2, 0, 1> xzxy;
        Vec4Proxy<T, 0, 2, 0, 2> xzxz;
        Vec4Proxy<T, 0, 2, 0, 3> xzxw;
        Vec4Proxy<T, 0, 2, 1, 0> xzyx;
        Vec4Proxy<T, 0, 2, 1, 1> xzyy;
        Vec4Proxy<T, 0, 2, 1, 2> xzyz;
        Vec4Proxy<T, 0, 2, 1, 3> xzyw;
        Vec4Proxy<T, 0, 2, 2, 0> xzzx;
        Vec4Proxy<T, 0, 2, 2, 1> xzzy;
        Vec4Proxy<T, 0, 2, 2, 2> xzzz;
        Vec4Proxy<T, 0, 2, 2, 3> xzzw;
        Vec4Proxy<T, 0, 2, 3, 0> xzwx;
        Vec4Proxy<T, 0, 2, 3, 1> xzwy;
        Vec4Proxy<T, 0, 2, 3, 2> xzwz;
        Vec4Proxy<T, 0, 2, 3, 3> xzww;
        Vec4Proxy<T, 0, 3, 0, 0> xwxx;
        Vec4Proxy<T, 0, 3, 0, 1> xwxy;
        Vec4Proxy<T, 0, 3, 0, 2> xwxz;
        Vec4Proxy<T, 0, 3, 0, 3> xwxw;
        Vec4Proxy<T, 0, 3, 1, 0> xwyx;
        Vec4Proxy<T, 0, 3, 1, 1> xwyy;
        Vec4Proxy<T, 0, 3, 1, 2> xwyz;
        Vec4Proxy<T, 0, 3, 1, 3> xwyw;
        Vec4Proxy<T, 0, 3, 2, 0> xwzx;
        Vec4Proxy<T, 0, 3, 2, 1> xwzy;
        Vec4Proxy<T, 0, 3, 2, 2> xwzz;
        Vec4Proxy<T, 0, 3, 2, 3> xwzw;
        Vec4Proxy<T, 0, 3, 3, 0> xwwx;
        Vec4Proxy<T, 0, 3, 3, 1> xwwy;
        Vec4Proxy<T, 0, 3, 3, 2> xwwz;
        Vec4Proxy<T, 0, 3, 3, 3> xwww;
        Vec4Proxy<T, 1, 0, 0, 0> yxxx;
        Vec4Proxy<T, 1, 0, 0, 1> yxxy;
        Vec4Proxy<T, 1, 0, 0, 2> yxxz;
        Vec4Proxy<T, 1, 0, 0, 3> yxxw;
        Vec4Proxy<T, 1, 0, 1, 0> yxyx;
        Vec4Proxy<T, 1, 0, 1, 1> yxyy;
        Vec4Proxy<T, 1, 0, 1, 2> yxyz;
        Vec4Proxy<T, 1, 0, 1, 3> yxyw;
        Vec4Proxy<T, 1, 0, 2, 0> yxzx;
        Vec4Proxy<T, 1, 0, 2, 1> yxzy;
        Vec4Proxy<T, 1, 0, 2, 2> yxzz;
        Vec4Proxy<T, 1, 0, 2, 3> yxzw;
        Vec4Proxy<T, 1, 0, 3, 0> yxwx;
        Vec4Proxy<T, 1, 0, 3, 1> yxwy;
        Vec4Proxy<T, 1, 0, 3, 2> yxwz;
        Vec4Proxy<T, 1, 0, 3, 3> yxww;
        Vec4Proxy<T, 1, 1, 0, 0> yyxx;
        Vec4Proxy<T, 1, 1, 0, 1> yyxy;
        Vec4Proxy<T, 1, 1, 0, 2> yyxz;
        Vec4Proxy<T, 1, 1, 0, 3> yyxw;
        Vec4Proxy<T, 1, 1, 1, 0> yyyx;
        Vec4Proxy<T, 1, 1, 1, 1> yyyy;
        Vec4Proxy<T, 1, 1, 1, 2> yyyz;
        Vec4Proxy<T, 1, 1, 1, 3> yyyw;
        Vec4Proxy<T, 1, 1, 2, 0> yyzx;
        Vec4Proxy<T, 1, 1, 2, 1> yyzy;
        Vec4Proxy<T, 1, 1, 2, 2> yyzz;
        Vec4Proxy<T, 1, 1, 2, 3> yyzw;
        Vec4Proxy<T, 1, 1, 3, 0> yywx;
        Vec4Proxy<T, 1, 1, 3, 1> yywy;
        Vec4Proxy<T, 1, 1, 3, 2> yywz;
        Vec4Proxy<T, 1, 1, 3, 3> yyww;
        Vec4Proxy<T, 1, 2, 0, 0> yzxx;
        Vec4Proxy<T, 1, 2, 0, 1> yzxy;
        Vec4Proxy<T, 1, 2, 0, 2> yzxz;
        Vec4Proxy<T, 1, 2, 0, 3> yzxw;
        Vec4Proxy<T, 1, 2, 1, 0> yzyx;
        Vec4Proxy<T, 1, 2, 1, 1> yzyy;
        Vec4Proxy<T, 1, 2, 1, 2> yzyz;
        Vec4Proxy<T, 1, 2, 1, 3> yzyw;
        Vec4Proxy<T, 1, 2, 2, 0> yzzx;
        Vec4Proxy<T, 1, 2, 2, 1> yzzy;
        Vec4Proxy<T, 1, 2, 2, 2> yzzz;
        Vec4Proxy<T, 1, 2, 2, 3> yzzw;
        Vec4Proxy<T, 1, 2, 3, 0> yzwx;
        Vec4Proxy<T, 1, 2, 3, 1> yzwy;
        Vec4Proxy<T, 1, 2, 3, 2> yzwz;
        Vec4Proxy<T, 1, 2, 3, 3> yzww;
        Vec4Proxy<T, 1, 3, 0, 0> ywxx;
        Vec4Proxy<T, 1, 3, 0, 1> ywxy;
        Vec4Proxy<T, 1, 3, 0, 2> ywxz;
        Vec4Proxy<T, 1, 3, 0, 3> ywxw;
        Vec4Proxy<T, 1, 3, 1, 0> ywyx;
        Vec4Proxy<T, 1, 3, 1, 1> ywyy;
        Vec4Proxy<T, 1, 3, 1, 2> ywyz;
        Vec4Proxy<T, 1, 3, 1, 3> ywyw;
        Vec4Proxy<T, 1, 3, 2, 0> ywzx;
        Vec4Proxy<T, 1, 3, 2, 1> ywzy;
        Vec4Proxy<T, 1, 3, 2, 2> ywzz;
        Vec4Proxy<T, 1, 3, 2, 3> ywzw;
        Vec4Proxy<T, 1, 3, 3, 0> ywwx;
        Vec4Proxy<T, 1, 3, 3, 1> ywwy;
        Vec4Proxy<T, 1, 3, 3, 2> ywwz;
        Vec4Proxy<T, 1, 3, 3, 3> ywww;
        Vec4Proxy<T, 2, 0, 0, 0> zxxx;
        Vec4Proxy<T, 2, 0, 0, 1> zxxy;
        Vec4Proxy<T, 2, 0, 0, 2> zxxz;
        Vec4Proxy<T, 2, 0, 0, 3> zxxw;
        Vec4Proxy<T, 2, 0, 1, 0> zxyx;
        Vec4Proxy<T, 2, 0, 1, 1> zxyy;
        Vec4Proxy<T, 2, 0, 1, 2> zxyz;
        Vec4Proxy<T, 2, 0, 1, 3> zxyw;
        Vec4Proxy<T, 2, 0, 2, 0> zxzx;
        Vec4Proxy<T, 2, 0, 2, 1> zxzy;
        Vec4Proxy<T, 2, 0, 2, 2> zxzz;
        Vec4Proxy<T, 2, 0, 2, 3> zxzw;
        Vec4Proxy<T, 2, 0, 3, 0> zxwx;
        Vec4Proxy<T, 2, 0, 3, 1> zxwy;
        Vec4Proxy<T, 2, 0, 3, 2> zxwz;
        Vec4Proxy<T, 2, 0, 3, 3> zxww;
        Vec4Proxy<T, 2, 1, 0, 0> zyxx;
        Vec4Proxy<T, 2, 1, 0, 1> zyxy;
        Vec4Proxy<T, 2, 1, 0, 2> zyxz;
        Vec4Proxy<T, 2, 1, 0, 3> zyxw;
        Vec4Proxy<T, 2, 1, 1, 0> zyyx;
        Vec4Proxy<T, 2, 1, 1, 1> zyyy;
        Vec4Proxy<T, 2, 1, 1, 2> zyyz;
        Vec4Proxy<T, 2, 1, 1, 3> zyyw;
        Vec4Proxy<T, 2, 1, 2, 0> zyzx;
        Vec4Proxy<T, 2, 1, 2, 1> zyzy;
        Vec4Proxy<T, 2, 1, 2, 2> zyzz;
        Vec4Proxy<T, 2, 1, 2, 3> zyzw;
        Vec4Proxy<T, 2, 1, 3, 0> zywx;
        Vec4Proxy<T, 2, 1, 3, 1> zywy;
        Vec4Proxy<T, 2, 1, 3, 2> zywz;
        Vec4Proxy<T, 2, 1, 3, 3> zyww;
        Vec4Proxy<T, 2, 2, 0, 0> zzxx;
        Vec4Proxy<T, 2, 2, 0, 1> zzxy;
        Vec4Proxy<T, 2, 2, 0, 2> zzxz;
        Vec4Proxy<T, 2, 2, 0, 3> zzxw;
        Vec4Proxy<T, 2, 2, 1, 0> zzyx;
        Vec4Proxy<T, 2, 2, 1, 1> zzyy;
        Vec4Proxy<T, 2, 2, 1, 2> zzyz;
        Vec4Proxy<T, 2, 2, 1, 3> zzyw;
        Vec4Proxy<T, 2, 2, 2, 0> zzzx;
        Vec4Proxy<T, 2, 2, 2, 1> zzzy;
        Vec4Proxy<T, 2, 2, 2, 2> zzzz;
        Vec4Proxy<T, 2, 2, 2, 3> zzzw;
        Vec4Proxy<T, 2, 2, 3, 0> zzwx;
        Vec4Proxy<T, 2, 2, 3, 1> zzwy;
        Vec4Proxy<T, 2, 2, 3, 2> zzwz;
        Vec4Proxy<T, 2, 2, 3, 3> zzww;
        Vec4Proxy<T, 2, 3, 0, 0> zwxx;
        Vec4Proxy<T, 2, 3, 0, 1> zwxy;
        Vec4Proxy<T, 2, 3, 0, 2> zwxz;
        Vec4Proxy<T, 2, 3, 0, 3> zwxw;
        Vec4Proxy<T, 2, 3, 1, 0> zwyx;
        Vec4Proxy<T, 2, 3, 1, 1> zwyy;
        Vec4Proxy<T, 2, 3, 1, 2> zwyz;
        Vec4Proxy<T, 2, 3, 1, 3> zwyw;
        Vec4Proxy<T, 2, 3, 2, 0> zwzx;
        Vec4Proxy<T, 2, 3, 2, 1> zwzy;
        Vec4Proxy<T, 2, 3, 2, 2> zwzz;
        Vec4Proxy<T, 2, 3, 2, 3> zwzw;
        Vec4Proxy<T, 2, 3, 3, 0> zwwx;
        Vec4Proxy<T, 2, 3, 3, 1> zwwy;
        Vec4Proxy<T, 2, 3, 3, 2> zwwz;
        Vec4Proxy<T, 2, 3, 3, 3> zwww;
        Vec4Proxy<T, 3, 0, 0, 0> wxxx;
        Vec4Proxy<T, 3, 0, 0, 1> wxxy;
        Vec4Proxy<T, 3, 0, 0, 2> wxxz;
        Vec4Proxy<T, 3, 0, 0, 3> wxxw;
        Vec4Proxy<T, 3, 0, 1, 0> wxyx;
        Vec4Proxy<T, 3, 0, 1, 1> wxyy;
        Vec4Proxy<T, 3, 0, 1, 2> wxyz;
        Vec4Proxy<T, 3, 0, 1, 3> wxyw;
        Vec4Proxy<T, 3, 0, 2, 0> wxzx;
        Vec4Proxy<T, 3, 0, 2, 1> wxzy;
        Vec4Proxy<T, 3, 0, 2, 2> wxzz;
        Vec4Proxy<T, 3, 0, 2, 3> wxzw;
        Vec4Proxy<T, 3, 0, 3, 0> wxwx;
        Vec4Proxy<T, 3, 0, 3, 1> wxwy;
        Vec4Proxy<T, 3, 0, 3, 2> wxwz;
        Vec4Proxy<T, 3, 0, 3, 3> wxww;
        Vec4Proxy<T, 3, 1, 0, 0> wyxx;
        Vec4Proxy<T, 3, 1, 0, 1> wyxy;
        Vec4Proxy<T, 3, 1, 0, 2> wyxz;
        Vec4Proxy<T, 3, 1, 0, 3> wyxw;
        Vec4Proxy<T, 3, 1, 1, 0> wyyx;
        Vec4Proxy<T, 3, 1, 1, 1> wyyy;
        Vec4Proxy<T, 3, 1, 1, 2> wyyz;
        Vec4Proxy<T, 3, 1, 1, 3> wyyw;
        Vec4Proxy<T, 3, 1, 2, 0> wyzx;
        Vec4Proxy<T, 3, 1, 2, 1> wyzy;
        Vec4Proxy<T, 3, 1, 2, 2> wyzz;
        Vec4Proxy<T, 3, 1, 2, 3> wyzw;
        Vec4Proxy<T, 3, 1, 3, 0> wywx;
        Vec4Proxy<T, 3, 1, 3, 1> wywy;
        Vec4Proxy<T, 3, 1, 3, 2> wywz;
        Vec4Proxy<T, 3, 1, 3, 3> wyww;
        Vec4Proxy<T, 3, 2, 0, 0> wzxx;
        Vec4Proxy<T, 3, 2, 0, 1> wzxy;
        Vec4Proxy<T, 3, 2, 0, 2> wzxz;
        Vec4Proxy<T, 3, 2, 0, 3> wzxw;
        Vec4Proxy<T, 3, 2, 1, 0> wzyx;
        Vec4Proxy<T, 3, 2, 1, 1> wzyy;
        Vec4Proxy<T, 3, 2, 1, 2> wzyz;
        Vec4Proxy<T, 3, 2, 1, 3> wzyw;
        Vec4Proxy<T, 3, 2, 2, 0> wzzx;
        Vec4Proxy<T, 3, 2, 2, 1> wzzy;
        Vec4Proxy<T, 3, 2, 2, 2> wzzz;
        Vec4Proxy<T, 3, 2, 2, 3> wzzw;
        Vec4Proxy<T, 3, 2, 3, 0> wzwx;
        Vec4Proxy<T, 3, 2, 3, 1> wzwy;
        Vec4Proxy<T, 3, 2, 3, 2> wzwz;
        Vec4Proxy<T, 3, 2, 3, 3> wzww;
        Vec4Proxy<T, 3, 3, 0, 0> wwxx;
        Vec4Proxy<T, 3, 3, 0, 1> wwxy;
        Vec4Proxy<T, 3, 3, 0, 2> wwxz;
        Vec4Proxy<T, 3, 3, 0, 3> wwxw;
        Vec4Proxy<T, 3, 3, 1, 0> wwyx;
        Vec4Proxy<T, 3, 3, 1, 1> wwyy;
        Vec4Proxy<T, 3, 3, 1, 2> wwyz;
        Vec4Proxy<T, 3, 3, 1, 3> wwyw;
        Vec4Proxy<T, 3, 3, 2, 0> wwzx;
        Vec4Proxy<T, 3, 3, 2, 1> wwzy;
        Vec4Proxy<T, 3, 3, 2, 2> wwzz;
        Vec4Proxy<T, 3, 3, 2, 3> wwzw;
        Vec4Proxy<T, 3, 3, 3, 0> wwwx;
        Vec4Proxy<T, 3, 3, 3, 1> wwwy;
        Vec4Proxy<T, 3, 3, 3, 2> wwwz;
        Vec4Proxy<T, 3, 3, 3, 3> wwww;
    };

    static const Vec4<T> unitX;
    static const Vec4<T> unitY;
    static const Vec4<T> unitZ;
    static const Vec4<T> unitW;

    Vec4() = default;
    Vec4(T _x, T _y, T _z, T _w);
    Vec4(const std::initializer_list<T>& list);

    virtual ~Vec4(){}

    Vec4(const Vec4<T>& v) = default;
    template <class X>
    Vec4(const Vec4<X>& v);
    Vec4(Vec4<T>&& v) = default;

    Vec4<T>& operator=(const Vec4<T>& v1) = default;
    template <class X>
    Vec4<T>& operator=(const Vec4<X>& v1);
    Vec4<T>& operator=(Vec4<T> && v1) = default;

    inline T& operator [](int i);
    inline const T& operator [](int i) const;

    Vec4<T> operator+(const Vec4<T>& v1) const;
    Vec4<T> operator-(const Vec4<T>& v1) const;
    Vec4<T> operator*(const T scalar) const;
    Vec4<T> operator*(const Vec4<T>& v1) const;
    Vec4<T> operator/(const T scalar) const;

    Vec4<T> operator-() const;

    Vec4<T>& operator+=(const Vec4<T>& v1);
    Vec4<T>& operator-=(const Vec4<T>& v1);
    Vec4<T>& operator*=(const T scalar);
    Vec4<T>& operator*=(const Vec4<T>& v1);
    Vec4<T>& operator/=(const T scalar);

    T magnitude();
    T distance(const Vec4<T>& v);
    void normalize();
    Vec4<T> normalized();

    void fill(T value);

    virtual void translate(const Vec4<T>& v) {};
    virtual void scale(const Vec4<T>& c, const Vec4<T>& s) {};
    virtual void scale(const Vec4<T>& c, const T s) {};
    virtual void scale(const Vec4<T>& s) {};
    virtual void scale(const T s) {};

    template <class Transformation>
    void transform(Transformation&& func);
};

template <class T>
const Vec4<T> Vec4<T>::unitX = Vec4<T> {1, 0, 0, 0};
template <class T>
const Vec4<T> Vec4<T>::unitY = Vec4<T> {0, 1, 0, 0};
template <class T>
const Vec4<T> Vec4<T>::unitZ = Vec4<T> {0, 0, 1, 0};
template <class T>
const Vec4<T> Vec4<T>::unitW = Vec4<T> {0, 0, 1, 1};

typedef Vec4<double> Vec4D;
typedef Vec4<float> Vec4F;
typedef Vec4<int> Vec4I;

#include "../src/Vec4.cpp"

#endif // VEC4_H
