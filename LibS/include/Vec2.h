#ifndef Vec2_H_INCLUDED
#define Vec2_H_INCLUDED

template <class T>
class Vec2 : public Shape2<T>
{
public:

    union
    {
        struct {T x, y;};
        struct {T v[2];};
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

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& c, const Vec2<T>& s);
    void scale(const Vec2<T>& c, const T s);
    void scale(const Vec2<T>& s);
    void scale(const T s);

    T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.

};
template <class T>
const Vec2<T> Vec2<T>::unitX = Vec2<T> {1, 0};
template <class T>
const Vec2<T> Vec2<T>::unitY = Vec2<T> {0, 1};

typedef Vec2<double> Vec2D;
typedef Vec2<float> Vec2F;
typedef Vec2<int> Vec2I;

extern template class Vec2<double>;
extern template class Vec2<float>;
extern template class Vec2<int>;

#include "../src/Vec2.cpp"
#endif // Vec2_H_INCLUDED
