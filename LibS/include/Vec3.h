#ifndef Vec3_H_INCLUDED
#define Vec3_H_INCLUDED

template <class T>
class Vec3;

template <class T, size_t X, size_t Y, size_t Z>
class Vec3Proxy : public Shape3<T>
{
public:
    static constexpr bool isWritable = ___internal::are_unique<size_t, X, Y, Z>::value;

    virtual ~Vec3Proxy(){}

    template <class U>
    Vec3Proxy<T, X, Y, Z>& operator=(const Vec3<U>& v1);

    Vec3Proxy<T, X, Y, Z>& operator=(const Vec3<T>& v1);
    Vec3Proxy<T, X, Y, Z>& operator=(Vec3<T> && v1);

    operator Vec3<T>() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    Vec3<T> operator+(const Vec3<T>& v1) const;
    Vec3<T> operator-(const Vec3<T>& v1) const;
    Vec3<T> operator*(const T scalar) const;
    Vec3<T> operator*(const Vec3<T>& v1) const;
    Vec3<T> operator/(const T scalar) const;
    Vec3<T> operator/(const Vec3<T>& v1) const;

    Vec3<T> operator-() const;

    Vec3Proxy<T, X, Y, Z>& operator+=(const Vec3<T>& v1);
    Vec3Proxy<T, X, Y, Z>& operator-=(const Vec3<T>& v1);
    Vec3Proxy<T, X, Y, Z>& operator*=(const T scalar);
    Vec3Proxy<T, X, Y, Z>& operator*=(const Vec3<T>& v1);
    Vec3Proxy<T, X, Y, Z>& operator/=(const T scalar);
    Vec3Proxy<T, X, Y, Z>& operator/=(const Vec3<T>& v1);

    T magnitude() const;
    T distanceTo(const Vec3<T>& v1) const;
    void normalize();
    Vec3<T> normalized() const;

    void fill(T value);

    virtual void translate(const Vec3<T>& v) {};
    virtual void scale(const Vec3<T>& c, const Vec3<T>& s) {};
    virtual void scale(const Vec3<T>& c, const T s) {};
    virtual void scale(const Vec3<T>& s) {};
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
};

#include "Vec2.h"
#include "Vec4.h"

template <class T>
class Vec3 : public Shape3<T>
{
public:
    union
    {
        struct {T x, y, z;};
        struct {T v[3];};

        Vec2Proxy<T, 0, 0> xx;
        Vec2Proxy<T, 0, 1> xy;
        Vec2Proxy<T, 0, 2> xz;
        Vec2Proxy<T, 1, 0> yx;
        Vec2Proxy<T, 1, 1> yy;
        Vec2Proxy<T, 1, 2> yz;
        Vec2Proxy<T, 2, 0> zx;
        Vec2Proxy<T, 2, 1> zy;
        Vec2Proxy<T, 2, 2> zz;

        Vec3Proxy<T, 0, 0, 0> xxx;
        Vec3Proxy<T, 0, 0, 1> xxy;
        Vec3Proxy<T, 0, 0, 2> xxz;
        Vec3Proxy<T, 0, 1, 0> xyx;
        Vec3Proxy<T, 0, 1, 1> xyy;
        Vec3Proxy<T, 0, 1, 2> xyz;
        Vec3Proxy<T, 0, 2, 0> xzx;
        Vec3Proxy<T, 0, 2, 1> xzy;
        Vec3Proxy<T, 0, 2, 2> xzz;
        Vec3Proxy<T, 1, 0, 0> yxx;
        Vec3Proxy<T, 1, 0, 1> yxy;
        Vec3Proxy<T, 1, 0, 2> yxz;
        Vec3Proxy<T, 1, 1, 0> yyx;
        Vec3Proxy<T, 1, 1, 1> yyy;
        Vec3Proxy<T, 1, 1, 2> yyz;
        Vec3Proxy<T, 1, 2, 0> yzx;
        Vec3Proxy<T, 1, 2, 1> yzy;
        Vec3Proxy<T, 1, 2, 2> yzz;
        Vec3Proxy<T, 2, 0, 0> zxx;
        Vec3Proxy<T, 2, 0, 1> zxy;
        Vec3Proxy<T, 2, 0, 2> zxz;
        Vec3Proxy<T, 2, 1, 0> zyx;
        Vec3Proxy<T, 2, 1, 1> zyy;
        Vec3Proxy<T, 2, 1, 2> zyz;
        Vec3Proxy<T, 2, 2, 0> zzx;
        Vec3Proxy<T, 2, 2, 1> zzy;
        Vec3Proxy<T, 2, 2, 2> zzz;

