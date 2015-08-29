#ifndef CYLINDER_H_INCLUDED
#define CYLINDER_H_INCLUDED

template <class T>
class Cylinder : public Shape<T>
{
public:
    Vec3<T> center; //cylinder base center
    T radius;
    T height;

    Cylinder() = default;
    Cylinder(const Vec3<T>& p1, T r, T h);

    virtual ~Cylinder() {}

    Cylinder(const Cylinder<T>& c) { center = c.center; radius = c.radius; height = c.height; }
    Cylinder(Cylinder<T>&& c) { center = std::move(c.center); radius = std::move(c.radius); height = std::move(c.height); }

    Cylinder<T>& operator =(const Cylinder<T>& c) { center = c.center; radius = c.radius; height = c.height; return *this; }
    Cylinder<T>& operator =(Cylinder<T> && c) { center = std::move(c.center); radius = std::move(c.radius); height = std::move(c.height); return *this; }

    Cylinder<T> operator +(const Vec3<T>& v) const;
    Cylinder<T> operator -(const Vec3<T>& v) const;

    Cylinder<T>& operator +=(const Vec3<T>& v);
    Cylinder<T>& operator -=(const Vec3<T>& v);

    template <class T2>
    explicit operator Cylinder<T2>() const;

    Circle<T> base() const;

    template <class Transformation>
    void transform(Transformation&& func);

    /* INTERSECTIONS */
    template<class S>
    bool intersects(const S& b) const;
};

typedef Cylinder<double> CylinderD;
typedef Cylinder<float> CylinderF;
typedef Cylinder<int> CylinderI;

extern template class Cylinder<double>;
extern template class Cylinder<float>;
extern template class Cylinder<int>;

#include "../src/Cylinder.cpp"

#endif // CYLINDER_H_INCLUDED
