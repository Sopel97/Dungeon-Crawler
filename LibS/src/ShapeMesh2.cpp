#include "..\include\ShapeMesh2.h"

namespace ls
{
    template <class ShapeT>
    ShapeMesh2<ShapeT>::ShapeMesh2(const std::vector<ShapeT>& elements) :
        m_shapes(elements)
    {
    }
    template <class ShapeT>
    ShapeMesh2<ShapeT>::ShapeMesh2(std::vector<ShapeT>&& elements) noexcept(std::is_nothrow_move_constructible<std::vector<ShapeT>>::value) :
        m_shapes(std::move(elements))
    {
    }
    template <class ShapeT>
    ShapeMesh2<ShapeT>::ShapeMesh2(std::initializer_list<ShapeT> elements) : 
        m_shapes(elements)
    {
    }
    template <class ShapeT>
    template <class InputIt>
    ShapeMesh2<ShapeT>::ShapeMesh2(InputIt begin, InputIt end) :
        m_shapes(begin, end)
    {

    }

    template <class ShapeT>
    ShapeMesh2<ShapeT> ShapeMesh2<ShapeT>::translated(const Vec2<ValueType>& displacement) const
    {
        ShapeMesh2<ShapeT> m(*this);
        m += displacement;
        return m;
    }
    template <class ShapeT>
    ShapeMesh2<ShapeT>& ShapeMesh2<ShapeT>::translate(const Vec2<ValueType>& displacement) &
    {
        int size = m_shapes.size();
        for(T& element : m_shapes)
        {
            element += displacement;
        }
        return *this;
    }
    template <class ShapeT>
    template <class... Args>
    int ShapeMesh2<ShapeT>::emplaceShape(Args&&... args)
    {
        m_shapes.emplace_back(std::forward<Args>(args)...);
    }

    template <class ShapeT>
    void ShapeMesh2<ShapeT>::addShape(const ShapeT& newElement) &
    {
        m_shapes.push_back(newElement);
    }
    template <class ShapeT>
    void ShapeMesh2<ShapeT>::addShape(ShapeT&& newElement) &
    {
        m_shapes.push_back(std::move(newElemente));
    }
    template <class ShapeT>
    template <class InputIt>
    void ShapeMesh2<ShapeT>::addShapes(InputIt begin, InputIt end) &
    {
        m_shapes.insert(m_shapes.end(), begin(), end());
    }
    template <class ShapeT>
    void ShapeMesh2<ShapeT>::merge(const ShapeMesh2<ShapeT>& other) &
    {
        addShapes(other.m_shapes.begin(), other.m_shapes.end());
    }
    template <class ShapeT>
    void ShapeMesh2<ShapeT>::addShapes(std::initializer_list<ShapeT> elements) &
    {
        m_shapes.insert(m_shapes.end(), elements.begin, elements.end());
    }

    template <class ShapeT>
    const std::vector<ShapeT>& ShapeMesh2<ShapeT>::shapes() const &
    {
        return m_shapes;
    }
    template <class ShapeT>
    std::vector<ShapeT> ShapeMesh2<ShapeT>::shapes() &&
    {
        return std::move(m_shape);
    }
    template <class ShapeT>
    const ShapeT& ShapeMesh2<ShapeT>::shape(int i) const
    {
        return m_shapes[i];
    }
    template <class ShapeT>
    ShapeT& ShapeMesh2<ShapeT>::shape(int i)
    {
        return m_shapes[i];
    }

    template <class ShapeT>
    int ShapeMesh2<ShapeT>::size() const
    {
        return m_shapes.size();
    }
}