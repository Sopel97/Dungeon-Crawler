#ifndef RANDOMSHAPEPICKER_H
#define RANDOMSHAPEPICKER_H

template <class T>
class RandomShapePicker
{
public:
    RandomShapePicker(const Shape2<T>& shape);
    RandomShapePicker(const Shape2<T>& shape, const Random::RandomEngineBase& randomEngine); //it has it's own random engine so it is cloned

    void setRandomEngine(const Random::RandomEngineBase& randomEngine);
    void updatePreprocessedData();

    Vec2<T> pickRandomPoint();
    Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    std::vector<Vec2<T>> pickRandomPoints(size_t quantity);
    std::vector<Vec2<T>> pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const;

    Triangle<T> pickRandomTriangle();
    Triangle<T> pickRandomTriangle(Random::RandomEngineBase& randomEngine) const;
    std::vector<Triangle<T>> pickRandomTriangles(size_t quantity);
    std::vector<Triangle<T>> pickRandomTriangles(size_t quantity, Random::RandomEngineBase& randomEngine) const;

    LineSegment<T> pickRandomLineSegment();
    LineSegment<T> pickRandomLineSegment(Random::RandomEngineBase& randomEngine) const;
    std::vector<LineSegment<T>> pickRandomLineSegments(size_t quantity);
    std::vector<LineSegment<T>> pickRandomLineSegments(size_t quantity, Random::RandomEngineBase& randomEngine) const;

protected:
    std::unique_ptr<Shape2<T>> m_shape;
    std::unique_ptr<Random::RandomEngineBase> m_randomEngine; //may be nullptr. If it is, then it is required to be passed directly to generating method
    std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> m_pointPickerPreprocessedData;
};

typedef RandomShapePicker<double> RandomShapePickerD;
typedef RandomShapePicker<float> RandomShapePickerF;
typedef RandomShapePicker<int> RandomShapePickerI;

#include "../src/RandomShapePicker.cpp"

#endif // RANDOMSHAPEPICKER_H
