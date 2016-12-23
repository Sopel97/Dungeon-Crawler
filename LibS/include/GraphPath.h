#pragma once

#include <vector>

namespace ls
{
    namespace
    {
        class GraphPath
        {
        private:
            std::vector<int> m_path;
        
        public:
            GraphPath() = default;
            GraphPath(std::vector<int>&& path);
            template <class ForwardIter>
            GraphPath(ForwardIter begin, ForwardIter end);
            GraphPath(std::initializer_list<int> vertices);

            GraphPath(const GraphPath& other) = default;
            GraphPath(GraphPath&& other) = default;

            GraphPath& operator=(const GraphPath& other) = default;
            GraphPath& operator=(GraphPath&& other) = default;

            int length() const;
            void append(int v);
            bool formsCycle() const;

            void reserve(int n);

            const std::vector<int>& vertices() const;
        };
    }
}

#include "../src/GraphPath.cpp"
