template <class T>
LineSegment<T>::LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end) : begin(_begin), end(_end)
{
}

template <class T>
template <class X>
LineSegment<T>::LineSegment(const LineSegment<X>& s) : begin(s._begin), end(s._end)
{
}
template <class T>
template <class X>
LineSegment<T>& LineSegment<T>::operator=(const LineSegment<X>& s)
{
    begin = s.begin;
    end = s.end;
    return *this;
}

template <class T>
LineSegment<T> LineSegment<T>::operator+(const Vec2<T>& v) const
{
    return LineSegment<T>(begin + v, end + v);
}
template <class T>
LineSegment<T>& LineSegment<T>::operator+=(const Vec2<T>& v)
{
    begin += v;
    end += v;
    return *this;
}
template <class T>
LineSegment<T> LineSegment<T>::operator-(const Vec2<T>& v) const
{
    return LineSegment<T>(begin - v, end - v);
}
template <class T>
LineSegment<T>& LineSegment<T>::operator-=(const Vec2<T>& v)
{
    begin -= v;
    end -= v;
    return *this;
}

template <class T>
T LineSegment<T>::length() const
{
    return begin.distanceTo(end);
}

template <class T>
void LineSegment<T>::translate(const Vec2<T>& v)
{
    begin.translate(v);
    end.translate(v);
}
template <class T>
void LineSegment<T>::scale(const Vec2<T>& s)
{
    begin.scale(s);
    end.scale(s);
}
template <class T>
T LineSegment<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> LineSegment<T>::nearestPointTo(const Vec2<T>& point) const
{
    Vec2<T> AO = point - begin;
    Vec2<T> AB = end - begin;
    T t = AO.dot(AB) / AB.quadrance();
    t = clamp(t, T(0.0), T(1.0));
    return begin + AB * t;
}
/*
template <class T>
Vec2<T> LineSegment<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    T t = randomEngine.next<T>(T(0.0), T(1.0));
    return begin + (end - begin) * t;
}
template <class T>
Vec2<T> LineSegment<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
*/
template <class T>
Vec2<T> LineSegment<T>::center() const
{
    return (begin + end) / T(2);
}

template <class T>
bool LineSegment<T>::intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const
{
    return Intersections::intersection(*this, lineSegment, intersectionPoint);
}

