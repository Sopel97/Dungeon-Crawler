#ifndef POLYLINE_H
#define POLYLINE_H

template <class T>
class Polyline : public Shape2<T>
{
public:
    struct RandomPointPickerPreprocessedData : public Shape2<T>::RandomPointPickerPreprocessedData
    {
        //currently empty but will be used later
    };
    std::vector<Vec2<T>> vertices;

    Polyline() = default;
    Polyline(const std::initializer_list<Vec2<T>>& list);
    Polyline(const std::vector<Vec2<T>>& v);
    Polyline(std::vector<Vec2<T>>&& v);
    Polyline(Vec2<T>* v, size_t count);

    static Polyline<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polyline<T> fromTriangle(const Triangle<T>& triangle);

    Polyline(const Polyline<T>&) = default;
    template <class X>
    Polyline(const Polyline<X>& p);
    Polyline(Polyline<T>&&) = default;

    virtual ~Polyline(){}

    Polyline<T>& operator=(const Polyline<T>&) = default;
    template <class X>
    Polyline<T>& operator=(const Polyline<X>& p);
    Polyline<T>& operator=(Polyline<T> &&) = default;

    Polyline<T> operator+(const Vec2<T>& v) const;
    Polyline<T> operator-(const Vec2<T>& v) const;

    Polyline<T>& operator+=(const Vec2<T>& v);
    Polyline<T>& operator-=(const Vec2<T>& v);

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

    //std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;

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
