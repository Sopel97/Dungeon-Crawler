#ifndef CUBOID_H_INCLUDED
#define CUBOID_H_INCLUDED

template <class T>
class Cuboid : public Shape<T>
{
public:
    Vec3<T> min, max;

    Cuboid() = default;
    Cuboid(const Vec3<T>& p1, const Vec3<T>& p2);
    Cuboid(const Vec3<T>& p1, const T width, const T height, const T depth);

    virtual ~Cuboid() {}

    Cuboid(const Cuboid<T>& c) { min = c.min; max = c.max; }
    Cuboid(Cuboid<T>&& c) { min = std::move(c.min); max = std::move(c.max); }

    Cuboid<T>& operator =(const Cuboid<T>& c) { min = c.min; max = c.max; return *this; }
    Cuboid<T>& operator =(Cuboid<T> && c) { min = c.min; max = c.max; return *this; }

    Cuboid<T> operator +(const Vec3<T>& v) const;
    Cuboid<T> operator -(const Vec3<T>& v) const;

    Cuboid<T>& operator +=(const Vec3<T>& v);
    Cuboid<T>& operator -=(const Vec3<T>& v);

    template <class T2>
    explicit operator Cuboid<T2>() const;

    T width() const;
    T height() const;
    T depth() const;

    /* INTERSECTIONS */
    template<class S>
    bool intersects(const S& b) const;
};

typedef Cuboid<double> CuboidD;
typedef Cuboid<float> CuboidF;
typedef Cuboid<int> CuboidI;

extern template class Cuboid<double>;
extern template class Cuboid<float>;
extern template class Cuboid<int>;

#include "../src/Cuboid.cpp"

#endif // CUBOID_H_INCLUDED
