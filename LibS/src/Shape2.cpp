template <class T>
Polyline<T> Shape2<T>::asPolyline() const
{
    return Polyline<T>();
}

template <class T>
Vec2<T> Shape2<T>::project(const Vec2<T>& b) const
{
    //not specified
    return Vec2<T>();
}

template <class T>
std::pair<T, T> Shape2<T>::projectMinMax(const Vec2<T>& b) const
{
    //not specified
    return std::make_pair(T(), T());
}
template <class T>
void Shape2<T>::scale(const T s)
{
    scale(Vec2<T>(s));
}
template <class T>
void Shape2<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{
    translate(-c);
    scale(s);
    translate(c);
}
template <class T>
void Shape2<T>::scale(const Vec2<T>& c, const T s)
{
    scale(c, Vec2<T>(s));
}
template <class T>
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Shape2<T>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>();
}
template <class T>
Vec2<T> Shape2<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return pickRandomPoint(randomEngine, *(createPreprocessedDataForRandomPointPicker()));
}
template <class T>
std::vector<Vec2<T>> Shape2<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return pickRandomPoints(quantity, randomEngine, *(createPreprocessedDataForRandomPointPicker()));
}
template <class T>
Vec2<T> Shape2<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return Vec2<T>();
}
template <class T>
std::vector<Vec2<T>> Shape2<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    std::vector<Vec2<T>> result;
    result.reserve(quantity);
    while(quantity--)
    {
        result.emplace_back(pickRandomPoint(randomEngine, preprocessedData));
    }
    return result;
}
template <class T>
Triangle<T> Shape2<T>::pickRandomTriangle(Random::RandomEngineBase& randomEngine) const
{
    return pickRandomTriangle(randomEngine, *(createPreprocessedDataForRandomPointPicker()));
}
template <class T>
Triangle<T> Shape2<T>::pickRandomTriangle(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return Triangle<T>(pickRandomPoint(randomEngine, preprocessedData), pickRandomPoint(randomEngine, preprocessedData), pickRandomPoint(randomEngine, preprocessedData));
}
template <class T>
std::vector<Triangle<T>> Shape2<T>::pickRandomTriangles(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return pickRandomTriangles(quantity, randomEngine, *(createPreprocessedDataForRandomPointPicker()));
}
template <class T>
std::vector<Triangle<T>> Shape2<T>::pickRandomTriangles(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    std::vector<Triangle<T>> result;
    result.reserve(quantity);
    while(quantity--)
    {
        result.emplace_back(pickRandomTriangle(randomEngine, preprocessedData));
    }
    return result;
}
template <class T>
LineSegment<T> Shape2<T>::pickRandomLineSegment(Random::RandomEngineBase& randomEngine) const
{
    return pickRandomLineSegment(randomEngine, *(createPreprocessedDataForRandomPointPicker()));
}
template <class T>
LineSegment<T> Shape2<T>::pickRandomLineSegment(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return LineSegment<T>(pickRandomPoint(randomEngine, preprocessedData), pickRandomPoint(randomEngine, preprocessedData));
}
template <class T>
std::vector<LineSegment<T>> Shape2<T>::pickRandomLineSegments(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return pickRandomLineSegments(quantity, randomEngine, *(createPreprocessedDataForRandomPointPicker()));
}
template <class T>
std::vector<LineSegment<T>> Shape2<T>::pickRandomLineSegments(size_t quantity, Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    std::vector<LineSegment<T>> result;
    result.reserve(quantity);
    while(quantity--)
    {
        result.emplace_back(pickRandomLineSegment(randomEngine, preprocessedData));
    }
    return result;
}


template <class T>
Vec2<T> Shape2<T>::center() const
{
    return Vec2<T>();
}
template <class T>
T Shape2<T>::area() const
{
    return std::abs(signedArea());
}
template <class T>
T Shape2<T>::signedArea() const
{
    return 0.0;
}
template <class T>
bool Shape2<T>::isConvex() const
{
    return true; //almost all shapes are convex
}
