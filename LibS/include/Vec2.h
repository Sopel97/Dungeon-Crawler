#ifndef Vec2_H_INCLUDED
#define Vec2_H_INCLUDED

template <class T>
class Vec2;

//TODO: consider making operator always return values of type of the first operand
template <class T, size_t X, size_t Y>
class Vec2Proxy : public Shape2<T>
{
public:
    static constexpr bool isWritable = ___internal::are_unique<size_t, X, Y>::value;

    virtual ~Vec2Proxy(){}

    template <class U>
    Vec2Proxy<T, X, Y>& operator=(const Vec2<U>& v1);

    Vec2Proxy<T, X, Y>& operator=(const Vec2<T>& v1);
    Vec2Proxy<T, X, Y>& operator=(Vec2<T> && v1);

    operator Vec2<T>() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator+(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator-(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator*(const TSecond scalar) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator*(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator/(const TSecond scalar) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator/(const Vec2<TSecond>& v1) const;

    Vec2<T> operator-() const;

    Vec2Proxy<T, X, Y>& operator+=(const Vec2<T>& v1);
    Vec2Proxy<T, X, Y>& operator-=(const Vec2<T>& v1);
    Vec2Proxy<T, X, Y>& operator*=(const T scalar);
    Vec2Proxy<T, X, Y>& operator*=(const Vec2<T>& v1);
    Vec2Proxy<T, X, Y>& operator/=(const T scalar);
    Vec2Proxy<T, X, Y>& operator/=(const Vec2<T>& v1);

    T magnitude() const;
    T quadrance() const;
    T distanceTo(const LineSegment<T>& lineSegment) const;
    void normalize();
    Vec2<T> normalized() const;
    Vec2<T> normalLeft() const;
    Vec2<T> normalRight() const;
    Vec2<T> normal() const; //one of above two. Should be used only when it makes no difference which one is used.
    T dot(const Vec2<T>& b) const;
    T cross(const Vec2<T>& b) const;
    Vec2<T> project(const Vec2<T>& b) const; //TODO: change name to projected
    Angle<T> angle() const;
    Angle<T> angle(const Vec2<T>& other) const;

    void fill(T value);

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& c, const Vec2<T>& s);
    virtual void scale(const Vec2<T>& c, const T s);
    virtual void scale(const Vec2<T>& s);
    virtual void scale(const T s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Vec2<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Vec2<T> transformed(const Transformation2<T>& transformation) const;

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const;

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;

    T& getX();
    const T& getX() const;
    T& getY();
    const T& getY() const;
};

#include "Vec3.h"
#include "Vec4.h"

template <class T>
class Vec2 : public Shape2<T>
{
public:

    union
    {
        struct {T x, y;};
        struct {T v[2];};

        Vec2Proxy<T, 0, 0> xx;
        Vec2Proxy<T, 0, 1> xy;
        Vec2Proxy<T, 1, 0> yx;
        Vec2Proxy<T, 1, 1> yy;

        Vec3Proxy<T, 0, 0, 0> xxx;
        Vec3Proxy<T, 0, 0, 1> xxy;
        Vec3Proxy<T, 0, 1, 0> xyx;
        Vec3Proxy<T, 0, 1, 1> xyy;
        Vec3Proxy<T, 1, 0, 0> yxx;
        Vec3Proxy<T, 1, 0, 1> yxy;
        Vec3Proxy<T, 1, 1, 0> yyx;
        Vec3Proxy<T, 1, 1, 1> yyy;

        Vec4Proxy<T, 0, 0, 0, 0> xxxx;
        Vec4Proxy<T, 0, 0, 0, 1> xxxy;
        Vec4Proxy<T, 0, 0, 1, 0> xxyx;
        Vec4Proxy<T, 0, 0, 1, 1> xxyy;
        Vec4Proxy<T, 0, 1, 0, 0> xyxx;
        Vec4Proxy<T, 0, 1, 0, 1> xyxy;
        Vec4Proxy<T, 0, 1, 1, 0> xyyx;
        Vec4Proxy<T, 0, 1, 1, 1> xyyy;
        Vec4Proxy<T, 1, 0, 0, 0> yxxx;
        Vec4Proxy<T, 1, 0, 0, 1> yxxy;
        Vec4Proxy<T, 1, 0, 1, 0> yxyx;
        Vec4Proxy<T, 1, 0, 1, 1> yxyy;
        Vec4Proxy<T, 1, 1, 0, 0> yyxx;
        Vec4Proxy<T, 1, 1, 0, 1> yyxy;
        Vec4Proxy<T, 1, 1, 1, 0> yyyx;
        Vec4Proxy<T, 1, 1, 1, 1> yyyy;
    };

    static const Vec2<T> unitX;
    static const Vec2<T> unitY;

    Vec2();
    Vec2(T _xy);
    Vec2(T _x, T _y);
    Vec2(const std::initializer_list<T>& list);

    static Vec2<T> direction(const Angle<T>& angle);

    Vec2(const Vec2<T>& v);
    template <class X>
    Vec2(const Vec2<X>& v);
    Vec2(Vec2<T>&& v);

    Vec2<T>& operator=(const Vec2<T>& v1);
    template <class X>
    Vec2<T>& operator=(const Vec2<X>& v1);
    Vec2<T>& operator=(Vec2<T> && v1);

    virtual ~Vec2(){}

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator+(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator-(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator*(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator/(const Vec2<TSecond>& v1) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator*(const TSecond scalar) const;
    template <class TSecond>
    Vec2<typename std::common_type<T, TSecond>::type> operator/(const TSecond scalar) const;

    Vec2<T> operator-() const;

    Vec2<T>& operator+=(const Vec2<T>& v1);
    Vec2<T>& operator-=(const Vec2<T>& v1);
    Vec2<T>& operator*=(const T scalar);
    Vec2<T>& operator*=(const Vec2<T>& v1);
    Vec2<T>& operator/=(const T scalar);
    Vec2<T>& operator/=(const Vec2<T>& v1);

    T magnitude() const;
    T quadrance() const;
    T distanceTo(const LineSegment<T>& lineSegment) const;
    void normalize();
    Vec2<T> normalized() const;
    Vec2<T> normalLeft() const;
    Vec2<T> normalRight() const;
    Vec2<T> normal() const; //one of above two. Should be used only when it makes no difference which one is used.
    T dot(const Vec2<T>& b) const;
    T cross(const Vec2<T>& b) const;
    Vec2<T> project(const Vec2<T>& b) const;
    Angle<T> angle() const;
    Angle<T> angle(const Vec2<T>& other) const;

    void fill(T value);

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& c, const Vec2<T>& s);
    virtual void scale(const Vec2<T>& c, const T s);
    virtual void scale(const Vec2<T>& s);
    virtual void scale(const T s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Vec2<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Vec2<T> transformed(const Transformation2<T>& transformation) const;

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const;

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;
protected:
};
/* think of a way to make it so TFirst cannot be a vec2 type or remove these functions at all
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator+(TFirst lhs, const Vec2<TSecond>& rhs);
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator-(TFirst lhs, const Vec2<TSecond>& rhs);
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator*(TFirst lhs, const Vec2<TSecond>& rhs);
template <class TFirst, class TSecond>
Vec2<typename std::common_type<TFirst, TSecond>::type> operator/(TFirst lhs, const Vec2<TSecond>& rhs);

template <class T, size_t X, size_t Y>
Vec2<T> operator+(T lhs, const Vec2Proxy<T, X, Y>& rhs);
template <class T, size_t X, size_t Y>
Vec2<T> operator-(T lhs, const Vec2Proxy<T, X, Y>& rhs);
template <class T, size_t X, size_t Y>
Vec2<T> operator*(T lhs, const Vec2Proxy<T, X, Y>& rhs);
template <class T, size_t X, size_t Y>
Vec2<T> operator/(T lhs, const Vec2Proxy<T, X, Y>& rhs);
//*/
template <class T>
const Vec2<T> Vec2<T>::unitX = Vec2<T> {1, 0};
template <class T>
const Vec2<T> Vec2<T>::unitY = Vec2<T> {0, 1};

typedef Vec2<double> Vec2D;
typedef Vec2<float> Vec2F;
typedef Vec2<int> Vec2I;

#include "../src/Vec2.cpp"
#endif // Vec2_H_INCLUDED
