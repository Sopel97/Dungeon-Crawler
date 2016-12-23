#include "..\include\Polyline2.h"

namespace ls
{
    template <class T>
    Polyline2<T>::Polyline2(std::initializer_list<Vec2<T>> vertices) :
        m_vertices(vertices)
    {
    }
    template <class T>
    Polyline2<T>::Polyline2(const std::vector<Vec2<T>>& vertices) :
        m_vertices(vertices)
    {
    }
    template <class T>
    Polyline2<T>::Polyline2(std::vector<Vec2<T>>&& vertices) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value) :
        m_vertices(std::move(vertices))
    {
    }
    template <class T>
    template <class InputIt>
    Polyline2<T>::Polyline2(InputIt begin, InputIt end) :
        m_vertices(begin, end)
    {

    }

    template <class T>
    Polyline2<T> Polyline2<T>::translated(const Vec2<T>& displacement) const
    {
        Polyline2 newPolyline(*this);
        newPolyline.translate(displacement);
        return newPolyline;
    }
    template <class T>
    Polyline2<T>& Polyline2<T>::translate(const Vec2<T>& displacement) &
    {
        for(Vec2<T>& vertex : m_vertices)
        {
            vertex += displacement;
        }
        return *this;
    }

    template <class T>
    template <class T2>
    Polyline2<T>::operator Polyline2<T2>() const
    {
        Polyline2<T2> poly;
        poly.reserve(m_vertices.size());
        for(const auto& v : m_vertices)
        {
            poly.push_back(static_cast<Vec2<T2>>(v));
        }
        return poly;
    }

    template <class T>
    template <class... Args>
    int Polyline2<T>::emplaceVertex(Args&&... coords)
    {
        m_vertices.emplace_back(std::forward<Args>(coords)...);
    }
    template <class T>
    void Polyline2<T>::addVertex(const Vec2<T>& newVertex) &
    {
        m_vertices.push_back(newVertex);
    }
    template <class T>
    template <class InputIt>
    void Polyline2<T>::addVertices(InputIt begin, InputIt end) &
    {
        m_vertices.insert(m_vertices.end(), begin(), end());
    }
    template <class T>
    void Polyline2<T>::addVertices(std::initializer_list<Vec2<T>> newVertices) &
    {
        m_vertices.insert(m_vertices.end(), newVertices.begin(), newVertices.end());
    }

    template <class T>
    const std::vector<Vec2<T>>& Polyline2<T>::vertices() const &
    {
        return m_vertices;
    }
    template <class T>
    std::vector<Vec2<T>> Polyline2<T>::vertices() &&
    {
        return std::move(m_vertices);
    }
    template <class T>
    const Vec2<T>& Polyline2<T>::vertex(int i) const
    {
        return m_vertices[i];
    }
    template <class T>
    Vec2<T>& Polyline2<T>::vertex(int i)
    {
        return m_vertices[i];
    }

    template <class T>
    int Polyline2<T>::size() const
    {
        return m_vertices.size();
    }


    template <class T>
    Polyline2<T> Polyline2<T>::fromLineSegment(const LineSegment2<T>& lineSegment)
    {
        return Polyline2<T> {lineSegment.begin, lineSegment.end};
    }



    template <class T>
    ConstPolyline2View <T> ConstPolyline2View<T>::ordered(const std::vector<const Vec2<T>*>& vertices)
    {
        ConstPolyline2View<T> object(vertices);

        object.sort();

        return object;
    }
    template <class T>
    ConstPolyline2View <T> ConstPolyline2View<T>::ordered(std::vector<const Vec2<T>*>&& vertices) noexcept
    {
        ConstPolyline2View<T> object(std::move(vertices));

        object.sort();

        return object;
    }

    template <class T>
    ConstPolyline2View<T>::ConstPolyline2View(const std::vector<const Vec2<T>*>& vertices) :
        m_v(vertices)
    {

    }
    template <class T>
    ConstPolyline2View<T>::ConstPolyline2View(std::vector<const Vec2<T>*>&& vertices) noexcept :
    m_v(std::move(vertices))
    {

    }

    template <class T>
    ConstPolyline2View<T>::ConstPolyline2View(const ConstPolyline2View<T>& other) :
        m_v(other.m_v)
    {

    }
    template <class T>
    ConstPolyline2View<T>::ConstPolyline2View(ConstPolyline2View<T>&& other) noexcept :
    m_v(std::move(other.m_v))
    {

    }

    template <class T>
    ConstPolyline2View<T>& ConstPolyline2View<T>::operator=(const ConstPolyline2View<T>& other) &
    {
        m_v = other.m_v;

        return *this;
    }
    template <class T>
    ConstPolyline2View<T>& ConstPolyline2View<T>::operator=(ConstPolyline2View<T>&& other) & noexcept
    {
        m_v = std::move(other.m_v);

        return *this;
    }
    
    template <class T>
    const Vec2<T>& ConstPolyline2View<T>::vertex(int i) const
    {
        return *(m_v[i]);
    }

    template <class T>
    const Vec2<T>* ConstPolyline2View<T>::ptrToVertex(int i) const
    {
        return m_v[i];
    }

    template <class T>
    void ConstPolyline2View<T>::setPtrToVertex(int i, const Vec2<T>* newPtr) &
    {
        m_v[i] = newPtr;
    }

    template <class T>
    void ConstPolyline2View<T>::addVertex(const Vec2<T>* newVertex) &
    {
        m_v.push_back(newVertex);
    }
    template <class T>
    template <class InputIt>
    void ConstPolyline2View<T>::addVertices(InputIt begin, InputIt end) &
    {
        m_v.insert(m_v.end(), begin, end);
    }

    template <class T>
    void ConstPolyline2View<T>::sort() &
    {
        std::sort(m_v.begin(), m_v.end());
    }

    template <class T>
    int ConstPolyline2View<T>::numberOfVertices() const
    {
        return m_v.size();
    }

    template <class T>
    Polyline2<T> ConstPolyline2View<T>::toPolyline() const
    {
        std::vector<Vec2<T>> vertices;
        vertices.reserve(m_v.size());
        for(auto ptr : m_v)
        {
            vertices.emplace_back(*ptr);
        }
        return Polyline2<T>(std::move(vertices));
    }

    template <class T>
    bool operator==(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs)
    {
        return std::equal(lhs.vertices().begin(), lhs.vertices().end(), rhs.vertices().begin(), rhs.vertices().end());
    }
    template <class T>
    bool operator!=(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs)
    {
        return std::lexicographical_compare(lhs.vertices().begin(), lhs.vertices().end(), rhs.vertices().begin(), rhs.vertices().end());
    }
    template <class T>
    bool operator<=(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator>=(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs)
    {
        return !(lhs < rhs);
    }
}