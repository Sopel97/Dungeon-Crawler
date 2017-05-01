#pragma once

#include <memory>
#include <vector>

namespace ls
{
	template <class T>
	class BinaryTree;

	template <class T>
	class BinaryTreeIterator
	{
	private:
		using NodeHandle = typename BinaryTree<T>::NodeHandle;

		BinaryTree<T>* m_tree;
		NodeHandle m_node;
	public:
		BinaryTreeIterator(BinaryTree<T>& tree, NodeHandle h) :
			m_tree(&tree),
			m_node(h)
		{

		}

		BinaryTreeIterator<T> left()
		{
			return { *m_tree, m_tree->left(m_node) };
		}
		BinaryTreeIterator<T> right()
		{
			return { *m_tree, m_tree->right(m_node) };
		}
		BinaryTreeIterator<T> parent()
		{
			return { *m_tree, m_tree->parent(m_node) };
		}
		bool hasLeft()
		{
			return m_tree->hasLeft(m_node);
		}
		bool hasRight()
		{
			return m_tree->hasRight(m_node);
		}
		bool hasParent()
		{
			return m_tree->hasParent(m_node);
		}
		bool isLeaf()
		{
			return m_tree->isLeaf(m_node);
		}
		const T& data() const
		{
			return m_tree->node(m_node);
		}
		T& data()
		{
			return m_tree->node(m_node);
		}
        NodeHandle handle() const
        {
            return m_node;
        }
	};

	template <class T>
	class BinaryTree
	{
	public:
		using NodeHandle = int;
		constexpr static NodeHandle invalidHandle = -1;
	private:
		struct NodeConnections
		{
			NodeConnections(NodeHandle l, NodeHandle r, NodeHandle p) :
				left(l),
				right(r),
				parent(p)
			{

			}

			NodeHandle left;
			NodeHandle right;
			NodeHandle parent;
		};

		std::vector<T> m_nodes;
		std::vector<NodeConnections> m_nodesConnections;
	public:

		BinaryTree(const T& root) :
			m_nodes{},
			m_nodesConnections{}
		{
			m_nodes.push_back(root);
			m_nodesConnections.emplace_back(invalidHandle, invalidHandle, invalidHandle);
		}
		BinaryTree(T&& root) :
			m_nodes{},
			m_nodesConnections{}
		{
			m_nodes.push_back(std::move(root));
			m_nodesConnections.emplace_back(invalidHandle, invalidHandle, invalidHandle);
		}
		BinaryTree(const BinaryTree<T>& other) :
			m_nodes(other.m_nodes),
			m_nodesConnections(other.m_nodesConnections)
		{
		}
		BinaryTree(BinaryTree<T>&& other) :
			m_nodes(std::move(other.m_nodes)),
			m_nodesConnections(std::move(other.m_nodesConnections))
		{
		}

		BinaryTree<T>& operator=(const BinaryTree<T>& other)
		{
			m_nodes = other.m_nodes;
			m_nodesConnections = other.m_nodesConnections;

			return *this;
		}

		BinaryTree<T>& operator=(BinaryTree<T>&& other)
		{
			m_nodes = std::move(other.m_nodes);
			m_nodesConnections = std::move(other.m_nodesConnections);

			return *this;
		}
				
		NodeHandle rootHandle() const
		{
			return 0;
		}

		T& node(NodeHandle h)
		{
			return m_nodes[h];
		}
		const T& node(NodeHandle h) const
		{
			return m_nodes[h];
		}

		NodeHandle left(NodeHandle h) const
		{
			return m_nodesConnections[h].left;
		}
		NodeHandle right(NodeHandle h) const
		{
			return m_nodesConnections[h].right;
		}
		NodeHandle parent(NodeHandle h) const
		{
			return m_nodesConnections[h].parent;
		}
		bool hasParent(NodeHandle h) const
		{
			//only root does not have a parent
			return h != 0;
		}
		bool hasLeft(NodeHandle h) const
		{
			return left(h) != invalidHandle;
		}
		bool hasRight(NodeHandle h) const
		{
			return right(h) != invalidHandle;
		}
		bool isLeaf(NodeHandle h) const
		{
			return !hasRight(h) && !hasLeft(h);
		}
		bool isValidHandle(NodeHandle h) const
		{
			return h >= 0 && h < m_nodes.size();
		}

		template <class U>
		NodeHandle insertLeft(NodeHandle h, U&& newElement)
		{
			return insert(h, &NodeConnections::left, std::forward<U>(newElement));
		}
		template <class U>
		NodeHandle insertRight(NodeHandle h, U&& newElement)
		{
			return insert(h, &NodeConnections::right, std::forward<U>(newElement));
		}
		template <class... Args>
		NodeHandle emplaceLeft(NodeHandle h, Args&&... args)
		{
			return emplace(h, &NodeConnections::left, std::forward<Args>(args)...);
		}
		template <class... Args>
		NodeHandle emplaceRight(NodeHandle h, Args&&... args)
		{
			return emplace(h, &NodeConnections::right, std::forward<Args>(args)...);
		}

		BinaryTreeIterator<T> iterator(NodeHandle h)
		{
			return { *this, h };
		}

		NodeHandle find(const T& el) const
		{
			const int size = m_nodes.size();
			for (int i = 0; i < size; ++i)
			{
				if (m_nodes[i] == el) return i;
			}

			return invalidHandle;
		}
		template <class Func>
		NodeHandle findIf(Func&& comparator) const
		{
			const int size = m_nodes.size();
			for (int i = 0; i < size; ++i)
			{
				if (std::forward<Func>(comparator)(m_nodes[i])) return i;
			}

			return invalidHandle;
		}

	private:

		template <class U>
		NodeHandle insert(NodeHandle h, NodeHandle NodeConnections::*child, U&& newElement)
		{
			m_nodes.push_back(std::forward<U>(newElement));
			m_nodesConnections.emplace_back(invalidHandle, invalidHandle, h);
			NodeHandle newH = m_nodes.size() - 1;

			NodeConnections& connections = m_nodesConnections.back();
			NodeConnections& parentConnections = m_nodesConnections[h];

			connections.*child = parentConnections.*child;
			parentConnections.*child = newH;

			return newH;
		}

		template <class... Args>
		NodeHandle emplace(NodeHandle h, NodeHandle NodeConnections::*child, Args&&... args)
		{
			m_nodes.emplace_back(std::forward<Args>(args)...);
			m_nodesConnections.emplace_back(invalidHandle, invalidHandle, h);
			NodeHandle newH = m_nodes.size() - 1;

			NodeConnections& connections = m_nodesConnections.back();
			NodeConnections& parentConnections = m_nodesConnections[h];

			connections.*child = parentConnections.*child;
			parentConnections.*child = newH;

			return newH;
		}
	};
}