        Vec4Proxy<T, 0, 0, 0, 0> xxxx;
        Vec4Proxy<T, 0, 0, 0, 1> xxxy;
        Vec4Proxy<T, 0, 0, 0, 2> xxxz;
        Vec4Proxy<T, 0, 0, 1, 0> xxyx;
        Vec4Proxy<T, 0, 0, 1, 1> xxyy;
        Vec4Proxy<T, 0, 0, 1, 2> xxyz;
        Vec4Proxy<T, 0, 0, 2, 0> xxzx;
        Vec4Proxy<T, 0, 0, 2, 1> xxzy;
        Vec4Proxy<T, 0, 0, 2, 2> xxzz;
        Vec4Proxy<T, 0, 1, 0, 0> xyxx;
        Vec4Proxy<T, 0, 1, 0, 1> xyxy;
        Vec4Proxy<T, 0, 1, 0, 2> xyxz;
        Vec4Proxy<T, 0, 1, 1, 0> xyyx;
        Vec4Proxy<T, 0, 1, 1, 1> xyyy;
        Vec4Proxy<T, 0, 1, 1, 2> xyyz;
        Vec4Proxy<T, 0, 1, 2, 0> xyzx;
        Vec4Proxy<T, 0, 1, 2, 1> xyzy;
        Vec4Proxy<T, 0, 1, 2, 2> xyzz;
        Vec4Proxy<T, 0, 2, 0, 0> xzxx;
        Vec4Proxy<T, 0, 2, 0, 1> xzxy;
        Vec4Proxy<T, 0, 2, 0, 2> xzxz;
        Vec4Proxy<T, 0, 2, 1, 0> xzyx;
        Vec4Proxy<T, 0, 2, 1, 1> xzyy;
        Vec4Proxy<T, 0, 2, 1, 2> xzyz;
        Vec4Proxy<T, 0, 2, 2, 0> xzzx;
        Vec4Proxy<T, 0, 2, 2, 1> xzzy;
        Vec4Proxy<T, 0, 2, 2, 2> xzzz;
        Vec4Proxy<T, 1, 0, 0, 0> yxxx;
        Vec4Proxy<T, 1, 0, 0, 1> yxxy;
        Vec4Proxy<T, 1, 0, 0, 2> yxxz;
        Vec4Proxy<T, 1, 0, 1, 0> yxyx;
        Vec4Proxy<T, 1, 0, 1, 1> yxyy;
        Vec4Proxy<T, 1, 0, 1, 2> yxyz;
        Vec4Proxy<T, 1, 0, 2, 0> yxzx;
        Vec4Proxy<T, 1, 0, 2, 1> yxzy;
        Vec4Proxy<T, 1, 0, 2, 2> yxzz;
        Vec4Proxy<T, 1, 1, 0, 0> yyxx;
        Vec4Proxy<T, 1, 1, 0, 1> yyxy;
        Vec4Proxy<T, 1, 1, 0, 2> yyxz;
        Vec4Proxy<T, 1, 1, 1, 0> yyyx;
        Vec4Proxy<T, 1, 1, 1, 1> yyyy;
        Vec4Proxy<T, 1, 1, 1, 2> yyyz;
        Vec4Proxy<T, 1, 1, 2, 0> yyzx;
        Vec4Proxy<T, 1, 1, 2, 1> yyzy;
        Vec4Proxy<T, 1, 1, 2, 2> yyzz;
        Vec4Proxy<T, 1, 2, 0, 0> yzxx;
        Vec4Proxy<T, 1, 2, 0, 1> yzxy;
        Vec4Proxy<T, 1, 2, 0, 2> yzxz;
        Vec4Proxy<T, 1, 2, 1, 0> yzyx;
        Vec4Proxy<T, 1, 2, 1, 1> yzyy;
        Vec4Proxy<T, 1, 2, 1, 2> yzyz;
        Vec4Proxy<T, 1, 2, 2, 0> yzzx;
        Vec4Proxy<T, 1, 2, 2, 1> yzzy;
        Vec4Proxy<T, 1, 2, 2, 2> yzzz;
        Vec4Proxy<T, 2, 0, 0, 0> zxxx;
        Vec4Proxy<T, 2, 0, 0, 1> zxxy;
        Vec4Proxy<T, 2, 0, 0, 2> zxxz;
        Vec4Proxy<T, 2, 0, 1, 0> zxyx;
        Vec4Proxy<T, 2, 0, 1, 1> zxyy;
        Vec4Proxy<T, 2, 0, 1, 2> zxyz;
        Vec4Proxy<T, 2, 0, 2, 0> zxzx;
        Vec4Proxy<T, 2, 0, 2, 1> zxzy;
        Vec4Proxy<T, 2, 0, 2, 2> zxzz;
        Vec4Proxy<T, 2, 1, 0, 0> zyxx;
        Vec4Proxy<T, 2, 1, 0, 1> zyxy;
        Vec4Proxy<T, 2, 1, 0, 2> zyxz;
        Vec4Proxy<T, 2, 1, 1, 0> zyyx;
        Vec4Proxy<T, 2, 1, 1, 1> zyyy;
        Vec4Proxy<T, 2, 1, 1, 2> zyyz;
        Vec4Proxy<T, 2, 1, 2, 0> zyzx;
        Vec4Proxy<T, 2, 1, 2, 1> zyzy;
        Vec4Proxy<T, 2, 1, 2, 2> zyzz;
        Vec4Proxy<T, 2, 2, 0, 0> zzxx;
        Vec4Proxy<T, 2, 2, 0, 1> zzxy;
        Vec4Proxy<T, 2, 2, 0, 2> zzxz;
        Vec4Proxy<T, 2, 2, 1, 0> zzyx;
        Vec4Proxy<T, 2, 2, 1, 1> zzyy;
        Vec4Proxy<T, 2, 2, 1, 2> zzyz;
        Vec4Proxy<T, 2, 2, 2, 0> zzzx;
        Vec4Proxy<T, 2, 2, 2, 1> zzzy;
        Vec4Proxy<T, 2, 2, 2, 2> zzzz;
    };

