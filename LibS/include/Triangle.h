#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

template <class T>
class Triangle : public Shape2<T>
{
public:
    struct RandomPointPickerPreprocessedData : public Shape2<T>::RandomPointPickerPreprocessedData
    {
        RandomPointPickerPreprocessedData(const Vec2<T>& e12, const Vec2<T>& e02, const Vec2<T>& e01, const Vec2<T>& e10) :
            edge12(e12),
            edge02(e02),
            edge01(e01),
            edge10(e10)
        {
        }
        Vec2<T> edge12;
        Vec2<T> edge02;
        Vec2<T> edge01;
        Vec2<T> edge10;
    };

    Vec2<T> vertices[3];

    Triangle() = default;
    Triangle(const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3);
    Triangle(const Vec2<T>* p);

    static Triangle<T> equilateral(const Vec2D& center, const T base);
    static Triangle<T> isosceles(const Vec2D& center, const T base, const T height);
    static Triangle<T> rightTriangle(const Vec2D& rightAngledVertex, const T width, const T height); //with 90 degree angle on the left

    Triangle(const Triangle<T>&) = default;
    template <class X>
    Triangle(const Triangle<X>& t);
    Triangle(Triangle<T>&&) = default;

    virtual ~Triangle(){}

    Triangle<T>& operator =(const Triangle<T>&) = default;
    template <class X>
    Triangle<T>& operator =(const Triangle<X>& t);
    Triangle<T>& operator =(Triangle<T> &&) = default;

    Triangle<T> operator +(const Vec2<T>& p) const;
    Triangle<T> operator -(const Vec2<T>& p) const;

    Triangle<T>& operator +=(const Vec2<T>& p);
    Triangle<T>& operator -=(const Vec2<T>& p);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    //std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    Vec2<T> center() const;
    T signedArea() const;
};

typedef Triangle<double> TriangleD;
typedef Triangle<float> TriangleF;
typedef Triangle<int> TriangleI;

extern template class Triangle<double>;
extern template class Triangle<float>;
extern template class Triangle<int>;

#include "../src/Triangle.cpp"
#endif // TRIANGLE_H_INCLUDED
