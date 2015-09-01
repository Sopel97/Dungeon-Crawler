#ifndef POLYLINE_H
#define POLYLINE_H

template <class T>
class Polyline : public Shape2<T>
{
public:
    std::vector<Vec2<T>> vertices;

    Polyline() {}
    Polyline(const std::initializer_list<Vec2<T>>& list);
    Polyline(const std::vector<Vec2<T>>& v);
    Polyline(std::vector<Vec2<T>>&& v);
    Polyline(Vec2<T>* v, size_t count);

    static Polyline<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polyline<T> fromTriangle(const Triangle<T>& triangle);

    Polyline(const Polyline<T>& other) { vertices = other.vertices; }
    Polyline(Polyline<T>&& other) { vertices = std::move(other.vertices); }

    virtual ~Polyline() {}

    Polyline<T>& operator=(const Polyline<T>& other) { vertices = other.vertices; return *this; }
    Polyline<T>& operator=(Polyline<T> && other) { vertices = std::move(other.vertices); return *this; }

    Polyline<T> operator+(const Vec2<T>& v) const;
    Polyline<T> operator-(const Vec2<T>& v) const;

    Polyline<T>& operator+=(const Vec2<T>& v);
    Polyline<T>& operator-=(const Vec2<T>& v);

    template <class T2>
    explicit operator Polyline<T2>() const;

    void add(const Vec2<T>& v);
    void add(const std::vector<T>& v);
    void add(const std::initializer_list<Vec2<T>>& list);

    void add(Vec2<T>&& v);
    void add(std::vector<T>&& v);

    std::pair<T, T> projectMinMax(const Vec2<T>& b) const; //this is used in SAT, but it does not belong here so should be removed

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    size_t size() const;
};

typedef Polyline<double> PolylineD;
typedef Polyline<float> PolylineF;
typedef Polyline<int> PolylineI;

extern template class Polyline<double>;
extern template class Polyline<float>;
extern template class Polyline<int>;

#include "../src/Polyline.cpp"

#endif // POLYLINE_H
