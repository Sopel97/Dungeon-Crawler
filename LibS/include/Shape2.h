#ifndef Shape2_H_INCLUDED
#define Shape2_H_INCLUDED

/*  Available 2D shapes

    Circle
    LineSegment
    Polygon
    Polyline
    Ray
    Rectangle
    Triangle
    Vec2

*/

/*

    Be aware that some methods don't make sense with some shapes (this usually follows the mathematical sense) as well as not every shape can be used, for example, for defining some space.
    As this gives more flexibility and makes things easier it requires the user to be aware of this.
    Example:
        in class RapidlyExploringRandomTree using LineSegment as a space (even though it has a method for picking points) makes no sense.
        Method for computing center or picking a random point is not defined for ray

    Eventually all operations that make sense should be implemented.
    Operations that make no mathematical sense (or are undefined) have meaningless (and not defined) results.

*/
template <class T>
class Shape2 : public Shape<T>
{
public:
    struct RandomPointPickerPreprocessedData //Stores data for pickRandomPoint function. Sending it through parameter will save some time (depending on shape) for preprocessing. Every shape has it's own struct deriving from this one.
    {

    };

    using VectorType = Vec2<T>;

    Shape2() = default;

    Shape2(const Shape2&) = default;
    Shape2(Shape2&&) = default;

    virtual ~Shape2(){}

    Shape2<T>& operator = (const Shape2&) = default;
    Shape2<T>& operator = (Shape2 &&) = default;

    virtual Polyline<T> asPolyline() const;

    virtual void translate(const Vec2<T>& v) = 0;
    virtual void scale(const Vec2<T>& c, const Vec2<T>& s);
    virtual void scale(const Vec2<T>& c, const T s);
    virtual void scale(const Vec2<T>& s) = 0;
    virtual void scale(const T s);

    virtual Vec2<T> project(const Vec2<T>& b) const;
    virtual std::pair<T, T> projectMinMax(const Vec2<T>& b) const; //min and max values after projecting onto line

    virtual T distanceTo(const Vec2<T>& v1) const = 0;   //returns 0 if the point is inside the shape
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const = 0;   //returns point when points is inside the shape

    //transformations work pointwise for shape defining points. So a circle won't change into ellipse, rectangle won't rotate and such. Though it could change size even though it's not determined by any vector.
    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction) = 0;
    virtual void transform(const Transformation2<T>& transformation) = 0;

    virtual std::unique_ptr<RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual std::vector<Vec2<T>> pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const;
    virtual std::vector<Vec2<T>> pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.

    //TODO: Define following two types of methods for shapes that may be concave or have holes/gaps. Probably using randomized algorithm.
    virtual Triangle<T> pickRandomTriangle(Random::RandomEngineBase& randomEngine) const; //standard way requires the shape to be convex
    virtual Triangle<T> pickRandomTriangle(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual std::vector<Triangle<T>> pickRandomTriangles(size_t quantity, Random::RandomEngineBase& randomEngine) const;
    virtual std::vector<Triangle<T>> pickRandomTriangles(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.

    virtual LineSegment<T> pickRandomLineSegment(Random::RandomEngineBase& randomEngine) const; //standard way requires the shape to be convex
    virtual LineSegment<T> pickRandomLineSegment(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual std::vector<LineSegment<T>> pickRandomLineSegments(size_t quantity, Random::RandomEngineBase& randomEngine) const;
    virtual std::vector<LineSegment<T>> pickRandomLineSegments(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.

    virtual Vec2<T> center() const;
    virtual T area() const;
    virtual T signedArea() const;
    virtual bool isConvex() const;

    SHAPE2_PURE_VIRTUAL_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const = 0;
};

typedef Shape2<double> Shape2D;
typedef Shape2<float> Shape2F;
typedef Shape2<int> Shape2I;

#include "../src/Shape2.cpp"
#endif // Shape2_H_INCLUDED
