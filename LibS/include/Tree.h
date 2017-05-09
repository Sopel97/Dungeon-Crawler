#pragma once

#include <vector>
#include <utility>

namespace ls
{
    template <class DataType>
    class Tree;

    namespace detail
    {
        template <class DataType>
        struct TreeNode
        {
            template <class... Args>
            TreeNode(TreeNode* p, Args&&... args) :
                parent(p),
                data(std::forward<Args>(args)...)
            {

            }

            TreeNode(const TreeNode& other) = delete;
            TreeNode(TreeNode&& other) :
                parent(other.parent),
                children(std::move(other.children)),
                data(std::move(other.data))
            {
                other.children.clear();
            }
            TreeNode& operator=(const TreeNode& other) = delete;
            TreeNode& operator=(TreeNode&& other)
            {
                parent = other.parent;
                children = std::move(other.children);
                data = std::move(other.data);

                other.children.clear();

                return *this;
            }

            void detachChild(TreeNode* toDetach)
            {
                for (auto& child : children)
                {
                    if (child == toDetach)
                    {
                        child = children.back();
                        children.pop_back();
                        break;
                    }
                }
            }
            void removeChild(TreeNode* toRemove)
            {
                detachChild(toRemove);
                delete toRemove;
            }

            ~TreeNode()
            {
                for (auto node : children)
                {
                    delete node;
                }
            }

            TreeNode* parent;
            std::vector<TreeNode*> children;
            DataType data;
        };

        template <class DataType, class NodeType>
        class TreeIteratorPrototype
        {
        private:
            template <class T>
            friend class Tree;

            NodeType* m_node;
        public:
            TreeIteratorPrototype() :
                m_node(nullptr)
            {

            }
            explicit TreeIteratorPrototype(NodeType* h) :
                m_node(h)
            {

            }
            bool operator==(const TreeIteratorPrototype<DataType, NodeType>& rhs) const
            {
                return m_node == rhs.m_node;
            }
            bool operator!=(const TreeIteratorPrototype<DataType, NodeType>& rhs) const
            {
                return !this->operator==(rhs);
            }

            TreeIteratorPrototype<DataType, NodeType> parent() const
            {
                return TreeIteratorPrototype<DataType, NodeType>(m_node->parent);
            }
            TreeIteratorPrototype<DataType, NodeType> child(int i) const
            {
                return TreeIteratorPrototype<DataType, NodeType>(m_node->children[i]);
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
            bool isValid() const
            {
                return m_node != nullptr;
            }
        };

        template <class DataType>
        using TreeIterator = TreeIteratorPrototype<DataType, TreeNode<DataType>>;
        template <class DataType>
        using ConstTreeIterator = TreeIteratorPrototype<const DataType, const TreeNode<DataType>>;
    }

    template <class T>
    class Tree
    {
    private:
        using Node = detail::TreeNode<T>;

        Node* m_root;
    public:
        using DataType = T;
        
        using Iterator = detail::TreeIterator<T>;
        using ConstIterator = detail::ConstTreeIterator<T>;
    public:

        Tree() :
            m_root(nullptr)
        {
        }
        template <class... Args>
        Tree(Args&&... args) :
            m_root(new Node(nullptr, std::forward<Args>(args)...))
        {
        }
        Tree(const Tree<DataType>& other) = delete;
        Tree(Tree<DataType>&& other) :
            m_root(other.m_root)
        {
            other.m_root = nullptr;
        }
        ~Tree()
        {
            if (m_root != nullptr) delete m_root;
        }

        Tree<DataType>& operator=(const Tree<DataType>& other) = delete;
        Tree<DataType>& operator=(Tree<DataType>&& other)
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
        Iterator emplaceChild(Iterator h, Args&&... args)
        {
            Node* newNode = new Node( h.m_node, std::forward<Args>(args)... );
            h.m_node->children.emplace_back(newNode);

            return Iterator(newNode);
        }

        Tree<DataType> detach(Iterator h)
        {
            if (h.hasParent())
            {
                h.parent().m_node->detachChild(h.m_node);
                h.m_node->parent = nullptr;
            }
            else
            {
                m_root = nullptr;
            }

            return Tree<DataType>(h.m_node);
        }

        Iterator attach(Iterator h, Tree<DataType>&& tree)
        {
            Node* treeRoot = tree.m_root;
            tree.m_root = nullptr;

            h.m_node->children.push_back(treeRoot);
            treeRoot->parent = h.m_node;

            return Iterator(treeRoot);
        }

        void remove(Iterator h)
        {
            if (h.hasParent())
            {
                h.parent().m_node->removeChild(h.m_node);
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

        Iterator find(const DataType& el)
        {
            return Iterator(find(el, m_root));
        }
        template <class Func>
        Iterator findIf(Func&& comparator)
        {
            return Iterator(findIf(std::forward<Func>(comparator), m_root));
        }
        ConstIterator find(const DataType& el) const
        {
            return ConstIterator(find(el, m_root));
        }
        template <class Func>
        ConstIterator findIf(Func&& comparator) const
        {
            return ConstIterator(findIf(std::forward<Func>(comparator), m_root));
        }

        Iterator findChild(Iterator h, const DataType& el)
        {
            for (auto child : h.m_node->children)
            {
                if (child->data == el) return Iterator(child);
            }
            return Iterator(nullptr);
        }
        template <class Func>
        Iterator findIfChild(Iterator h, Func&& comparator)
        {
            for (auto child : h.m_node->children)
            {
                if (std::forward<Func>(comparator)(child->data)) return Iterator(child);
            }
            return Iterator(nullptr);
        }
        ConstIterator findChild(ConstIterator h, const DataType& el) const
        {
            for (const auto child : h.m_node->children)
            {
                if (child->data == el) return ConstIterator(child);
            }
            return ConstIterator(nullptr);
        }
        template <class Func>
        ConstIterator findIfChild(ConstIterator h, Func&& comparator) const
        {
            for (const auto child : h.m_node->children)
            {
                if (std::forward<Func>(comparator)(child->data)) return ConstIterator(child);
            }
            return ConstIterator(nullptr);
        }

    private:
        Tree(Node* root) :
            m_root(root)
        {

        }

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