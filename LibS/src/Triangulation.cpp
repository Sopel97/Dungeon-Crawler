template <class T>
Triangulation<T>::Triangulation() :
    m_isCompleted(false)
{

}
template <class T>
Triangulation<T>::~Triangulation()
{
}

template <class T>
const Mesh2<Triangle<T>>& Triangulation<T>::triangleMesh() const
{
    return m_triangleMesh;
}
template <class T>
const std::set<typename Triangulation<T>::EdgeInd>& Triangulation<T>::connections() const
{
    return m_connections;
}
template <class T>
const std::vector<typename Triangulation<T>::TriangleInd>& Triangulation<T>::triangles() const
{
    return m_triangles;
}

template <class T>
bool Triangulation<T>::isCompleted() const
{
    return m_isCompleted;
}
