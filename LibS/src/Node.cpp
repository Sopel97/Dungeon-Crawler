template <class T>
TreeNode<T>::TreeNode(const Vec2<T>& position) :
    m_position(position),
    m_parent(nullptr)
{

}
template <class T>
TreeNode<T>::TreeNode(const Vec2<T>& position, TreeNode<T>* parent) :
    m_position(position),
    m_parent(parent)
{

}
template <class T>
TreeNode<T>::TreeNode(const Vec2<T>& position, TreeNode<T>* parent, TreeNode<T>* child) :
    m_position(position),
    m_parent(parent),
    m_children({child})
{

}
template <class T>
TreeNode<T>::TreeNode(const Vec2<T>& position, TreeNode<T>* parent, const std::vector<TreeNode<T>*>& children) :
    m_position(position),
    m_parent(parent),
    m_children(children)
{

}
template <class T>
TreeNode<T>::TreeNode(const Vec2<T>& position, TreeNode<T>* parent, std::vector<TreeNode<T>*>&& children) :
    m_position(position),
    m_children(std::move(children)),
    m_parent(parent)
{

}
template <class T>
TreeNode<T>::TreeNode(const TreeNode<T>& other) :
    m_position(other.m_position),
    m_parent(other.m_parent),
    m_children(other.m_children)
{

}
template <class T>
TreeNode<T>::TreeNode(TreeNode<T>&& other) :
    m_position(std::move(other.m_position)),
    m_parent(other.m_parent),
    m_children(std::move(other.m_children))
{

}

template <class T>
void TreeNode<T>::addChild(TreeNode<T>* newChild)
{
    m_children.push_back(newChild);
}

template <class T>
void TreeNode<T>::removeChild(TreeNode<T>* child)
{
    m_children.erase(std::remove(std::begin(m_children), std::end(m_children), child), std::end(m_children));
}

template <class T>
void TreeNode<T>::setParent(TreeNode<T>* newParent)
{
    m_parent = newParent;
}
template <class T>
const Vec2<T>& TreeNode<T>::position() const
{
    return m_position;
}
template <class T>
const std::vector<TreeNode<T>*>& TreeNode<T>::children() const
{
    return m_children;
}

template <class T>
const TreeNode<T>* TreeNode<T>::parent() const
{
    return m_parent;
}



template <class T>
GraphNode<T>::GraphNode(const Vec2<T>& position) :
    m_position(position)
{

}
template <class T>
GraphNode<T>::GraphNode(const Vec2<T>& position, const std::vector<GraphNode<T>*>& children) :
    m_position(position),
    m_children(children)
{

}
template <class T>
GraphNode<T>::GraphNode(const Vec2<T>& position, const std::vector<GraphNode<T>*>&& children) :
    m_position(position),
    m_children(std::move(children))
{

}
template <class T>
GraphNode<T>::GraphNode(const GraphNode<T>& other) :
    m_position(other.m_position),
    m_children(other.m_children)
{

}
template <class T>
GraphNode<T>::GraphNode(GraphNode<T>&& other) :
    m_position(std::move(other.m_position)),
    m_children(std::move(other.m_children))
{

}

template <class T>
void GraphNode<T>::addChild(GraphNode<T>* newChild)
{
    m_children.push_back(newChild);
}
template <class T>
void GraphNode<T>::removeChild(GraphNode<T>* child)
{
    m_children.erase(std::remove(std::begin(m_children), std::end(m_children), child), std::end(m_children));
}

template <class T>
const Vec2<T>& GraphNode<T>::position() const
{
    return m_position;
}
template <class T>
const std::vector<GraphNode<T>*>& GraphNode<T>::children() const
{
    return m_children;
}
