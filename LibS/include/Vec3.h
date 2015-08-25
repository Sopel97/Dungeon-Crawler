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

    Vec3(){}
    Vec3(T _x, T _y, T _z);
    Vec3(const std::initializer_list<T>& list);

    Vec3(const Vec3<T>& v){x = v.x; y = v.y; z = v.z;}
    Vec3(Vec3<T>&& v){x = std::move(v.x); y = std::move(v.y); z = std::move(v.z);}

    virtual ~Vec3(){}

    Vec3<T>& operator=(const Vec3<T>& v1){x = v1.x; y = v1.y; z = v1.z; return *this;}
    Vec3<T>& operator=(Vec3<T> && v1){x = std::move(v1.x); y = std::move(v1.y); z = std::move(v1.z); return *this;}

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

    template <class T2>
    explicit operator Vec3<T2>() const;

    T magnitude();
    T distance(const Vec3<T>& v);
    void normalize();
    Vec3<T> normalized();

    void fill(T value);

    void translate(const Vec3<T>& v) {};
    void scale(const Vec3<T>& c, const Vec3<T>& s) {};
    void scale(const Vec3<T>& c, const T s) {};
    void scale(const Vec3<T>& s) {};
    void scale(const T s) {};

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

extern template class Vec3<double>;
extern template class Vec3<float>;
extern template class Vec3<int>;

#include "../src/Vec3.cpp"
#endif // Vec3_H_INCLUDED
