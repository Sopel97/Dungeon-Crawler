#pragma once

#include <vector>
#include <utility>

namespace ls
{
    template <class DataType>
    class Tree;

    template <class DataType>
    class ConstTreeIterator
    {
    private:
        using ConstNodeHandle = typename Tree<DataType>::ConstNodeHandle;

        const Tree<DataType>* m_tree;
        ConstNodeHandle m_node;
    public:
        ConstTreeIterator(const Tree<DataType>& tree, ConstNodeHandle h) :
            m_tree(&tree),
            m_node(h)
        {

        }

        ConstTreeIterator<DataType> parent() const
        {
            return { *m_tree, m_tree->parent(m_node) };
        }
        ConstTreeIterator<DataType> child(int i) const
        {
            return { *m_tree, m_tree->child(m_node, i) };
        }
        int numberOfChildren() const
        {
            return m_tree->numberOfChildren(m_node);
        }
        bool hasParent() const
        {
            return m_tree->hasParent(m_node);
        }
        bool isLeaf() const
        {
            return m_tree->isLeaf(m_node);
        }
        const DataType& data() const
        {
            return m_tree->data(m_node);
        }
        ConstNodeHandle handle() const
        {
            return m_node;
        }
    };

    template <class T>
    class Tree
    {
    public:
        using DataType = T;
    private:
        struct Node
        {
            Node(Node* p, const DataType& d) :
                parent(p),
                data(d)
            {

            }
            Node(Node* p, DataType&& d) :
                parent(p),
                data(std::move(d))
            {

            }
            Node(const Node& other) = delete;
            Node(Node&& other) :
                parent(other.parent),
                children(std::move(other.children)),
                data(std::move(other.data))
            {
                other.children.clear();
            }
            Node& operator=(const Node& other) = delete;
            Node& operator=(Node&& other)
            {
                parent = other.parent;
                children = std::move(other.children);
                data = std::move(other.data);

                other.children.clear();

                return *this;
            }

            void removeChild(Node* toRemove)
            {
                for (auto& child : children)
                {
                    if (child == toRemove)
                    {
                        child = children.back();
                        children.pop_back();
                    }
                }
            }

            ~Node()
            {
                for (auto node : children)
                {
                    delete node;
                }
            }

            Node* parent;
            std::vector<Node*> children;
            DataType data;
        };

        Node m_root;
    public:
        using NodeHandle = Node*;
        using ConstNodeHandle = const Node*;
        constexpr static NodeHandle invalidHandle = nullptr;
    public:

        Tree(const DataType& root) :
            m_root{ invalidHandle, root }
        {
        }
        Tree(DataType&& root) :
            m_root{ invalidHandle, std::move(root) }
        {
        }
        Tree(const Tree<DataType>& other) = delete;
        Tree(Tree<DataType>&& other) :
            m_root(std::move(other.m_root))
        {
        }

        Tree<DataType>& operator=(const Tree<DataType>& other) = delete;
        Tree<DataType>& operator=(Tree<DataType>&& other)
        {
            m_root = std::move(other.m_root);

            return *this;
        }

        NodeHandle rootHandle()
        {
            return &m_root;
        }
        ConstNodeHandle rootHandle() const
        {
            return &m_root;
        }
        ConstNodeHandle rootConstHandle() const
        {
            return &m_root;
        }

        DataType& data(NodeHandle h)
        {
            return h->data;
        }
        const DataType& data(ConstNodeHandle h) const
        {
            return h->data;
        }

        ConstNodeHandle parent(ConstNodeHandle h) const
        {
            return h->parent;
        }
        bool hasParent(ConstNodeHandle h) const
        {
            //only root does not have a parent
            return h != rootHandle();
        }
        bool isLeaf(ConstNodeHandle h) const
        {
            return h->childen.empty();
        }

        template <class U>
        NodeHandle insertChild(NodeHandle h, U&& newElement)
        {
            Node* newNode = new Node( h, std::forward<U>(newElement) );
            h->children.emplace_back(newNode);

            return newNode;
        }
        template <class... Args>
        NodeHandle emplaceChild(NodeHandle h, Args&&... args)
        {
            Node* newNode = new Node( h, DataType(std::forward<Args>(args)...) );
            h->children.emplace_back(newNode);

            return newNode;
        }

        //also removes all children of h
        void remove(NodeHandle h)
        {
            h->parent.removeChild(h);
            delete h;
        }

        int numberOfChildren(ConstNodeHandle h) const
        {
            return h->children.size();
        }
        ConstNodeHandle child(ConstNodeHandle h, int i) const
        {
            return h->children[i];
        }
        NodeHandle child(NodeHandle h, int i)
        {
            return h->children[i];
        }

        ConstTreeIterator<DataType> constIterator(ConstNodeHandle h)
        {
            return { *this, h };
        }

        NodeHandle find(const DataType& el)
        {
            return find(el, rootHandle());
        }
        template <class Func>
        NodeHandle findIf(Func&& comparator)
        {
            return findIf(std::forward<Func>(comparator), rootHandle());
        }
        ConstNodeHandle find(const DataType& el) const
        {
            return find(el, rootConstHandle());
        }
        template <class Func>
        ConstNodeHandle findIf(Func&& comparator) const
        {
            return findIf(std::forward<Func>(comparator), rootConstHandle());
        }

        NodeHandle findChild(NodeHandle h, const DataType& el)
        {
            return const_cast<NodeHandle>(const_cast<const Tree<DataType>*>(this)->findChild(h, el));
        }
        template <class Func>
        NodeHandle findIfChild(NodeHandle h, Func&& comparator)
        {
            return const_cast<NodeHandle>(const_cast<const Tree<DataType>*>(this)->findIfChild(h, std::forward<Func>(comparator)));
        }
        ConstNodeHandle findChild(ConstNodeHandle h, const DataType& el) const
        {
            for (const auto child : h->children)
            {
                if (child->data == el) return child;
            }
            return invalidHandle;
        }
        template <class Func>
        ConstNodeHandle findIfChild(ConstNodeHandle h, Func&& comparator) const
        {
            for (const auto child : h->children)
            {
                if (std::forward<Func>(comparator)(child->data)) return child;
            }
            return invalidHandle;
        }
        bool isValidHandle(ConstNodeHandle h) const
        {
            return h != invalidHandle;
        }

    private:
        NodeHandle find(const DataType& el, NodeHandle h)
        {
            return const_cast<NodeHandle>(const_cast<const Tree<DataType>*>(this)->find(el, h));
        }
        template <class Func>
        NodeHandle findIf(Func&& comparator, NodeHandle h)
        {
            return const_cast<NodeHandle>(const_cast<const Tree<DataType>*>(this)->findIf(std::forward<Func>(comparator), h));
        }
        ConstNodeHandle find(const DataType& el, ConstNodeHandle h) const
        {
            if (h->data == el) return h;
            for (const auto child : h->children)
            {
                auto found = find(el, child);
                if (found != invalidHandle) return found;
            }
            return invalidHandle;
        }
        template <class Func>
        ConstNodeHandle findIf(Func&& comparator, ConstNodeHandle h) const
        {
            if (std::forward<Func>(comparator)(h->data)) return h;
            for (const auto child : h->children)
            {
                auto found = findIf(std::forward<Func>(comparator), child);
                if (found != invalidHandle) return found;
            }
            return invalidHandle;
        }
    };
}