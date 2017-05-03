#pragma once

#include <memory>
#include <vector>

//TODO: may require change to dynamically allocated nodes with NodeHandle being a pointer, currently there is no good way to erase a node
namespace ls
{
    template <class T>
    class BinaryTree;

    template <class T>
    class ConstBinaryTreeIterator
    {
    private:
        using ConstNodeHandle = typename BinaryTree<T>::ConstNodeHandle;

        BinaryTree<T>* m_tree;
        ConstNodeHandle m_node;
    public:
        ConstBinaryTreeIterator(BinaryTree<T>& tree, ConstNodeHandle h) :
            m_tree(&tree),
            m_node(h)
        {

        }

        ConstBinaryTreeIterator<T> left() const
        {
            return { *m_tree, m_tree->left(m_node) };
        }
        ConstBinaryTreeIterator<T> right() const
        {
            return { *m_tree, m_tree->right(m_node) };
        }
        ConstBinaryTreeIterator<T> parent() const
        {
            return { *m_tree, m_tree->parent(m_node) };
        }
        bool hasLeft() const
        {
            return m_tree->hasLeft(m_node);
        }
        bool hasRight() const
        {
            return m_tree->hasRight(m_node);
        }
        bool hasParent() const
        {
            return m_tree->hasParent(m_node);
        }
        bool isLeaf() const
        {
            return m_tree->isLeaf(m_node);
        }
        const T& data() const
        {
            return m_tree->data(m_node);
        }
        ConstNodeHandle handle() const
        {
            return m_node;
        }
    };

    template <class T>
    class BinaryTree
    {
    public:
        using DataType = T;
    private:
        struct Node
        {
            Node(Node* l, Node* r, Node* p, const DataType& d) :
                left(l),
                right(r),
                parent(p),
                data(d)
            {

            }
            Node(Node* l, Node* r, Node* p, DataType&& d) :
                left(l),
                right(r),
                parent(p),
                data(std::move(d))
            {

            }

            Node(const Node& other) = delete;
            Node(Node&& other) :
                left(other.left),
                right(other.right),
                parent(other.parent),
                data(std::move(other.data))
            {
                other.left = nullptr;
                other.right = nullptr;
            }
            Node& operator=(const Node& other) = delete;
            Node& operator=(Node&& other)
            {
                left = other.left;
                right = other.right;
                parent = other.parent;
                data = std::move(other.data);

                other.left = nullptr;
                other.right = nullptr;

                return *this;
            }

            ~Node()
            {
                if (left != nullptr) delete left;
                if (right != nullptr) delete right;
            }

            Node* left;
            Node* right;
            Node* parent;
            DataType data;
        };

        Node m_root;
    public:
        using NodeHandle = Node*;
        using ConstNodeHandle = const Node*;
        constexpr static NodeHandle invalidHandle = nullptr;

        BinaryTree(const T& rootData) :
            m_root(invalidHandle, invalidHandle, invalidHandle, rootData)
        {
        }
        BinaryTree(T&& rootData) :
            m_root(invalidHandle, invalidHandle, invalidHandle, std::move(rootData))
        {
        }
        BinaryTree(const BinaryTree<T>& other) = delete;
        BinaryTree(BinaryTree<T>&& other) :
            m_root(std::move(other.m_root))
        {
        }

        BinaryTree<T>& operator=(const BinaryTree<T>& other) = delete;

        BinaryTree<T>& operator=(BinaryTree<T>&& other)
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

        T& data(NodeHandle h)
        {
            return h->data;
        }
        const T& data(ConstNodeHandle h) const
        {
            return h->data;
        }

