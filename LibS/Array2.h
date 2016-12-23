#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <stack>
#include <tuple>
#include <functional>

namespace ls
{
    class IndexRegion2
    {
    public:
        struct Index
        {
            int x, y;
        };

    private:
        class const_iterator;

        class iterator
        {
        private:
        public:
            friend class const_iterator;

            using difference_type = int;
            using value_type = Index;
            using reference = Index&;
            using pointer = Index*;
            using iterator_category = std::forward_iterator_tag;

            iterator() = default;
            ~iterator() = default;
            iterator(int minX, int minY, int maxX, int maxY) noexcept : m_minY(minY), m_maxX(maxX), m_maxY(maxY), m_index{minX, minY} {}
            iterator(const iterator& other) = default;

            iterator& operator=(const iterator& other) = default;

            iterator  operator++ (int) { iterator i = *this; ++m_index.y; if(m_index.y > m_maxY) { m_index.y = m_minY; ++m_index.x; } return i; }
            iterator& operator++ () { ++m_index.y; if(m_index.y > m_maxY) { m_index.y = m_minY; ++m_index.x; } return *this; }

            reference operator*  () { return m_index; }
            pointer   operator-> () { return &m_index; }

            bool      operator== (const iterator& rhs) { return std::tie(m_index.x, m_index.y) == std::tie(rhs.m_index.x, rhs.m_index.y); }
            bool      operator!= (const iterator& rhs) { return std::tie(m_index.x, m_index.y) != std::tie(rhs.m_index.x, rhs.m_index.y); }

        private:
            int m_minY;
            int m_maxX;
            int m_maxY;
            Index m_index;
        };

        class const_iterator
        {
        private:
        public:
            using difference_type = int;
            using value_type = Index;
            using const_reference = const Index&;
            using const_pointer = const Index*;
            using iterator_category = std::forward_iterator_tag;

            const_iterator() = default;
            ~const_iterator() = default;
            const_iterator(int minX, int minY, int maxX, int maxY) noexcept : m_minY(minY), m_maxX(maxX), m_maxY(maxY), m_index{minX, minY} {}
            const_iterator(const iterator& other) noexcept : m_minY(other.m_minY), m_maxX(other.m_maxX), m_maxY(other.m_maxY), m_index(other.m_index) {}
            const_iterator(const const_iterator& other) = default;

            const_iterator& operator=(const const_iterator& other) = default;

            const_iterator  operator++ (int) { const_iterator i = *this; ++m_index.y; if(m_index.y > m_maxY) { m_index.y = m_minY; ++m_index.x; } return i; }
            const_iterator& operator++ () { ++m_index.y; if(m_index.y > m_maxY) { m_index.y = m_minY; ++m_index.x; } return *this; }

            const_reference operator*  () { return m_index; }
            const_pointer   operator-> () { return &m_index; }

            bool      operator== (const iterator& rhs) { return std::tie(m_index.x, m_index.y) == std::tie(rhs.m_index.x, rhs.m_index.y); }
            bool      operator!= (const iterator& rhs) { return std::tie(m_index.x, m_index.y) != std::tie(rhs.m_index.x, rhs.m_index.y); }

        private:
            int m_minY;
            int m_maxX;
            int m_maxY;
            Index m_index;
        };

    public:

        IndexRegion2(int minX, int minY, int maxX, int maxY) noexcept :
            m_minX(minX),
            m_minY(minY),
            m_maxX(maxX),
            m_maxY(maxY)
        {

        }

        iterator begin()
        {
            return iterator(m_minX, m_minY, m_maxX, m_maxY);
        }
        iterator end()
        {
            return iterator(m_maxX + 1, m_minY, m_maxX, m_maxY);
        }
        const_iterator begin() const
        {
            return const_iterator(m_minX, m_minY, m_maxX, m_maxY);
        }
        const_iterator end() const
        {
            return const_iterator(m_maxX + 1, m_minY, m_maxX, m_maxY);
        }
        const_iterator cbegin() const
        {
            return iterator(m_minX, m_minY, m_maxX, m_maxY);
        }
        const_iterator cend() const
        {
            return const_iterator(m_maxX + 1, m_minY, m_maxX, m_maxY);
        }
    private:
        int m_minX;
        int m_minY;
        int m_maxX;
        int m_maxY;
    };

