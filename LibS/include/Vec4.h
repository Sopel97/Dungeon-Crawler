#ifndef VEC4_H
#define VEC4_H

template <class T>
class Vec4 : public Shape4<T>
{
public:
    union
    {
        struct {T x, y, z, w;};
        struct {T v[4];};
    };

    static const Vec4<T> unitX;
    static const Vec4<T> unitY;
    static const Vec4<T> unitZ;
    static const Vec4<T> unitW;

    Vec4(){}
    Vec4(T _x, T _y, T _z, T _w);
    Vec4(const std::initializer_list<T>& list);

    virtual ~Vec4(){}

    Vec4(const Vec4<T>& v){x = v.x;y = v.y;z = v.z;w = v.w;}
    Vec4(Vec4<T>&& v){x = std::move(v.x);y = std::move(v.y);z = std::move(v.z);w = std::move(v.w);}

    Vec4<T>& operator=(const Vec4<T>& v1){x = v1.x;y = v1.y;z = v1.z;w = v1.w; return *this;}
    Vec4<T>& operator=(Vec4<T> && v1){x = std::move(v1.x);y = std::move(v1.y);z = std::move(v1.z);w = std::move(v1.w); return *this;}

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

    template <class T2>
    explicit operator Vec4<T2>() const;

    T magnitude();
    T distance(const Vec4<T>& v);
    void normalize();
    Vec4<T> normalized();

    void fill(T value);

    void translate(const Vec4<T>& v) {};
    void scale(const Vec4<T>& c, const Vec4<T>& s) {};
    void scale(const Vec4<T>& c, const T s) {};
    void scale(const Vec4<T>& s) {};
    void scale(const T s) {};

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

extern template class Vec4<double>;
extern template class Vec4<float>;
extern template class Vec4<int>;

#include "../src/Vec4.cpp"

#endif // VEC4_H
