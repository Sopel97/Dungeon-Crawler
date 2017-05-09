#pragma once

#include <list>

namespace ls
{
    template <class TreeType>
    class TreeForest
    {
    public:
        using DataType = typename TreeType::DataType;
        using TreeStorageType = std::list<TreeType>;
        using TreeHandle = typename TreeStorageType::iterator;
        using ConstTreeHandle = typename TreeStorageType::const_iterator;
        using NodeIterator = typename TreeType::Iterator;
        using ConstNodeIterator = typename TreeType::ConstIterator;
    private:
        TreeStorageType m_trees;
    public:
        TreeForest() = default;
        TreeForest(const TreeForest& other) = delete;
        TreeForest(TreeForest&& other) :
            m_trees(std::move(other.m_trees))
        {
        }
        TreeForest& operator=(const TreeForest& other) = delete;
        TreeForest& operator=(TreeForest&& other)
        {
            m_trees = std::move(other.m_trees);

            return *this;
        }

        template <class... Args>
        TreeHandle emplaceTree(Args&&... args)
        {
            m_trees.emplace_back(std::forward<Args>(args)...);
            return std::prev(m_trees.end());
        }
        void removeTree(TreeHandle h)
        {
            m_trees.erase(h);
        }

        TreeHandle findTree(const TreeType* tree)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                if (&(*iter) == tree) return iter;
            }

            return m_trees.end();
        }
        TreeHandle findTreeWithRoot(const DataType& el)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                if (iter->croot().data() == el) return iter;
            }

            return m_trees.end();
        }
        ConstTreeHandle findTreeWithRoot(const DataType& el) const
        {
            for (auto iter = m_trees.cbegin(); iter != m_trees.cend(); ++iter)
            {
                if (iter->croot().data() == el) return iter;
            }

            return m_trees.cend();
        }

        std::pair<TreeHandle, NodeIterator> find(const DataType& el)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                auto found = iter->find(el);
                if (found.isValid()) return { iter, found };
            }
            return { m_trees.end(), NodeIterator(nullptr) };
        }
        template <class Func>
        std::pair<TreeHandle, NodeIterator> findIf(Func&& comparator)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                auto found = iter->findIf(std::forward<Func>(comparator));
                if (found.isValid()) return { iter, found };
            }
            return { m_trees.end(), NodeIterator(nullptr) };
        }
        std::pair<ConstTreeHandle, ConstNodeIterator> find(const DataType& el) const
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                auto found = iter->find(el);
                if (found.isValid()) return { iter, found };
            }
            return { m_trees.end(), ConstNodeIterator(nullptr) };
        }
        template <class Func>
        std::pair<ConstTreeHandle, ConstNodeIterator> findIf(Func&& comparator) const
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                auto found = iter->findIf(std::forward<Func>(comparator));
                if (found.isValid()) return { iter, found };
            }
            return { m_trees.end(), ConstNodeIterator(nullptr) };
        }

        bool isValid(ConstTreeHandle h) const
        {
            return h != m_trees.cend();
        }
    };
}