    template <class T>
    class Array2
    {
    private:

    public:
        using ValueType = T;

        class ConstCol;

        class Col
        {
        public:
            friend class ConstCol;

            using iterator = T*;
            using const_iterator = const T*;

            Col() = default;

            Col(T* dataCol, int size) noexcept :
                m_dataCol(dataCol),
                m_colSize(size)
            {
            }

            Col(const Col& col) = default;
            Col(Col&& col) = default;

            T& operator[] (int y)
            {
                return *(m_dataCol + y);
            }
            const T& operator[] (int y) const
            {
                return *(m_dataCol + y);
            }

            Col& operator= (const Col& col) = default;
            Col& operator= (Col && col) = default;

            Col& operator= (std::initializer_list<T> list)
            {
                int i = 0;
                for(const T& v : list)
                {
                    operator[](i) = v;
                    ++i;
                }
            }

            const T* data() const
            {
                return m_dataCol;
            }

            void fill(const T& value)
            {
                std::fill(begin(), end(), value);
            }

            int size() const
            {
                return m_colSize;
            }

            bool empty() const
            {
                return m_dataCol == nullptr;
            }

            iterator begin()
            {
                return m_dataCol;
            }

            iterator end()
            {
                return m_dataCol + m_colSize;
            }
            const_iterator begin() const
            {
                return m_dataCol;
            }

            const_iterator end() const
            {
                return m_dataCol + m_colSize;
            }
            const_iterator cbegin() const
            {
                return m_dataCol;
            }

            const_iterator cend() const
            {
                return m_dataCol + m_colSize;
            }

            Col previousCol(int n = 1) const
            {
                return Col(m_dataCol - (m_colSize * n), m_colSize);
            }
            Col nextCol(int n = 1) const
            {
                return Col(m_dataCol + (m_colSize * n), m_colSize);
            }
            Col& moveToPreviousCol(int n = 1)
            {
                m_dataCol -= (m_colSize * n);

                return *this;
            }
            Col& moveToNextCol(int n = 1)
            {
                m_dataCol += (m_colSize * n);

                return *this;
            }

        private:
            T* m_dataCol;
            int m_colSize;
        };

        class ConstCol
        {
        public:
            using const_iterator = const T*;

            ConstCol() = default;

            ConstCol(const T* dataCol, int size) noexcept :
                m_dataCol(dataCol),
                m_colSize(size)
            {
            }

            ConstCol(const Col& col) noexcept :
            m_dataCol(col.m_dataCol),
                m_colSize(col.m_colSize)
            {

            }

            ConstCol(const ConstCol& col) = default;
            ConstCol(ConstCol&& col) = default;

            const T& operator[] (int y) const
            {
                return *(m_dataCol + y);
            }

            ConstCol& operator= (const Col& col)
            {
                m_dataCol = col.m_dataCol;
                m_colSize = col.m_colSize;
                return *this;
            }

            ConstCol& operator= (Col && col)
            {
                m_dataCol = std::move(col.m_dataCol);
                m_colSize = std::move(col.m_colSize);
                return *this;
            }

            const T* data() const
            {
                return m_dataCol;
            }

            void fill(const T& value)
            {
                std::fill(begin(), end(), value);
            }

            int size() const
            {
                return m_colSize;
            }

            bool empty() const
            {
                return m_dataCol == nullptr;
            }

            const_iterator begin() const
            {
                return m_dataCol;
            }

            const_iterator end() const
            {
                return m_dataCol + m_colSize;
            }
            const_iterator cbegin() const
            {
                return m_dataCol;
            }

