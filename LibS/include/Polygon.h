#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

template <class T>
class Polygon : public Shape2<T> //Supports convex and concave polygons. Does not support self intersecting (complex) polygons (some methods may work, some may not).
{
public:
    std::vector<Vec2<T>> vertices;

    Polygon(){}
    Polygon(const std::initializer_list<Vec2<T>>& list);
    Polygon(const std::vector<Vec2<T>>& v);
    Polygon(std::vector<Vec2<T>>&& v);
    Polygon(Vec2<T>* v, size_t count);

    //static Polygon<T> randomInscribedInCircle(const Circle<T>& circle, int edges); later, when using Random.h will be somehow standarized
    static Polygon<T> regular(const Vec2D& center, int sides, int radius);
    static Polygon<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polygon<T> fromTriangle(const Triangle<T>& triangle);

    Polygon(const Polygon<T>& other){vertices = other.vertices;}
    Polygon(Polygon<T>&& other){vertices = std::move(other.vertices);}

    virtual ~Polygon(){}

    Polygon<T>& operator=(const Polygon<T>& other){vertices = other.vertices; return *this;}
    Polygon<T>& operator=(Polygon<T> && other){vertices = std::move(other.vertices); return *this;}

    Polygon<T> operator+(const Vec2<T>& v) const;
    Polygon<T> operator-(const Vec2<T>& v) const;

    Polygon<T>& operator+=(const Vec2<T>& v);
    Polygon<T>& operator-=(const Vec2<T>& v);

    template <class T2>
    explicit operator Polygon<T2>() const;

    void add(const Vec2<T>& v);
    void add(const std::vector<T>& v);
    void add(const std::initializer_list<Vec2<T>>& list);

    void add(Vec2<T>&& v);
    void add(std::vector<T>&& v);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    Vec2<T> project(const Vec2<T>& b) const;
    std::pair<T, T> projectMinMax(const Vec2<T>& b) const; //this is used in SAT, but it does not belong here so should be removed

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    Vec2<T> centerOfMass() const;
    bool isConvex() const;
    T signedArea() const;
    T area() const;

    size_t size() const;
};

typedef Polygon<double> PolygonD;
typedef Polygon<float> PolygonF;
typedef Polygon<int> PolygonI;

extern template class Polygon<double>;
extern template class Polygon<float>;
extern template class Polygon<int>;

#include "../src/Polygon.cpp"
#endif // POLYGON_H_INCLUDED
