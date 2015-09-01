template <class T>
Ray<T>::Ray(const Vec2<T>& o, const Vec2<T>& d) : origin(o), direction(d.normalized())
{

}


template <class T>
Ray<T> Ray<T>::operator+(const Vec2<T>& v) const
{
    Ray<T> r(*this);
    r.origin += v;
    return r;
}
template <class T>
Ray<T>& Ray<T>::operator+=(const Vec2<T>& v)
{
    origin += v;
    return *this;
}
template <class T>
Ray<T> Ray<T>::operator-(const Vec2<T>& v) const
{
    Ray<T> r(*this);
    r.origin -= v;
    return r;
}
template <class T>
Ray<T>& Ray<T>::operator-=(const Vec2<T>& v)
{
    origin -= v;
    return *this;
}

template <class T>
template <class T2>
Ray<T>::operator Ray<T2>() const
{
    return Ray<T2>(static_cast<Vec2<T2>>(origin), static_cast<Vec2<T2>>(direction));
}

template <class T>
void Ray<T>::setDirection(const Vec2<T>& d)
{
    direction = d.normalized();
}

template <class T>
T Ray<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Ray<T>::nearestPointTo(const Vec2<T>& point) const
{
    Vec2<T> AO = point - origin;
    T t = AO.dot(direction) / direction.quadrance();
    t = std::max(t, T(0.0));
    return origin + direction * t;
}