            const_iterator cend() const
            {
                return m_dataCol + m_colSize;
            }

            ConstCol previousCol(int n = 1) const
            {
                return ConstCol(m_dataCol - (m_colSize * n), m_colSize);
            }
            ConstCol nextCol(int n = 1) const
            {
                return ConstCol(m_dataCol + (m_colSize * n), m_colSize);
            }
            ConstCol& moveToPreviousCol(int n = 1)
            {
                m_dataCol -= (m_colSize * n);

                return *this;
            }
            ConstCol& moveToNextCol(int n = 1)
            {
                m_dataCol += (m_colSize * n);

                return *this;
            }

        private:
            const T* m_dataCol;
            int m_colSize;
        };

        class ConstRow;

        class Row
        {
        public:
            friend class ConstRow;

            class iterator
            {
            public:
                using difference_type = int;
                using value_type = T;
                using reference = T&;
                using pointer = T*;
                using iterator_category = std::random_access_iterator_tag;

                iterator() noexcept : m_start(nullptr), m_ptr(nullptr), m_jump(0) {}
                iterator(const iterator& other) = default;
                iterator(pointer ptr, difference_type jump) noexcept : m_start(ptr), m_ptr(ptr), m_jump(jump) {}

                iterator&      operator=  (const iterator& other) = default;

                iterator       operator++ (int) { iterator i = *this; m_ptr += m_jump; return i; }
                iterator       operator-- (int) { iterator i = *this; m_ptr -= m_jump; return i; }
                iterator&      operator++ () { m_ptr += m_jump; return *this; }
                iterator&      operator-- () { m_ptr -= m_jump; return *this; }

                iterator       operator+  (int n) { iterator i = *this; i.m_ptr += m_jump * n; return i; }
                iterator       operator-  (int n) { iterator i = *this; i.m_ptr -= m_jump * n; return i; }
                iterator&      operator+= (int n) { m_ptr += m_jump * n; return *this; }
                iterator&      operator-= (int n) { m_ptr -= m_jump * n; return *this; }

                difference_type operator-  (const iterator& n) { return (m_ptr - n.m_ptr) / m_jump; }

                reference       operator[] (int n) { return m_start[m_jump * n]; }
                reference       operator*  () { return *m_ptr; }
                pointer         operator-> () { return m_ptr; }

                bool            operator== (const iterator& rhs) { return m_ptr == rhs.m_ptr; }
                bool            operator<  (const iterator& rhs) { return m_ptr < rhs.m_ptr; }
                bool            operator>  (const iterator& rhs) { return m_ptr > rhs.m_ptr; }
                bool            operator<= (const iterator& rhs) { return m_ptr <= rhs.m_ptr; }
                bool            operator>= (const iterator& rhs) { return m_ptr >= rhs.m_ptr; }
                bool            operator!= (const iterator& rhs) { return m_ptr != rhs.m_ptr; }

            private:
                friend class const_iterator;

                pointer m_start;
                pointer m_ptr;
                difference_type m_jump;
            };

            class const_iterator
            {
            public:
                using difference_type = int;
                using value_type = T;
                using const_reference = const T&;
                using const_pointer = const T*;
                using iterator_category = std::random_access_iterator_tag;

                const_iterator() noexcept : m_start(nullptr), m_ptr(nullptr), m_jump(0) {}
                const_iterator(const iterator& it) noexcept : m_start(it.m_start), m_ptr(it.m_ptr), m_jump(it.m_jump) {}
                const_iterator(const const_iterator& other) = default;
                const_iterator(const_pointer ptr, difference_type jump) noexcept : m_start(ptr), m_ptr(ptr), m_jump(jump) {}

                const_iterator& operator=  (const const_iterator& other) = default;

                const_iterator  operator++ (int) { const_iterator i = *this; m_ptr += m_jump; return i; }
                const_iterator  operator-- (int) { const_iterator i = *this; m_ptr -= m_jump; return i; }
                const_iterator& operator++ () { m_ptr += m_jump; return *this; }
                const_iterator& operator-- () { m_ptr -= m_jump; return *this; }

