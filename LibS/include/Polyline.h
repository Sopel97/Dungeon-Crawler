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

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Polyline<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Polyline<T> transformed(const Transformation2<T>& transformation) const;

    Vec2<T> project(const Vec2<T>& b) const;
    std::pair<T, T> projectMinMax(const Vec2<T>& b) const;

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    virtual std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;

    Polyline<T> asPolyline() const;

    size_t size() const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef Polyline<double> PolylineD;
typedef Polyline<float> PolylineF;
typedef Polyline<int> PolylineI;

#include "../src/Polyline.cpp"

#endif // POLYLINE_H
