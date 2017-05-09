#pragma once

#include <memory>
#include <vector>
#include <functional>

//TODO: may require change to dynamically allocated nodes with NodeHandle being a pointer, currently there is no good way to erase a node
namespace ls
{
    template <class T>
    class BinaryTree;

    namespace detail
    {
        template <class DataType>
        struct BinaryTreeNode
        {
            template <class... Args>
            BinaryTreeNode(BinaryTreeNode* l, BinaryTreeNode* r, BinaryTreeNode* p, Args&&... args) :
                left(l),
                right(r),
                parent(p),
                data(std::forward<Args>(args)...)
            {

            }

            BinaryTreeNode(const BinaryTreeNode& other) = delete;
            BinaryTreeNode(BinaryTreeNode&& other) :
                left(other.left),
                right(other.right),
                parent(other.parent),
                data(std::move(other.data))
            {
                other.left = nullptr;
                other.right = nullptr;
            }
            BinaryTreeNode& operator=(const BinaryTreeNode& other) = delete;
            BinaryTreeNode& operator=(BinaryTreeNode&& other)
            {
                left = other.left;
                right = other.right;
                parent = other.parent;
                data = std::move(other.data);

                other.left = nullptr;
                other.right = nullptr;

                return *this;
            }

            void removeChild(BinaryTreeNode* toRemove)
            {
                if (left == node)
                {
                    delete left;
                    left = nullptr;
                }
                else if (right == node)
                {
                    delete right;
                    right = nullptr;
                }
            }

            ~BinaryTreeNode()
            {
                if (left != nullptr) delete left;
                if (right != nullptr) delete right;
            }

            BinaryTreeNode* left;
            BinaryTreeNode* right;
            BinaryTreeNode* parent;
            DataType data;
        };

        template <class DataType, class NodeType>
        class BinaryTreeIteratorPrototype
        {
        private:
            template <class T>
            friend class BinaryTree;
            template <class DataType, class NodeType>
            friend class BinaryTreeIteratorPrototype;

            NodeType* m_node;

        public:
            BinaryTreeIteratorPrototype() :
                m_node(nullptr)
            {

            }
            BinaryTreeIteratorPrototype(const BinaryTreeIteratorPrototype<typename std::remove_const<DataType>::type, typename std::remove_const<NodeType>::type>& h) :
                m_node(h.m_node)
            {

            }
            explicit BinaryTreeIteratorPrototype(NodeType* h) :
                m_node(h)
            {

            }

            bool operator==(const BinaryTreeIteratorPrototype<DataType, NodeType>& rhs) const
            {
                return m_node == rhs.m_node;
            }
            bool operator!=(const BinaryTreeIteratorPrototype<DataType, NodeType>& rhs) const
            {
                return !this->operator==(rhs);
            }
            bool operator<(const BinaryTreeIteratorPrototype<DataType, NodeType>& rhs) const
            {
                return std::less<>()(m_node, rhs, rhs.m_node);
            }

            BinaryTreeIteratorPrototype<DataType, NodeType> left() const
            {
                return BinaryTreeIteratorPrototype<DataType, NodeType>(m_node->left);
            }
            BinaryTreeIteratorPrototype<DataType, NodeType> right() const
            {
                return BinaryTreeIteratorPrototype<DataType, NodeType>(m_node->right);
            }
            BinaryTreeIteratorPrototype<DataType, NodeType> parent() const
            {
                return BinaryTreeIteratorPrototype<DataType, NodeType>(m_node->parent);
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
            bool isRoot() const
            {
                return !hasParent();
            }
            bool isLeaf() const
            {
                return !hasLeft() && !hasRight();
            }
            DataType& data() const
            {
                return m_node->data;
            }
            bool isValid() const
            {
                return m_node != nullptr;
            }
        };

        template <class DataType>
        using BinaryTreeIterator = BinaryTreeIteratorPrototype<DataType, BinaryTreeNode<DataType>>;
        template <class DataType>
        using ConstBinaryTreeIterator = BinaryTreeIteratorPrototype<const DataType, const BinaryTreeNode<DataType>>;
    }

    template <class T>
    class BinaryTree
    {
    private:
        using Node = detail::BinaryTreeNode<T>;

        Node* m_root;

    public:
        using DataType = T;
        using Iterator = detail::BinaryTreeIterator<T>;
        using ConstIterator = detail::ConstBinaryTreeIterator<T>;

        BinaryTree() :
            m_root(nullptr)
        {

        }
        template <class... Args>
        BinaryTree(Args&&... args) :
            m_root(new Node(nullptr, nullptr, nullptr, std::forward<Args>(args)...))
        {
        }
        BinaryTree(const BinaryTree<T>& other) = delete;
        BinaryTree(BinaryTree<T>&& other) :
            m_root(other.m_root)
        {
            other.m_root = nullptr;
        }
        ~BinaryTree()
        {
            if (m_root != nullptr) delete m_root;
        }

        BinaryTree<T>& operator=(const BinaryTree<T>& other) = delete;

        BinaryTree<T>& operator=(BinaryTree<T>&& other)
        {
            m_root = other.m_root;
            other.m_root = nullptr;

            return *this;
        }

        Iterator root()
        {
            return Iterator(m_root);
        }
        ConstIterator root() const
        {
            return ConstIterator(m_root);
        }
        ConstIterator croot() const
        {
            return ConstIterator(m_root);
        }

        template <class... Args>
        Iterator emplaceLeft(Iterator h, Args&&... args)
        {
            return Iterator(emplace(h.m_node, &Node::left, std::forward<Args>(args)...));
        }
        template <class... Args>
        Iterator emplaceRight(Iterator h, Args&&... args)
        {
            return Iterator(emplace(h.m_node, &Node::right, std::forward<Args>(args)...));
        }

        void remove(Iterator h)
        {
            if (h.hasParent())
            {
                Node* node = h.m_node;
                Node* parent = h.parent().m_node;
                parent->removeChild(node);
            }
            else
            {
                delete m_root;
                m_root = nullptr;
            }
        }

        bool isEmpty() const
        {
            return m_root == nullptr;
        }

        Iterator find(const T& el)
        {
            return Iterator(find(el, m_root));
        }
        ConstIterator find(const T& el) const
        {
            return ConstIterator(find(el, m_root));
        }
        template <class Func>
        Iterator findIf(Func&& comparator)
        {
            return Iterator(findIf(std::forward<Func>(comparator), m_root));
        }
        template <class Func>
        ConstIterator findIf(Func&& comparator) const
        {
            return ConstIterator(findIf(std::forward<Func>(comparator), m_root));
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
        template <class... Args>
        Node* emplace(Node* h, Node* Node::*child, Args&&... args)
        {
            Node* newNode = new Node(nullptr, nullptr, h, std::forward<Args>(args)...);
            newNode->*child = h->*child; // if this node was already used then push it further down
            h->*child = newNode;

            return newNode;
        }
    };
}