                const_iterator  operator+  (int n) { const_iterator i = *this; i.m_ptr += m_jump * n; return i; }
                const_iterator  operator-  (int n) { const_iterator i = *this; i.m_ptr -= m_jump * n; return i; }
                const_iterator& operator+= (int n) { m_ptr += m_jump * n; return *this; }
                const_iterator& operator-= (int n) { m_ptr -= m_jump * n; return *this; }

                difference_type operator-  (const const_iterator& n) { return (m_ptr - n.m_ptr) / m_jump; }

                const_reference operator[] (int n) { return m_start[m_jump * n]; }
                const_reference operator*  () { return *m_ptr; }
                const_pointer   operator-> () { return m_ptr; }

                bool            operator== (const const_iterator& rhs) { return m_ptr == rhs.m_ptr; }
                bool            operator<  (const const_iterator& rhs) { return m_ptr < rhs.m_ptr; }
                bool            operator>  (const const_iterator& rhs) { return m_ptr > rhs.m_ptr; }
                bool            operator<= (const const_iterator& rhs) { return m_ptr <= rhs.m_ptr; }
                bool            operator>= (const const_iterator& rhs) { return m_ptr >= rhs.m_ptr; }
                bool            operator!= (const const_iterator& rhs) { return m_ptr != rhs.m_ptr; }

            private:
                const_pointer m_start;
                const_pointer m_ptr;
                difference_type m_jump;
            };

            Row() = default;

            Row(T* dataRow, int size, int colSize) noexcept :
                m_dataRow(dataRow),
                m_rowSize(size),
                m_colSize(colSize)
            {
            }

            Row(const Row& row) = default;
            Row(Row&& row) = default;

            T& operator[] (int x)
            {
                return *(m_dataRow + x * m_colSize);
            }
            const T& operator[] (int x) const
            {
                return *(m_dataRow + x * m_colSize);
            }
            Row& operator= (const Row& row) noexcept
            {
                m_dataRow = row.m_dataRow;
                m_rowSize = row.m_rowSize;
                m_colSize = row.m_colSize;

                return *this;
            }
            Row& operator= (Row && row) noexcept
            {
                m_dataRow = std::move(row.m_dataRow);
                m_rowSize = std::move(row.m_rowSize);
                m_colSize = std::move(row.m_colSize);

                return *this;
            }


            Row& operator= (std::initializer_list<T> list)
            {
                int i = 0;
                for(const T& v : list)
                {
                    operator[](i) = v;
                    ++i;
                }
            }

            void fill(const T& value)
            {
                std::fill(begin(), end(), value);
            }

            int size() const
            {
                return m_rowSize;
            }

            bool empty() const
            {
                return m_dataRow == nullptr;
            }

            iterator begin()
            {
                return iterator(m_dataRow, m_colSize);
            }

            iterator end()
            {
                return iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
            }
            const_iterator begin() const
            {
                return const_iterator(m_dataRow, m_colSize);
            }

            const_iterator end() const
            {
                return const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
            }
            const_iterator cbegin() const
            {
                return const_iterator(m_dataRow, m_colSize);
            }

            const_iterator cend() const
            {
                return const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
            }
            Row previousRow(int n = 1) const
            {
                return Row(m_dataRow - n, m_rowSize, m_colSize);
            }
            Row nextRow(int n = 1) const
            {
                return Row(m_dataRow + n, m_rowSize, m_colSize);
            }
            Row& moveToPreviousRow(int n = 1)
            {
                m_dataRow -= n;

                return *this;
            }
            Row& moveToNextRow(int n = 1)
            {
                m_dataRow += n;

                return *this;
            }
        private:
            T* m_dataRow;
            int m_rowSize;
            int m_colSize;
        };

