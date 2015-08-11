#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <vector>
#include <typeindex>
#include <stdexcept>

class ResourceLoader
{
public:
    typedef void LoadType;
    virtual std::pair<std::string, void*> load(const std::string& path) const = 0; //should return nullptr when resource was not loaded. Also returns name.
    virtual ~ResourceLoader();
};

template <class T>
class ResourceHandle
{
public:
    ResourceHandle();
    ResourceHandle(T* resource);
    ResourceHandle(const ResourceHandle& other);

    void set(T* resource);
    const T& get() const;

    const T* operator->() const;
    T* operator->();

    operator bool() const;

    ~ResourceHandle();

protected:
    T* m_resource; //non owning
};


class ResourceManager
{
public:

    static ResourceManager& instance();

    template <class T>
    ResourceHandle<T> get(const std::string& name);

    template <class T, typename... Arguments>
    std::map<std::type_index, ResourceLoader*>::iterator registerLoader(Arguments&& ... args);

    template <class T>
    ResourceHandle<T> load(const std::string& path, const std::string& name); //this name has bigger priority than name given by resource loader
    template <class T>
    ResourceHandle<T> load(const std::string& path);

    ~ResourceManager();
protected:
    std::map<std::type_index, ResourceLoader*> m_resourceLoaders; //key is the type_index od the type that resource loader load
    std::map<std::string, void*> m_resources; //owning, key is the name of the object (not path)
};

//Template members definitions


template <class T>
ResourceHandle<T>::ResourceHandle() :
    m_resource(nullptr)
{

}
template <class T>
ResourceHandle<T>::ResourceHandle(T* resource) :
    m_resource(resource)
{
}
template <class T>
ResourceHandle<T>::ResourceHandle(const ResourceHandle& other) :
    m_resource(other.m_resource)
{

}

template <class T>
ResourceHandle<T>::operator bool() const
{
    return m_resource != nullptr;
}

template <class T>
void ResourceHandle<T>::set(T* resource)
{
    m_resource = resource;
}

template <class T>
const T& ResourceHandle<T>::get() const
{
    return *m_resource;
}
template <class T>
const T* ResourceHandle<T>::operator->() const
{
    return m_resource;
}
template <class T>
T* ResourceHandle<T>::operator->()
{
    return m_resource;
}

template <class T>
ResourceHandle<T>::~ResourceHandle()
{
}


template <class T>
ResourceHandle<T> ResourceManager::get(const std::string& name)
{
    auto iter = m_resources.find(name);
    void* resource = nullptr;
    if(iter != m_resources.end())
    {
        resource = iter->second;
    }
    else
    {
        throw std::runtime_error(std::string("No resource loaded with name ") + name);
    }
    return ResourceHandle<T>(static_cast<T*>(resource));
}

template <class T, typename... Arguments>
std::map<std::type_index, ResourceLoader*>::iterator ResourceManager::registerLoader(Arguments&& ... args)
{
    return m_resourceLoaders.insert(std::make_pair(std::type_index(typeid(typename T::LoadType)), new T(std::forward<Arguments>(args)...))).first;
}

template <class T>
ResourceHandle<T> ResourceManager::load(const std::string& path, const std::string& name)
{
    auto iter = m_resourceLoaders.find(std::type_index(typeid(T)));
    if(iter != m_resourceLoaders.end())
    {
        auto resourceIter = m_resources.find(name);
        if(resourceIter != m_resources.end()) return ResourceHandle<T>(static_cast<T*>(resourceIter->second));;
        std::pair<std::string, void*> resource = iter->second->load(path);
        if(resource.second == nullptr) throw std::runtime_error(std::string("Could not load resource from path ") + path);
        m_resources.insert(std::make_pair(name, resource.second));
        return ResourceHandle<T>(static_cast<T*>(resource.second));
    }
    else
    {
        throw std::runtime_error(std::string("No resource loader found for type ") + std::string(typeid(T).name()));
    }
}

template <class T>
ResourceHandle<T> ResourceManager::load(const std::string& path)
{
    auto iter = m_resourceLoaders.find(std::type_index(typeid(T)));
    if(iter != m_resourceLoaders.end())
    {
        std::pair<std::string, void*> resource = iter->second->load(path);
        if(resource.second == nullptr) throw std::runtime_error(std::string("Could not load resource from path ") + path);
        m_resources.insert(resource);
        return ResourceHandle<T>(static_cast<T*>(resource.second));
    }
    else
    {
        throw std::runtime_error(std::string("No resource loader found for type ") + std::string(typeid(T).name()));
    }
}

#endif // RESOURCEMANAGER_H
