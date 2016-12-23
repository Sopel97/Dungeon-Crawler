#include "../include/GraphPath.h"

namespace ls
{
    namespace
    {
        GraphPath::GraphPath(std::vector<int>&& path) :
            m_path(std::move(path))
        {
        }
        template <class ForwardIter>
        GraphPath::GraphPath(ForwardIter begin, ForwardIter end) :
            m_path(begin, end)
        {

        }
        GraphPath::GraphPath(std::initializer_list<int> vertices) :
            m_path(vertices.begin(), vertices.end())
        {

        }


        int GraphPath::length() const
        {
            return m_path.size();
        }
        void GraphPath::append(int v)
        {
            m_path.emplace_back(v);
        }
        bool GraphPath::formsCycle() const
        {
            return m_path.front() == m_path.back();
        }

        void GraphPath::reserve(int n)
        {
            m_path.reserve(n);
        }

        const std::vector<int>& GraphPath::vertices() const
        {
            return m_path;
        }
    }
}