        class ConstRow
        {
        public:
            ConstRow() = default;

            ConstRow(T* dataRow, int size, int colSize) noexcept :
                m_dataRow(dataRow),
                m_rowSize(size),
                m_colSize(colSize)
            {
            }

            ConstRow(const Row& row) noexcept :
            m_dataRow(row.m_dataRow),
                m_rowSize(row.m_rowSize),
                m_colSize(row.m_colSize)
            {
            }

            ConstRow(const ConstRow& row) = default;
            ConstRow(ConstRow&& row) = default;

            const T& operator[] (int x) const
            {
                return *(m_dataRow + x * m_colSize);
            }
            ConstRow& operator= (const ConstRow& row) noexcept
            {
                m_dataRow = row.m_dataRow;
                m_rowSize = row.m_rowSize;
                m_colSize = row.m_colSize;

                return *this;
            }
            ConstRow& operator= (ConstRow && row)
            {
                m_dataRow = std::move(row.m_dataRow);
                m_rowSize = std::move(row.m_rowSize);
                m_colSize = std::move(row.m_colSize);

                return *this;
            }

            int size() const
            {
                return m_rowSize;
            }

            bool empty() const
            {
                return m_dataRow == nullptr;
            }

            typename Row::const_iterator begin() const
            {
                return Row::const_iterator(m_dataRow, m_colSize);
            }

            typename Row::const_iterator end() const
            {
                return Row::const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
            }
            typename Row::const_iterator cbegin() const
            {
                return Row::const_iterator(m_dataRow, m_colSize);
            }

            typename Row::const_iterator cend() const
            {
                return Row::const_iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
            }
            ConstRow previousRow(int n = 1) const
            {
                return ConstRow(m_dataRow - n, m_rowSize, m_colSize);
            }
            ConstRow nextRow(int n = 1) const
            {
                return ConstRow(m_dataRow + n, m_rowSize, m_colSize);
            }
            ConstRow& moveToPreviousRow(int n = 1)
            {
                m_dataRow -= n;

                return *this;
            }
            ConstRow& moveToNextRow(int n = 1)
            {
                m_dataRow += n;

                return *this;
            }
        private:
            const T* m_dataRow;
            int m_rowSize;
            int m_colSize;
        };

        using iterator = T*;
        using const_iterator = const T*;

        Array2() :
            m_data(nullptr),
            m_width(0),
            m_height(0)
        {

        }

        Array2(int width, int height) :
            m_width(width),
            m_height(height)
        {
            const int totalSize = width * height;
            m_data = std::make_unique<T[]>(totalSize);
        }

        Array2(int width, int height, const T& initValue) :
            m_width(width),
            m_height(height)
        {
            const int totalSize = width * height;
            m_data = std::make_unique<T[]>(totalSize);
            for(int i = 0; i < totalSize; ++i)
            {
                m_data[i] = initValue;
            }
        }

