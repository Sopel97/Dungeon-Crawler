#pragma once

#include "..\Fwd.h"

#include <vector>
#include <initializer_list>

namespace ls
{
    template <class ShapeT>
    class ShapeMesh2 : public Shape2<typename ShapeT::ValueType>
    {
    public:
        using ShapeType = ShapeT;
        using ValueType = typename ShapeT::ValueType;

        ShapeMesh2() = default;
        explicit ShapeMesh2(const std::vector<ShapeT>& elements);
        explicit ShapeMesh2(std::vector<ShapeT>&& elements) noexcept(std::is_nothrow_move_constructible<std::vector<ShapeT>>::value);
        explicit ShapeMesh2(std::initializer_list<ShapeT> elements);
        template <class InputIt>
        ShapeMesh2(InputIt begin, InputIt end);

        ShapeMesh2(const ShapeMesh2<ShapeT>& other) = default;
        ShapeMesh2(ShapeMesh2<ShapeT>&& other) = default;

        ShapeMesh2<ShapeT>& operator=(const ShapeMesh2<ShapeT>& other) & = default;
        ShapeMesh2<ShapeT>& operator=(ShapeMesh2<ShapeT> && other) & = default;

        ShapeMesh2<ShapeT> translated(const Vec2<ValueType>& displacement) const;
        ShapeMesh2<ShapeT>& translate(const Vec2<ValueType>& displacement) &;

        template <class... Args>
        int emplaceShape(Args&&... args);
        void addShape(const ShapeT& newElement) &;
        void addShape(ShapeT&& newElement) &;
        template <class InputIt>
        void addShapes(InputIt begin, InputIt end) &;
        void merge(const ShapeMesh2<ShapeT>& other) &;
        void addShapes(std::initializer_list<ShapeT> newElements) &;

        const std::vector<ShapeT>& shapes() const &;
        std::vector<ShapeT> shapes() && ;
        const ShapeT& shape(int i) const;
        ShapeT& shape(int i);

        int size() const;

    protected:
        std::vector<ShapeT> m_shapes;
    };
}
#include "../src/ShapeMesh2.cpp"
