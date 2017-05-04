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

        TreeHandle findTree(const TreeType* tree)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                if (&(*iter) == tree) return iter;
            }

            return m_trees.end();
        }
        TreeHandle findTree(const DataType& el)
        {
            for (auto iter = m_trees.begin(); iter != m_trees.end(); ++iter)
            {
                if (iter->croot().data() == el) return iter;
            }

            return m_trees.end();
        }
        ConstTreeHandle findTree(const DataType& el) const
        {
            for (auto iter = m_trees.cbegin(); iter != m_trees.cend(); ++iter)
            {
                if (iter->croot().data() == el) return iter;
            }

            return m_trees.cend();
        }

        NodeIterator find(const DataType& el)
        {
            for (TreeType& tree : m_trees)
            {
                auto found = tree.find(el);
                if (found.isValid()) return found;
            }
            return {};
        }
        template <class Func>
        NodeIterator findIf(Func&& comparator)
        {
            for (TreeType& tree : m_trees)
            {
                auto found = tree.findIf(std::forward<Func>(comparator));
                if (found.isValid()) return found;
            }
            return {};
        }
        ConstNodeIterator find(const DataType& el) const
        {
            for (const TreeType& tree : m_trees)
            {
                auto found = tree.find(el);
                if (found.isValid()) return found;
            }
            return {};
        }
        template <class Func>
        ConstNodeIterator findIf(Func&& comparator) const
        {
            for (const TreeType& tree : m_trees)
            {
                auto found = tree.findIf(std::forward<Func>(comparator));
                if (found.isValid()) return found;
            }
            return {};
        }

        bool isValidTree(ConstTreeHandle h) const
        {
            return h != m_trees.cend();
        }
    };
}