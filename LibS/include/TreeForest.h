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
        using NodeHandle = typename TreeType::NodeHandle;
        using ConstNodeHandle = typename TreeType::ConstNodeHandle;
    private:
        TreeStorageType m_trees;
    public:
        TreeForest() = default;
        TreeForest(const TreeForest& other) = delete;
        TreeForest(TreeForest&& other) :
            m_trees(std::move(other.m_trees))
        {
            other.m_trees.clear();
        }
        TreeForest& operator=(const TreeForest& other) = delete;
        TreeForest& operator=(TreeForest&& other)
        {
            m_trees = std::move(other.m_trees);

            return *this;
        }

        template <class U>
        TreeHandle emplaceTree(U&& root)
        {
            m_trees.emplace_back(std::forward<U>(root));
            return std::prev(m_trees.end());
        }
        void removeTree(TreeHandle h)
        {
            m_trees.erase(h);
        }

        TreeType& tree(TreeHandle h)
        {
            return *h;
        }
        const TreeType& tree(ConstTreeHandle h) const
        {
            return *h;
        }

        TreeHandle findTree(const DataType& el)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                if (iter->data(iter->rootConstHandle()) == el) return iter;
            }

            return m_trees.end();
        }
        ConstTreeHandle findTree(const DataType& el) const
        {
            for (auto iter = m_trees.cbegin(); iter != m_trees.cend(); ++iter)
            {
                if (iter->data(iter->rootConstHandle()) == el) return iter;
            }

            return m_trees.cend();
        }

        NodeHandle find(const DataType& el)
        {
            return const_cast<NodeHandle>(const_cast<const TreeForest<TreeType>*>(this)->find(el));
        }
        template <class Func>
        NodeHandle findIf(Func&& comparator)
        {
            return const_cast<NodeHandle>(const_cast<const TreeForest<TreeType>*>(this)->findIf(std::forward<Func>(comparator)));
        }
        ConstNodeHandle find(const DataType& el) const
        {
            for (const TreeType* tree : m_trees)
            {
                auto found = tree->find(el);
                if (found != TreeType::invalidHandle) return found;
            }
        }
        template <class Func>
        ConstNodeHandle findIf(Func&& comparator) const
        {
            for (const TreeType* tree : m_trees)
            {
                auto found = tree->find(std::forward<Func>(comparator));
                if (found != TreeType::invalidHandle) return found;
            }
        }

        bool isValidHandle(ConstTreeHandle h) const
        {
            return h != m_trees.cend();
        }
    };
}