#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

template <class T>
class Triangle : public Shape2<T>
{
public:

    Vec2<T> vertices[3];

    Triangle() {}
    Triangle(const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3);
    Triangle(const Vec2<T>* p);

    static Triangle<T> equilateral(const Vec2<T>& center, const T base);
    static Triangle<T> isosceles(const Vec2<T>& center, const T base, const T height);
    static Triangle<T> rightTriangle(const Vec2<T>& rightAngledVertex, const T width, const T height); //with 90 degree angle on the left

    Triangle(const Triangle<T>& other){vertices[0] = other.vertices[0];vertices[1] = other.vertices[1];vertices[2] = other.vertices[2];}
    Triangle(Triangle<T>&& other){vertices[0] = std::move(other.vertices[0]);vertices[1] = std::move(other.vertices[1]);vertices[2] = std::move(other.vertices[2]);}

    virtual ~Triangle(){}

    Triangle<T>& operator =(const Triangle<T>& other){vertices[0] = other.vertices[0];vertices[1] = other.vertices[1];vertices[2] = other.vertices[2]; return *this;}
    Triangle<T>& operator =(Triangle<T> && other){vertices[0] = std::move(other.vertices[0]);vertices[1] = std::move(other.vertices[1]);vertices[2] = std::move(other.vertices[2]); return *this;}

    Triangle<T> operator +(const Vec2<T>& p) const;
    Triangle<T> operator -(const Vec2<T>& p) const;

    Triangle<T>& operator +=(const Vec2<T>& p);
    Triangle<T>& operator -=(const Vec2<T>& p);

    template <class T2>
    explicit operator Triangle<T2>() const;

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    Vec2<T> centerOfMass() const;
    T signedArea() const;
    T area() const;
};

typedef Triangle<double> TriangleD;
typedef Triangle<float> TriangleF;
typedef Triangle<int> TriangleI;

extern template class Triangle<double>;
extern template class Triangle<float>;
extern template class Triangle<int>;

#include "../src/Triangle.cpp"
#endif // TRIANGLE_H_INCLUDED
