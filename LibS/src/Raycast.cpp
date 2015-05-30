template <class T>
Raycast<T>::Raycast(const Ray<T>& _ray, const std::vector<Raycast<T>::Hit>& _hits) : ray(_ray), hits(_hits)
{

}
template <class T>
Raycast<T>::Raycast(const Ray<T>& _ray, std::vector<Raycast<T>::Hit>&& _hits) : ray(_ray), hits(std::move(_hits))
{

}
template <class T>
size_t Raycast<T>::hitCount() const
{
    return hits.size();
}

template <class T>
void Raycast<T>::sortHitPointsByDistance()
{
    std::sort(hits.begin(), hits.end(), [](const Hit & lhs, const Hit & rhs)->bool {return lhs.distance < rhs.distance;});
}

template <class T>
Raycast<T> Raycast<T>::empty(const Ray<T>& r)
{
    return Raycast<T>(r, std::vector<Raycast<T>::Hit>{});
}
