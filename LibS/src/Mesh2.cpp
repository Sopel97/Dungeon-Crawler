template <class ShapeType>
Mesh2<ShapeType>::Mesh2()
{

}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(const std::vector<ShapeType>& e) : elements(e)
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(std::vector<ShapeType>&& e) : elements(std::move(e))
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(const std::initializer_list<ShapeType>& list) : elements(list)
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(ShapeType* e, size_t count)
{
    elements.assign(e, e + count);
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(const Mesh2<ShapeType>& m) : elements(m.elements)
{
}
template <class ShapeType>
Mesh2<ShapeType>::Mesh2(Mesh2<ShapeType>&& m) : elements(std::move(m.elements))
{
}

template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator=(const Mesh2<ShapeType>& m)
{
    elements = m.elements;
    return *this;
}
template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator=(Mesh2<ShapeType> && m)
{
    elements = std::move(m.elements);
    return *this;
}

template <class ShapeType>
Mesh2<ShapeType> Mesh2<ShapeType>::operator+(const Vec2<T>& v) const
{
    Mesh2<ShapeType> m(*this);
    m += v;
    return m;
}
template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator+=(const Vec2<T>& v)
{
    int size = elements.size();
    for(T& element : elements)
    {
        element += v;
    }
    return *this;
}
template <class ShapeType>
Mesh2<ShapeType> Mesh2<ShapeType>::operator-(const Vec2<T>& v) const
{
    Mesh2<ShapeType> m(*this);
    m -= v;
    return m;
}
template <class ShapeType>
Mesh2<ShapeType>& Mesh2<ShapeType>::operator-=(const Vec2<T>& v)
{
    int size = elements.size();
    for(T& element : elements)
    {
        element -= v;
    }
    return *this;
}

template <class ShapeType>
void Mesh2<ShapeType>::add(const ShapeType& e)
{
    elements.push_back(e);
}
template <class ShapeType>
void Mesh2<ShapeType>::add(const std::vector<ShapeType>& e)
{
    elements.insert(elements.end(), e.begin(), e.end());
}
template <class ShapeType>
void Mesh2<ShapeType>::add(const Mesh2<ShapeType>& m)
{
    add(m.elements);
}
template <class ShapeType>
void Mesh2<ShapeType>::add(const std::initializer_list<ShapeType>& list)
{
    elements.insert(elements.end(), list.begin, list.end());
}

template <class ShapeType>
void Mesh2<ShapeType>::add(ShapeType&& e)
{
    elements.push_back(e);
}
template <class ShapeType>
void Mesh2<ShapeType>::add(std::vector<ShapeType>&& e)
{
    elements.insert(elements.end(), e.begin(), e.end());
}
template <class ShapeType>
void Mesh2<ShapeType>::add(Mesh2<ShapeType>&& m)
{
    add(std::move(m.elements));
}

template <class ShapeType>
typename Mesh2<ShapeType>::T Mesh2<ShapeType>::distanceTo(const Vec2<T>& point) const
{
    if(point.intersects(*this)) return 0;

    T minDistance = std::numeric_limits<T>::max();
    for(const auto& shape : elements)
    {
        minDistance = std::min(minDistance, shape.distanceTo(point));
    }
    return minDistance;
}
template <class ShapeType>
Vec2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::nearestPointTo(const Vec2<T>& point) const
{
    if(point.intersects(*this)) return point;

    T minDistance = std::numeric_limits<T>::max();
    Vec2<T> nearestPoint(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    for(const auto& shape : elements)
    {
        Vec2<T> nearPoint = shape.nearestPointTo(point);
        T distance = nearPoint.distanceTo(point);
        if(distance < minDistance)
        {
            minDistance = distance;
            nearestPoint = nearPoint;
        }
    }
    return nearestPoint;
}


template <class ShapeType>
size_t Mesh2<ShapeType>::size() const
{
    return elements.size();
}
/*
template <class ShapeType>
std::unique_ptr<typename Shape2<typename Mesh2<ShapeType>::T>::RandomPointPickerPreprocessedData> Mesh2<ShapeType>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>(*this);
}
template <class ShapeType>
Vec2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    return pickRandomPoint(randomEngine, *createPreprocessedDataForRandomPointPicker());
}
template <class ShapeType>
Vec2<typename Mesh2<ShapeType>::T> Mesh2<ShapeType>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    Mesh2<ShapeType>::RandomPointPickerPreprocessedData& polygonPreprocessedData = static_cast<Mesh2<ShapeType>::RandomPointPickerPreprocessedData&>(preprocessedData);
    T sumOfAreas = polygonPreprocessedData.shapesByArea.back().second;
    T randomArea = randomEngine.next<T>(T(0.0), sumOfAreas);
    auto chosenShapeIter = std::upper_bound(polygonPreprocessedData.shapesByArea.begin(), polygonPreprocessedData.shapesByArea.end(), randomArea, [](const T& lhs, const std::pair<const ShapeType*, T>& rhs)->bool{return lhs < rhs.second;});
    if(chosenShapeIter == polygonPreprocessedData.shapesByArea.end()) chosenShapeIter = polygonPreprocessedData.shapesByArea.begin();
    return chosenShapeIter->first->pickRandomPoint(randomEngine);
}

*/
