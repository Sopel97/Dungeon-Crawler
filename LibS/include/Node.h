#ifndef NODE_H
#define NODE_H

//TODO: consider using std::shared_ptr
template <class T>
class TreeNode
{
public:
    TreeNode() = default;
    TreeNode(const Vec2<T>& position);
    TreeNode(const Vec2<T>& position, TreeNode<T>* parent);
    TreeNode(const Vec2<T>& position, TreeNode<T>* parent, TreeNode<T>* child);
    TreeNode(const Vec2<T>& position, TreeNode<T>* parent, const std::vector<TreeNode<T>*>& children);
    TreeNode(const Vec2<T>& position, TreeNode<T>* parent, std::vector<TreeNode<T>*>&& children);

    TreeNode(const TreeNode<T>& other);
    TreeNode(TreeNode<T>&& other);

    void addChild(TreeNode<T>* newChild);
    void removeChild(TreeNode<T>* child);

    void setParent(TreeNode<T>* newParent);

    const Vec2<T>& position() const;
    const std::vector<TreeNode<T>*>& children() const;
    const TreeNode<T>* parent() const;
protected:
    Vec2<T> m_position;
    TreeNode<T>* m_parent;
    std::vector<TreeNode<T>*> m_children; //non owning
};

typedef TreeNode<double> TreeNodeD;
typedef TreeNode<float> TreeNodeF;
typedef TreeNode<int> TreeNodeI;



//TODO: consider using std::shared_ptr and changing name to UndirectedGraphNode
template <class T>
class GraphNode
{
public:
    GraphNode() = default;
    GraphNode(const Vec2<T>& position);
    GraphNode(const Vec2<T>& position, const std::vector<GraphNode<T>*>& children);
    GraphNode(const Vec2<T>& position, const std::vector<GraphNode<T>*>&& children);

    GraphNode(const GraphNode<T>& other);
    GraphNode(GraphNode<T>&& other);

    void addChild(GraphNode<T>* newChild);

    void removeChild(GraphNode<T>* child);

    const Vec2<T>& position() const;
    const std::vector<GraphNode<T>*>& children() const;
protected:
    Vec2<T> m_position;
    std::vector<GraphNode<T>*> m_children; //non owning
};

typedef GraphNode<double> GraphNodeD;
typedef GraphNode<float> GraphNodeF;
typedef GraphNode<int> GraphNodeI;


#include "../src/Node.cpp"

#endif // NODE_H
