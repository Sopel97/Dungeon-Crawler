#pragma once

#include <memory>
#include <vector>

//TODO: may require change to dynamically allocated nodes with NodeHandle being a pointer, currently there is no good way to erase a node
namespace ls
{
    template <class T>
    class BinaryTree;

    template <class T>
    class BinaryTreeIterator
    {
    private:
        using Node = typename BinaryTree<T>::Node;

        BinaryTree<T>* m_tree;
        Node* m_node;

    public:
        BinaryTreeIterator() :
            m_tree(nullptr),
            m_node(nullptr)
        {

        }
        BinaryTreeIterator(BinaryTree<T>& tree, Node* h) :
            m_tree(&tree),
            m_node(h)
        {

        }

        bool operator==(const BinaryTreeIterator<T>& rhs) const
        {
            return m_tree == rhs.m_tree && m_node == rhs.m_node;
        }
        bool operator!=(const BinaryTreeIterator<T>& rhs) const
        {
            return !this->operator==(rhs);
        }

        BinaryTreeIterator<T> left() const
        {
            return { *m_tree, m_node->left };
        }
        BinaryTreeIterator<T> right() const
        {
            return { *m_tree, m_node->right };
        }
        BinaryTreeIterator<T> parent() const
        {
            return { *m_tree, m_node->parent };
        }
        bool hasLeft() const
        {
            return m_node->left != nullptr;
        }
        bool hasRight() const
        {
            return m_node->right != nullptr;
        }
        bool hasParent() const
        {
            return m_node->parent != nullptr;
        }
        bool isLeaf() const
        {
            return !hasLeft() && !hasRight();
        }
        T& data() const
        {
            return m_node->data;
        }
        Node* node() const
        {
            return m_node;
        }
        BinaryTree<T>& tree() const
        {
            return *m_tree;
        }
        bool isValid() const
        {
            return m_node != nullptr;
        }
    };

    template <class T>
    class ConstBinaryTreeIterator
    {
    private:
        using Node = typename BinaryTree<T>::Node;

        const BinaryTree<T>* m_tree;
        const Node* m_node;

    public:
        ConstBinaryTreeIterator() :
            m_tree(nullptr),
            m_node(nullptr)
        {

        }
        ConstBinaryTreeIterator(const BinaryTree<T>& tree, const Node* h) :
            m_tree(&tree),
            m_node(h)
        {

        }
        ConstBinaryTreeIterator(const BinaryTreeIterator<T>& other) :
            m_tree(&(other.tree())),
            m_node(other.node())
        {

        }

        bool operator==(const ConstBinaryTreeIterator<T>& rhs) const
        {
            return m_tree == rhs.m_tree && m_node == rhs.m_node;
        }
        bool operator!=(const ConstBinaryTreeIterator<T>& rhs) const
        {
            return !this->operator==(rhs);
        }

        ConstBinaryTreeIterator<T> left() const
        {
            return { *m_tree, m_node->left };
        }
        ConstBinaryTreeIterator<T> right() const
        {
            return { *m_tree, m_node->right };
        }
        ConstBinaryTreeIterator<T> parent() const
        {
            return { *m_tree, m_node->parent };
        }
        bool hasLeft() const
        {
            return m_node->left != nullptr;
        }
        bool hasRight() const
        {
            return m_node->right != nullptr;
        }
        bool hasParent() const
        {
            return m_node->parent != nullptr;
        }
        bool isLeaf() const
        {
            return !hasLeft() && !hasRight();
        }
        const T& data() const
        {
            return m_node->data;
        }
        const Node* node() const
        {
            return m_node;
        }
        const BinaryTree<T>& tree() const
        {
            return *m_tree;
        }
        bool isValid() const
        {
            return m_node != nullptr;
        }
    };

    template <class T>
    class BinaryTree
    {
    public:
        using DataType = T;

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
    private:
        Node m_root;

    public:
        using Iterator = BinaryTreeIterator<T>;
        using ConstIterator = ConstBinaryTreeIterator<T>;

        BinaryTree(const T& rootData) :
            m_root(nullptr, nullptr, nullptr, rootData)
        {
        }
        BinaryTree(T&& rootData) :
            m_root(nullptr, nullptr, nullptr, std::move(rootData))
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
        Iterator insertLeft(Iterator h, U&& newElement)
        {
            return { *this, insert(h.node(), &Node::left, std::forward<U>(newElement)) };
        }
        template <class U>
        Iterator insertRight(Iterator h, U&& newElement)
        {
            return { *this, insert(h.node(), &Node::right, std::forward<U>(newElement)) };
        }
        template <class... Args>
        Iterator emplaceLeft(Iterator h, Args&&... args)
        {
            return { *this, emplace(h.node(), &Node::left, std::forward<Args>(args)...) };
        }
        template <class... Args>
        Iterator emplaceRight(Iterator h, Args&&... args)
        {
            return { *this, emplace(h.node(), &Node::right, std::forward<Args>(args)...) };
        }

        Iterator find(const T& el)
        {
            return { *this, find(el, root().node()) };
        }
        ConstIterator find(const T& el) const
        {
            return { *this, find(el, croot().node()) };
        }
        template <class Func>
        Iterator findIf(Func&& comparator)
        {
            return { *this, findIf(std::forward<Func>(comparator), root().node()) };
        }
        template <class Func>
        ConstIterator findIf(Func&& comparator) const
        {
            return { *this, findIf(std::forward<Func>(comparator), croot().node()) };
        }

    private:
        Node* find(const DataType& el, Node* h)
        {
            return const_cast<Node*>(const_cast<const BinaryTree<DataType>*>(this)->find(el, h));
        }
        template <class Func>
        Node* findIf(Func&& comparator, Node* h)
        {
            return const_cast<Node*>(const_cast<const BinaryTree<DataType>*>(this)->findIf(std::forward<Func>(comparator), h));
        }
        const Node* find(const DataType& el, const Node* h) const
        {
            if (h->data == el) return h;

            if (h->left != nullptr)
            {
                auto foundLeft = find(el, h->left);
                if (foundLeft != nullptr) return foundLeft;
            }
            if (h->right != nullptr)
            {
                auto foundRight = find(el, h->right);
                if (foundRight != nullptr) return foundRight;
            }

            return nullptr;
        }
        template <class Func>
        const Node* findIf(Func&& comparator, const Node* h) const
        {
            if (std::forward<Func>(comparator)(h->data)) return h;

            if (h->left != nullptr)
            {
                auto foundLeft = findIf(std::forward<Func>(comparator), h->left);
                if (foundLeft != nullptr) return foundLeft;
            }
            if (h->right != nullptr)
            {
                auto foundRight = findIf(std::forward<Func>(comparator), h->right);
                if (foundRight != nullptr) return foundRight;
            }

            return nullptr;
        }

        template <class U>
        Node* insert(Node* h, Node* Node::*child, U&& newElement)
        {
            Node* newNode = new Node(nullptr, nullptr, h, std::forward<U>(newElement));
            h->*child = newNode;

            return newNode;
        }

        template <class... Args>
        Node* emplace(Node* h, Node* Node::*child, Args&&... args)
        {
            Node* newNode = new Node(nullptr, nullptr, h, DataType(std::forward<Args>(args)...));
            h->*child = newNode;

            return newNode;
        }
    };
}