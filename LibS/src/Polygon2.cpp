#include "..\include\Polygon2.h"

namespace ls
{
    template <class T>
    Polygon2<T>::Polygon2(std::initializer_list<Vec2<T>> vertices) :
        m_vertices(vertices)
    {
    }
    template <class T>
    Polygon2<T>::Polygon2(const std::vector<Vec2<T>>& vertices) :
        m_vertices(vertices)
    {
    }
    template <class T>
    Polygon2<T>::Polygon2(std::vector<Vec2<T>>&& vertices) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value) :
        m_vertices(std::move(vertices))
    {
    }
    template <class T>
    template <class InputIt>
    Polygon2<T>::Polygon2(InputIt begin, InputIt end) :
        m_vertices(begin, end)
    {

    }

    template <class T>
    Polygon2<T> Polygon2<T>::translated(const Vec2<T>& displacement) const
    {
        Polygon2 newPolygon(*this);
        newPolygon.translate(displacement);
        return newPolygon;
    }
    template <class T>
    Polygon2<T>& Polygon2<T>::translate(const Vec2<T>& displacement) &
    {
        for(Vec2<T>& vertex : m_vertices)
        {
            vertex += displacement;
        }
        return *this;
    }

    template <class T>
    template <class T2>
    Polygon2<T>::operator Polygon2<T2>() const
    {
        Polygon2<T2> poly;
        poly.reserve(m_vertices.size());
        for(const auto& v : m_vertices)
        {
            poly.push_back(static_cast<Vec2<T2>>(v));
        }
        return poly;
    }

    template <class T>
    template <class... Args>
    void Polygon2<T>::emplaceVertex(Args&&... coords)
    {
        m_vertices.emplace_back(std::forward<Args>(coords)...);
    }
    template <class T>
    void Polygon2<T>::addVertex(const Vec2<T>& v) &
    {
        m_vertices.push_back(v);
    }
    template <class T>
    template <class InputIt>
    void Polygon2<T>::addVertices(InputIt begin, InputIt end) &
    {
        m_vertices.insert(m_vertices.end(), begin(), end());
    }
    template <class T>
    void Polygon2<T>::addVertices(std::initializer_list<Vec2<T>> vertices) &
    {
        m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
    }

    template <class T>
    const std::vector<Vec2<T>>& Polygon2<T>::vertices() const &
    {
        return m_vertices;
    }
    template <class T>
    std::vector<Vec2<T>> Polygon2<T>::vertices() &&
    {
        return std::move(m_vertices);
    }
    template <class T>
    const Vec2<T>& Polygon2<T>::vertex(int i) const
    {
        return m_vertices[i];
    }
    template <class T>
    Vec2<T>& Polygon2<T>::vertex(int i)
    {
        return m_vertices[i];
    }

    template <class T>
    Vec2<T> Polygon2<T>::centerOfMass() const
    {
        T xsum{0};
        T ysum{0};
        T area{0};
        for(int i = 0; i < numberOfVertices(); ++i)
        {
            const Vec2<T>& p0 = m_vertices[i];
            const Vec2<T>& p1 = m_vertices[(i + 1) % numberOfVertices()];

            T areaSum = (p0.x * p1.y) - (p1.x * p0.y);

            xsum += (p0.x + p1.x) * areaSum;
            ysum += (p0.y + p1.y) * areaSum;

            area += areaSum;
        }

        T centMassX = xsum / (area * T(3));
        T centMassY = ysum / (area * T(3));

        return Vec2<T>(centMassX, centMassY);
    }
    template <class T>
    bool Polygon2<T>::isConvex() const
    {
        int numberOfVertices = m_vertices.size();
        T prevCross = 0; //will be initialized by correct cross when i equals 0 in the loop
        for(int i = 0; i < numberOfVertices; ++i)
        {
            const Vec2<T>& v0 = m_vertices[i];
            const Vec2<T>& v1 = m_vertices[(i + 1)&numberOfVertices];
            const Vec2<T>& v2 = m_vertices[(i + 2)&numberOfVertices];

            Vec2<T> e0 = v1 - v0;
            Vec2<T> e1 = v2 - v1;

            const T cross = e0.cross(e1);
            if(i != 0)
            {
                if(cross * prevCross < T(0)) return false; //if cross and prev cross have different signs
            }
            prevCross = cross;
        }
        return true;
    }
    template <class T>
    T Polygon2<T>::signedArea() const
    {
        T area = 0;
        for(int i = 0; i < numberOfVertices(); ++i)
        {
            const Vec2<T>& p0 = m_vertices[i];
            const Vec2<T>& p1 = m_vertices[(i + 1) % numberOfVertices()];

            area += (p0.x * p1.y) - (p1.x * p0.y);
        }
        return area * T(0.5);
    }
    template <class T>
    T Polygon2<T>::area() const
    {
        return std::abs(signedArea());
    }

    template <class T>
    int Polygon2<T>::numberOfVertices() const
    {
        return m_vertices.size();
    }

    template <class T>
    Polygon2<T> Polygon2<T>::regular(const Vec2<T>& center, int numberOfEdges, const T& radius)
    {
        Polygon2<T> polygon;
        polygon.m_vertices.reserve(numberOfEdges);
        for(int i = 0; i < numberOfEdges; ++i)
        {
            polygon.m_vertices.emplace_back(center.x + radius * static_cast<T>(std::cos(T(2) * static_cast<T>(PI) * static_cast<T>(i) / static_cast<T>(numberOfEdges))), center.y + radius * static_cast<T>(std::sin(T(2) * static_cast<T>(PI) * static_cast<T>(i) / static_cast<T>(numberOfEdges))));
        }
        return polygon;
    }
    template <class T>
    Polygon2<T> Polygon2<T>::fromRectangle(const Rectangle2<T>& rectangle)
    {
        return Polygon2<T> {
            Vec2<T>{rectangle.min.x, rectangle.min.y},
                Vec2<T>{rectangle.max.x, rectangle.min.y},
                Vec2<T>{rectangle.max.x, rectangle.max.y},
                Vec2<T>{rectangle.min.x, rectangle.max.y}
        };
    }
    template <class T>
    Polygon2<T> Polygon2<T>::fromTriangle(const Triangle2<T>& triangle)
    {
        return Polygon2<T> {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]};
    }



    template <class T>
    ConstPolygon2View<T> ConstPolygon2View<T>::ordered(const std::vector<const Vec2<T>*>& vertices)
    {
        ConstPolygon2View<T> object(vertices);

        object.sort();

        return object;
    }

    template <class T>
    ConstPolygon2View<T> ConstPolygon2View<T>::ordered(std::vector<const Vec2<T>*>&& vertices) noexcept(std::is_nothrow_constructible<ConstPolygon2View<T>, std::vector<const Vec2<T>*>&&>::value)
    {
        ConstPolygon2View<T> object(std::move(vertices));

        object.sort();

        return object;
    }

    template <class T>
    ConstPolygon2View<T>::ConstPolygon2View(const std::vector<const Vec2<T>*>& vertices) :
        m_v(vertices)
    {

    }
    template <class T>
    ConstPolygon2View<T>::ConstPolygon2View(std::vector<const Vec2<T>*>&& vertices) noexcept(std::is_nothrow_move_constructible<std::vector<const Vec2<T>*>>::value) :
        m_v(std::move(vertices))
    {

    }

    template <class T>
    const Vec2<T>& ConstPolygon2View<T>::vertex(int i) const
    {
        return *(m_v[i]);
    }
    template <class T>
    const std::vector<const Vec2<T>*>& ConstPolygon2View<T>::vertices() const &
    {
        return m_v;
    }
    template <class T>
    std::vector<const Vec2<T>*> ConstPolygon2View<T>::vertices() &&
    {
        return std::move(m_v);
    }

    template <class T>
    const Vec2<T>* ConstPolygon2View<T>::ptrToVertex(int i) const
    {
        return m_v[i];
    }

    template <class T>
    void ConstPolygon2View<T>::setPtrToVertex(int i, const Vec2<T>* newPtr) &
    {
        m_v[i] = newPtr;
    }

    template <class T>
    void ConstPolygon2View<T>::addVertex(const Vec2<T>* newVertex) &
    {
        m_v.push_back(newVertex);
    }
    template <class T>
    template <class InputIt>
    void ConstPolygon2View<T>::addVertices(InputIt begin, InputIt end) &
    {
        m_v.insert(m_v.end(), begin, end);
    }

    template <class T>
    void ConstPolygon2View<T>::sort() &
    {
        std::sort(m_v.begin(), m_v.end());
    }

    template <class T>
    int ConstPolygon2View<T>::numberOfVertices() const
    {
        return m_v.size();
    }

    template <class T>
    Polygon2<T> ConstPolygon2View<T>::toPolygon() const
    {
        std::vector<Vec2<T>> vertices;
        vertices.reserve(m_v.size());
        for(auto ptr : m_v)
        {
            vertices.emplace_back(*ptr);
        }
        return Polygon2<T>(std::move(vertices));
    }

    template <class T>
    bool operator==(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs)
    {
        return std::equal(lhs.vertices().begin(), lhs.vertices().end(), rhs.vertices().begin(), rhs.vertices().end());
    }
    template <class T>
    bool operator!=(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs)
    {
        return std::lexicographical_compare(lhs.vertices().begin(), lhs.vertices().end(), rhs.vertices().begin(), rhs.vertices().end());
    }
    template <class T>
    bool operator<=(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator>=(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs)
    {
        return !(lhs < rhs);
    }

}