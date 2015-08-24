#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

template <class T>
class Polygon : public Shape2<T> //Supports convex and concave polygons. Does not support self intersecting (complex) polygons (some methods may work, some may not).
{
public:
    /*
    struct RandomPointPickerPreprocessedData : public Shape2<T>::RandomPointPickerPreprocessedData
    {
        RandomPointPickerPreprocessedData(PolygonTriangulation<T>&& triang) :
            triangulation(std::move(triang))
        {
            trianglesByArea.reserve(triangulation.triangleMesh().size());
            for(auto& triangle : triangulation.triangleMesh().elements)
            {
                trianglesByArea.push_back(std::make_pair(&triangle, triangle.area()));
            }
            size_t numberOfTriangles = trianglesByArea.size();
            for(size_t i = 1; i < numberOfTriangles; ++i)
            {
                trianglesByArea[i].second += trianglesByArea[i-1].second; //so the area is cumulative
            }
        }
        PolygonTriangulation<T> triangulation;
        std::vector<std::pair<const Triangle<T>*, T>> trianglesByArea; //<triangle ptr to triangulation result, CUMULATIVE area> - cumulative area so binary searching is possible without sorting
    };
*/
    std::vector<Vec2<T>> vertices;

    Polygon() = default;
    Polygon(const std::initializer_list<Vec2<T>>& list);
    Polygon(const std::vector<Vec2<T>>& v);
    Polygon(std::vector<Vec2<T>>&& v);
    Polygon(Vec2<T>* v, size_t count);

    //static Polygon<T> randomInscribedInCircle(const Circle<T>& circle, int edges); later, when using Random.h will be somehow standarized
    static Polygon<T> regular(const Vec2D& center, int sides, int radius);
    static Polygon<T> fromRectangle(const Rectangle<T>& rectangle);
    static Polygon<T> fromTriangle(const Triangle<T>& triangle);

    Polygon(const Polygon<T>&) = default;
    template <class X>
    Polygon(const Polygon<X>& p);
    Polygon(Polygon<T>&&) = default;

    virtual ~Polygon(){}

    Polygon<T>& operator=(const Polygon<T>&) = default;
    template <class X>
    Polygon<T>& operator=(const Polygon<X>& p);
    Polygon<T>& operator=(Polygon<T> &&) = default;

    Polygon<T> operator+(const Vec2<T>& v) const;
    Polygon<T> operator-(const Vec2<T>& v) const;

    Polygon<T>& operator+=(const Vec2<T>& v);
    Polygon<T>& operator-=(const Vec2<T>& v);

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

    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.

    Polyline<T> asPolyline() const;

    Vec2<T> center() const;
    bool isConvex() const;
    T signedArea() const;

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