        Array2(const Array2<T>& other) :
            m_width(other.m_width),
            m_height(other.m_height)
        {
            const int totalSize = m_width * m_height;
            m_data = std::make_unique<T[]>(totalSize);
            for(int i = 0; i < totalSize; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        Array2(Array2<T>&& other) noexcept :
        m_data(std::move(other.m_data)),
            m_width(std::move(other.m_width)),
            m_height(std::move(other.m_height))
        {
            other.m_data = nullptr;
        }

        Array2(const std::initializer_list<std::initializer_list<T>>& list)
        {
            m_height = list.size();
            m_width = (*(list.begin())).size();
            const int totalSize = m_width * m_height;
            m_data = std::make_unique<T[]>(totalSize);
            int y = 0;
            for(const auto& row : list)
            {
                int x = 0;
                for(const auto& v : row)
                {
                    at(x, y) = v;
                    ++x;
                }
                ++y;
            }
        }

        ConstCol operator[] (int x) const
        {
            return ConstCol(m_data.get() + x * m_height, m_height);
        }

        Col operator[] (int x)
        {
            return Col(m_data.get() + x * m_height, m_height);
        }

        const T& operator() (int x, int y) const
        {
            return m_data[x * m_height + y];
        }
        T& operator() (int x, int y)
        {
            return m_data[x * m_height + y];
        }
        const T& at(int x, int y) const
        {
            return m_data[x * m_height + y];
        }
        T& at(int x, int y)
        {
            return m_data[x * m_height + y];
        }

        Array2& operator= (Array2<T> && other) noexcept
        {
            m_data = std::move(other.m_data);
            m_width = other.m_width;
            m_height = other.m_height;

            other.m_data = nullptr;

            return *this;
        }

        Array2& operator= (const Array2<T>& other)
        {
            return operator=(std::move(Array2(other)));
        }

        Array2& operator= (const std::initializer_list<std::initializer_list<T>>& list)
        {
            return operator=(std::move(Array2(list)));
        }
        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }
        template <class Comp = std::equal_to<T>>
        void floodFill(int x, int y, const T& value, Comp comp = std::equal_to<T>())
        {
            int width = m_width;
            int height = m_height;

            std::stack<IndexRegion2::Index> coordsStack;
            coordsStack.push(IndexRegion2::Index{x, y});
            const T initialCell = at(x, y);

            while(!coordsStack.empty())
            {
                const IndexRegion2::Index ind = coordsStack.top();
                coordsStack.pop();
                T& thisCell = at(ind.x, ind.y);

                if(comp(value, thisCell)) continue;

                thisCell = value;
                if(ind.x > 0 && comp(at(ind.x - 1, ind.y), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x - 1, ind.y});
                if(ind.y > 0 && comp(at(ind.x, ind.y - 1), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x, ind.y - 1});
                if(ind.x < width - 1 && comp(at(ind.x + 1, ind.y), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x + 1, ind.y});
                if(ind.y < height - 1 && comp(at(ind.x, ind.y + 1), initialCell)) coordsStack.push(IndexRegion2::Index{ind.x, ind.y + 1});
            }
        }

        void swap(Array2<T>& other)
        {
            Array2<T> temp(std::move(*this));
            *this = std::move(other);
            other = std::move(temp);
        }

        const T* data() const
        {
            return m_data.get();
        }

        int width() const
        {
            return m_width;
        }

        int height() const
        {
            return m_height;
        }

        std::pair<int, int> size() const
        {
            return std::pair<int, int>(m_width, m_height);
        }

        bool empty() const
        {
            return m_data == nullptr;
        }

        iterator begin()
        {
            return m_data.get();
        }

        iterator end()
        {
            const int totalSize = m_width * m_height;
            return m_data.get() + totalSize;
        }
        const_iterator begin() const
        {
            return m_data.get();
        }

        const_iterator end() const
        {
            const int totalSize = m_width * m_height;
            return m_data.get() + totalSize;
        }
        const_iterator cbegin() const
        {
            return m_data.get();
        }

        const_iterator cend() const
        {
            const int totalSize = m_width * m_height;
            return m_data.get() + totalSize;
        }

        Row row(int n)
        {
            return Row(m_data.get() + n, m_width, m_height);
        }

        ConstRow row(int n) const
        {
            return ConstRow(m_data.get() + n, m_width, m_height);
        }

        ConstRow crow(int n) const
        {
            return ConstRow(m_data.get() + n, m_width, m_height);
        }

        Col col(int n)
        {
            return operator[](n);
        }

        ConstCol col(int n) const
        {
            return operator[](n);
        }

        ConstCol ccol(int n) const
        {
            return operator[](n);
        }

        IndexRegion2 possibleIndices() const
        {
            return IndexRegion2(0, 0, m_width - 1, m_height - 1);
        }

    private:
        std::unique_ptr<T[]> m_data;
        int m_width;
        int m_height;
    };

    template <class T>
    void swap(Array2<T>& lhs, Array2<T>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}