        NodeHandle left(NodeHandle h)
        {
            return h->left;
        }
        ConstNodeHandle left(ConstNodeHandle h) const
        {
            return h->left;
        }
        NodeHandle right(NodeHandle h)
        {
            return h->right;
        }
        ConstNodeHandle right(ConstNodeHandle h) const
        {
            return h->right;
        }
        NodeHandle parent(NodeHandle h)
        {
            return h->parent;
        }
        ConstNodeHandle parent(ConstNodeHandle h) const
        {
            return h->parent;
        }
        bool hasParent(ConstNodeHandle h) const
        {
            return h != rootHandle();
        }
        bool hasLeft(ConstNodeHandle h) const
        {
            return h->left != invalidHandle;
        }
        bool hasRight(ConstNodeHandle h) const
        {
            return h->right != invalidHandle;
        }
        bool isLeaf(ConstNodeHandle h) const
        {
            return !hasRight(h) && !hasLeft(h);
        }
        bool isValidHandle(NodeHandle h) const
        {
            return h != invalidHandle;
        }

        template <class U>
        NodeHandle insertLeft(NodeHandle h, U&& newElement)
        {
            return insert(h, &Node::left, std::forward<U>(newElement));
        }
        template <class U>
        NodeHandle insertRight(NodeHandle h, U&& newElement)
        {
            return insert(h, &Node::right, std::forward<U>(newElement));
        }
        template <class... Args>
        NodeHandle emplaceLeft(NodeHandle h, Args&&... args)
        {
            return emplace(h, &Node::left, std::forward<Args>(args)...);
        }
        template <class... Args>
        NodeHandle emplaceRight(NodeHandle h, Args&&... args)
        {
            return emplace(h, &Node::right, std::forward<Args>(args)...);
        }

        ConstBinaryTreeIterator<T> constIterator(ConstNodeHandle h)
        {
            return { *this, h };
        }

        NodeHandle find(const T& el)
        {
            find(el, rootHandle());
        }
        ConstNodeHandle find(const T& el) const
        {
            find(el, rootConstHandle());
        }
        template <class Func>
        NodeHandle findIf(Func&& comparator)
        {
            return findIf(std::forward<Func>(comparator), rootHandle());
        }
        template <class Func>
        ConstNodeHandle findIf(Func&& comparator) const
        {
            return findIf(std::forward<Func>(comparator), rootConstHandle());
        }

    private:
        NodeHandle find(const DataType& el, NodeHandle h)
        {
            return const_cast<NodeHandle>(const_cast<const BinaryTree<DataType>*>(this)->find(el, h));
        }
        template <class Func>
        NodeHandle findIf(Func&& comparator, NodeHandle h)
        {
            return const_cast<NodeHandle>(const_cast<const BinaryTree<DataType>*>(this)->findIf(std::forward<Func>(comparator), h));
        }
        ConstNodeHandle find(const DataType& el, ConstNodeHandle h) const
        {
            if (h->data == el) return h;

            if (h->left != invalidHandle)
            {
                auto foundLeft = find(el, h->left);
                if (found != invalidHandle) return found;
            }
            if (h->right != invalidHandle)
            {
                auto foundLeft = find(el, h->right);
                if (found != invalidHandle) return found;
            }

            return invalidHandle;
        }
        template <class Func>
        ConstNodeHandle findIf(Func&& comparator, ConstNodeHandle h) const
        {
            if (std::forward<Func>(comparator)(h->data)) return h;

            if (h->left != invalidHandle)
            {
                auto foundLeft = findIf(std::forward<Func>(comparator), h->left);
                if (foundLeft != invalidHandle) return foundLeft;
            }
            if (h->right != invalidHandle)
            {
                auto foundRight = findIf(std::forward<Func>(comparator), h->right);
                if (foundRight != invalidHandle) return foundRight;
            }

            return invalidHandle;
        }

        template <class U>
        NodeHandle insert(NodeHandle h, NodeHandle Node::*child, U&& newElement)
        {
            Node* newNode = new Node(invalidHandle, invalidHandle, h, std::forward<U>(newElement));
            h->*child = newNode;

            return newNode;
        }

        template <class... Args>
        NodeHandle emplace(NodeHandle h, NodeHandle Node::*child, Args&&... args)
        {
            Node* newNode = new Node(invalidHandle, invalidHandle, h, DataType(std::forward<Args>(args)...));
            h->*child = newNode;

            return newNode;
        }
    };
}