    static const Vec3<T> unitX;
    static const Vec3<T> unitY;
    static const Vec3<T> unitZ;

    Vec3() = default;
    Vec3(T _x, T _y, T _z);
    Vec3(const std::initializer_list<T>& list);

    Vec3(const Vec3<T>& v) = default;
    template <class X>
    Vec3(const Vec3<X>& v);
    Vec3(Vec3<T>&& v) = default;

    virtual ~Vec3(){}

    Vec3<T>& operator=(const Vec3<T>& v1) = default;
    template <class X>
    Vec3<T>& operator=(const Vec3<X>& v1);
    Vec3<T>& operator=(Vec3<T> && v1) = default;

    inline T& operator [](int i);
    inline const T& operator [](int i) const;

    Vec3<T> operator+(const Vec3<T>& v1) const;
    Vec3<T> operator-(const Vec3<T>& v1) const;
    Vec3<T> operator*(const T scalar) const;
    Vec3<T> operator*(const Vec3<T>& v1) const;
    Vec3<T> operator/(const T scalar) const;

    Vec3<T> operator-() const;

    Vec3<T>& operator+=(const Vec3<T>& v1);
    Vec3<T>& operator-=(const Vec3<T>& v1);
    Vec3<T>& operator*=(const T scalar);
    Vec3<T>& operator*=(const Vec3<T>& v1);
    Vec3<T>& operator/=(const T scalar);

    T magnitude();
    T distance(const Vec3<T>& v);
    void normalize();
    Vec3<T> normalized();

    void fill(T value);

    virtual void translate(const Vec3<T>& v) {};
    virtual void scale(const Vec3<T>& c, const Vec3<T>& s) {};
    virtual void scale(const Vec3<T>& c, const T s) {};
    virtual void scale(const Vec3<T>& s) {};
    virtual void scale(const T s) {};

    template <class Transformation>
    void transform(Transformation&& func);

    template<class S>
    bool intersects(const S& b) const;
};

template <class T>
const Vec3<T> Vec3<T>::unitX = Vec3<T> {1, 0, 0};
template <class T>
const Vec3<T> Vec3<T>::unitY = Vec3<T> {0, 1, 0};
template <class T>
const Vec3<T> Vec3<T>::unitZ = Vec3<T> {0, 0, 1};

typedef Vec3<double> Vec3D;
typedef Vec3<float> Vec3F;
typedef Vec3<int> Vec3I;

#include "../src/Vec3.cpp"
#endif // Vec3_H_INCLUDED
