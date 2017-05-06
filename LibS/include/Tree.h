#pragma once

#include <vector>
#include <utility>

namespace ls
{
    template <class DataType>
    class Tree;

    template <class DataType>
    class TreeIterator
    {
    private:
        using Node = typename Tree<DataType>::Node;

        Tree<DataType>* m_tree;
        Node* m_node;
    public:
        TreeIterator() :
            m_tree(nullptr),
            m_node(nullptr)
        {

        }
        TreeIterator(Tree<DataType>& tree, Node* h) :
            m_tree(&tree),
            m_node(h)
        {

        }
        bool operator==(const TreeIterator<DataType>& rhs) const
        {
            return m_tree == rhs.m_tree && m_node == rhs.m_node;
        }
        bool operator!=(const TreeIterator<DataType>& rhs) const
        {
            return !this->operator==(rhs);
        }

        TreeIterator<DataType> parent() const
        {
            return { *m_tree, m_node->parent };
        }
        TreeIterator<DataType> child(int i) const
        {
            return { *m_tree, m_node->children[i] };
        }
        int numberOfChildren() const
        {
            return m_node->children.size();
        }
        bool hasParent() const
        {
            return m_node->parent != nullptr;
        }
        bool isRoot() const
        {
            return !hasParent();
        }
        bool isLeaf() const
        {
            return m_node->children.empty();
        }
        DataType& data() const
        {
            return m_node->data;
        }
        const std::vector<Node*>& children() const
        {
            return m_node->children;
        }
        Node* node() const
        {
            return m_node;
        }
        Tree<DataType>& tree() const
        {
            return *m_tree;
        }
        bool isValid() const
        {
            return m_node != nullptr;
        }
    };
    template <class DataType>
    class ConstTreeIterator
    {
    private:
        using Node = typename Tree<DataType>::Node;

        const Tree<DataType>* m_tree;
        const Node* m_node;
    public:
        ConstTreeIterator() :
            m_tree(nullptr),
            m_node(nullptr)
        {

        }
        ConstTreeIterator(const Tree<DataType>& tree, const Node* h) :
            m_tree(&tree),
            m_node(h)
        {

        }
        ConstTreeIterator(const TreeIterator<DataType>& other) :
            m_tree(&(other.tree())),
            m_node(other.node())
        {

        }
        bool operator==(const ConstTreeIterator<DataType>& rhs) const
        {
            return m_tree == rhs.m_tree && m_node == rhs.m_node;
        }
        bool operator!=(const ConstTreeIterator<DataType>& rhs) const
        {
            return !this->operator==(rhs);
        }

        ConstTreeIterator<DataType> parent() const
        {
            return { *m_tree, m_node->parent };
        }
        ConstTreeIterator<DataType> child(int i) const
        {
            return { *m_tree, m_node->children[i] };
        }
        int numberOfChildren() const
        {
            return m_node->children.size();
        }
        bool hasParent() const
        {
            return m_node->parent != nullptr;
        }
        bool isRoot() const
        {
            return !hasParent();
        }
        bool isLeaf() const
        {
            return m_node->children.empty();
        }
        const DataType& data() const
        {
            return m_node->data;
        }
        const std::vector<Node*>& children() const
        {
            return m_node->children;
        }
        const Node* node() const
        {
            return m_node;
        }
        const Tree<DataType>& tree() const
        {
            return *m_tree;
        }
        bool isValid() const
        {
            return m_node != nullptr;
        }
    };

    template <class T>
    class Tree
    {
    public:
        using DataType = T;

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
            template <class... Args>
            Node(Node* p, Args&&... args) :
                parent(p),
                data(std::forward<Args>(args)...)
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
                delete toRemove;
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
        using Iterator = TreeIterator<T>;
        using ConstIterator = ConstTreeIterator<T>;
    public:

        Tree(const DataType& root) :
            m_root{ nullptr, root }
        {
        }
        Tree(DataType&& root) :
            m_root{ nullptr, std::move(root) }
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

        Iterator root()
        {
            return { *this, &m_root };
        }
        ConstIterator root() const
        {
            return { *this, &m_root };
        }
        ConstIterator croot() const
        {
            return { *this, &m_root };
        }

        template <class U>
        Iterator insertChild(Iterator h, U&& newElement)
        {
            Node* newNode = new Node( h.node(), std::forward<U>(newElement) );
            h.node()->children.emplace_back(newNode);

            return { *this, newNode };
        }
        template <class... Args>
        Iterator emplaceChild(Iterator h, Args&&... args)
        {
            Node* newNode = new Node( h.node(), std::forward<Args>(args)... );
            h.node()->children.emplace_back(newNode);

            return { *this, newNode };
        }

        //also removes all children of h
        // must not be the root
        void remove(Iterator h)
        {
            h.parent().node()->removeChild(h.node());
        }

        Iterator find(const DataType& el)
        {
            return { *this, find(el, root().node()) };
        }
        template <class Func>
        Iterator findIf(Func&& comparator)
        {
            return { *this, findIf(std::forward<Func>(comparator), root().node()) };
        }
        ConstIterator find(const DataType& el) const
        {
            return { *this, find(el, croot().node()) };
        }
        template <class Func>
        ConstIterator findIf(Func&& comparator) const
        {
            return { *this, findIf(std::forward<Func>(comparator), croot().node()) };
        }

        Iterator findChild(Iterator h, const DataType& el)
        {
            return const_cast<Iterator>(const_cast<const Tree<DataType>*>(this)->findChild(h, el));
        }
        template <class Func>
        Iterator findIfChild(Iterator h, Func&& comparator)
        {
            return const_cast<Iterator>(const_cast<const Tree<DataType>*>(this)->findIfChild(h, std::forward<Func>(comparator)));
        }
        ConstIterator findChild(ConstIterator h, const DataType& el) const
        {
            for (const auto child : h.node()->children)
            {
                if (child->data == el) return { *this, child };
            }
            return {*this, nullptr};
        }
        template <class Func>
        ConstIterator findIfChild(ConstIterator h, Func&& comparator) const
        {
            for (const auto child : h.node()->children)
            {
                if (std::forward<Func>(comparator)(child->data)) return { *this, child };
            }
            return { *this, nullptr };
        }

    private:
        Node* find(const DataType& el, Node* h)
        {
            return const_cast<Node*>(const_cast<const Tree<DataType>*>(this)->find(el, h));
        }
        template <class Func>
        Node* findIf(Func&& comparator, Node* h)
        {
            return const_cast<Node*>(const_cast<const Tree<DataType>*>(this)->findIf(std::forward<Func>(comparator), h));
        }
        const Node* find(const DataType& el, const Node* h) const
        {
            if (h->data == el) return h;
            for (const auto child : h->children)
            {
                auto found = find(el, child);
                if (found != nullptr) return found;
            }
            return nullptr;
        }
        template <class Func>
        const Node* findIf(Func&& comparator, const Node* h) const
        {
            if (std::forward<Func>(comparator)(h->data)) return h;
            for (const auto child : h->children)
            {
                auto found = findIf(std::forward<Func>(comparator), child);
                if (found != nullptr) return found;
            }
            return nullptr;
        }
    };
}