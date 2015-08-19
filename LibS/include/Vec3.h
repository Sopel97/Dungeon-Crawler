#ifndef Vec3_H_INCLUDED
#define Vec3_H_INCLUDED

template <class T>
class Vec3 : public Shape3<T>
{
public:
    union
    {
        struct {T x, y, z;};
        struct {T v[3];};
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
