template <class T>
RandomShapePicker<T>::RandomShapePicker(const Shape2<T>& shape) :
    m_shape(shape.clone()),
    m_randomEngine(nullptr),
    m_pointPickerPreprocessedData(shape.createPreprocessedDataForRandomPointPicker())
{

}
template <class T>
RandomShapePicker<T>::RandomShapePicker(const Shape2<T>& shape, const Random::RandomEngineBase& randomEngine) :
    m_shape(shape.clone()),
    m_randomEngine(randomEngine.clone()),
    m_pointPickerPreprocessedData(shape.createPreprocessedDataForRandomPointPicker())
{

}
template <class T>
void RandomShapePicker<T>::setRandomEngine(const Random::RandomEngineBase& randomEngine)
{
    m_randomEngine = randomEngine.clone();
}
template <class T>
void RandomShapePicker<T>::updatePreprocessedData()
{
    m_pointPickerPreprocessedData = m_shape->createPreprocessedDataForRandomPointPicker();
}

template <class T>
Vec2<T> RandomShapePicker<T>::pickRandomPoint() //m_randomEngine must be set for this
{
    return m_shape->pickRandomPoint(*m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
Vec2<T> RandomShapePicker<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomPoint(randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<Vec2<T>> RandomShapePicker<T>::pickRandomPoints(size_t quantity)
{
    return m_shape->pickRandomPoints(quantity, *m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<Vec2<T>> RandomShapePicker<T>::pickRandomPoints(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomPoints(quantity, randomEngine, *m_pointPickerPreprocessedData);
}

template <class T>
Triangle<T> RandomShapePicker<T>::pickRandomTriangle() //m_randomEngine must be set for this
{
    return m_shape->pickRandomTriangle(*m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
Triangle<T> RandomShapePicker<T>::pickRandomTriangle(Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomTriangle(randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<Triangle<T>> RandomShapePicker<T>::pickRandomTriangles(size_t quantity)
{
    return m_shape->pickRandomTriangles(quantity, *m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<Triangle<T>> RandomShapePicker<T>::pickRandomTriangles(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomTriangles(quantity, randomEngine, *m_pointPickerPreprocessedData);
}

template <class T>
LineSegment<T> RandomShapePicker<T>::pickRandomLineSegment() //m_randomEngine must be set for this
{
    return m_shape->pickRandomLineSegment(*m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
LineSegment<T> RandomShapePicker<T>::pickRandomLineSegment(Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomLineSegment(randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<LineSegment<T>> RandomShapePicker<T>::pickRandomLineSegments(size_t quantity)
{
    return m_shape->pickRandomLineSegments(quantity, *m_randomEngine, *m_pointPickerPreprocessedData);
}
template <class T>
std::vector<LineSegment<T>> RandomShapePicker<T>::pickRandomLineSegments(size_t quantity, Random::RandomEngineBase& randomEngine) const
{
    return m_shape->pickRandomLineSegments(quantity, randomEngine, *m_pointPickerPreprocessedData);
}
