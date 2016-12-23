#include "..\include\Raycast2.h"

namespace ls
{
    template <class T>
    Raycast2<T>::Raycast2(const Ray2<T>& _ray) noexcept(std::is_nothrow_copy_constructible<Ray2<T>>::value) :
        Raycast2(_ray, {})
    {

    }
    template <class T>
    Raycast2<T>::Raycast2(const Ray2<T>& _ray, const std::vector<typename Raycast2<T>::Hit>& _hits) :
        ray(_ray),
        m_hits(_hits)
    {

    }
    template <class T>
    Raycast2<T>::Raycast2(const Ray2<T>& _ray, std::vector<typename Raycast2<T>::Hit>&& _hits) noexcept(std::is_nothrow_copy_constructible<Ray2<T>>::value && std::is_nothrow_move_constructible<std::vector<Hit>>::value) :
        ray(_ray),
        m_hits(std::move(_hits))
    {

    }
    template <class T>
    int Raycast2<T>::numberOfHits() const
    {
        return m_hits.size();
    }

    template <class T>
    void Raycast2<T>::sortHitPointsByDistance() &
    {
        std::sort(m_hits.begin(), m_hits.end(), [] (const Hit & lhs, const Hit & rhs)->bool {return lhs.distance < rhs.distance; });
    }

    template <class T>
    const std::vector<typename Raycast2<T>::Hit>& Raycast2<T>::hits() const &
    {
        return m_hits;
    }
    template <class T>
    std::vector<typename Raycast2<T>::Hit> Raycast2<T>::hits() &&
    {
        return std::move(m_hits);
    }
}