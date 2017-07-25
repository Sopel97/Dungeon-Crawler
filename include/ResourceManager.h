#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <vector>
#include <typeindex>
#include <stdexcept>
#include <memory>

namespace sf { class Texture; }

template <class T>
class ResourceLoader
{
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

    template <class T, class... Args>
    ResourceHandle<T> load(Args&&... args, const char* name); //this name has bigger priority than name given by resource loader
    template <class T, class... Args>
    ResourceHandle<T> load(Args&&... args, const std::string& name); //this name has bigger priority than name given by resource loader
    template <class T>
    ResourceHandle<T> load(const std::string& path);

    ~ResourceManager();
protected:
    template <class ResourceType>
    class SpecificResources
    {
    public:
        static ResourceType* get(const std::string& name);
        static void add(const std::string& name, std::unique_ptr<ResourceType> resource);
    private:
        static std::map<std::string, std::unique_ptr<ResourceType>>& resources();
    };
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

template <class ResourceType>
ResourceType* ResourceManager::SpecificResources<ResourceType>::get(const std::string& name)
{
    auto iter = resources().find(name);
    if(iter == resources().end()) return nullptr;
    else return iter->second.get();
}
template <class ResourceType>
void ResourceManager::SpecificResources<ResourceType>::add(const std::string& name, std::unique_ptr<ResourceType> resource)
{
    resources().insert(std::make_pair(name, std::move(resource)));
}

template <class T>
ResourceHandle<T> ResourceManager::get(const std::string& name)
{
    static_assert(!std::is_same<T, sf::Texture>::value);
    T* resource = SpecificResources<T>::get(name);
    if(resource != nullptr) return ResourceHandle<T>(resource);
    else throw std::runtime_error(std::string("No resource found with name ") + name);
}

template <class T, class... Args>
ResourceHandle<T> ResourceManager::load(Args&&... args, const char* name)
{
    return load<T, Args...>(std::forward<Args>(args)..., std::string(name));
}

template <class T, class... Args>
ResourceHandle<T> ResourceManager::load(Args&&... args, const std::string& name)
{
    std::pair<std::string, std::unique_ptr<T>> resource = ResourceLoader<T>::load(std::forward<Args>(args)...);
    T* ptr = resource.second.get();
    if(ptr == nullptr) throw std::runtime_error(std::string("Could not load resource with name ") + name);
    SpecificResources<T>::add(name, std::move(resource.second));
    return ResourceHandle<T>(ptr);
}

template <class T>
ResourceHandle<T> ResourceManager::load(const std::string& path)
{
    std::pair<std::string, std::unique_ptr<T>> resource = ResourceLoader<T>::load(path);
    T* ptr = resource.second.get();
    if(ptr == nullptr) throw std::runtime_error(std::string("Could not load resource from path ") + path);
    SpecificResources<T>::add(resource.first, std::move(resource.second));
    return ResourceHandle<T>(ptr);
}

template <class ResourceType>
std::map<std::string, std::unique_ptr<ResourceType>>& ResourceManager::SpecificResources<ResourceType>::resources()
{
    static std::map<std::string, std::unique_ptr<ResourceType>> res;
    return res;
}

#endif // RESOURCEMANAGER_H
