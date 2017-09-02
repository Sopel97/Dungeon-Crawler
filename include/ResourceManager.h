#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <vector>
#include <typeindex>
#include <stdexcept>
#include <memory>

template <class T>
class ResourceLoader
{
};

template <class T>
class ResourceHandle
{
public:
    ResourceHandle() :
        m_resource(nullptr)
    {

    }
    ResourceHandle(std::unique_ptr<T>* resource) :
        m_resource(resource)
    {
    }
    ResourceHandle(const ResourceHandle& other) :
        m_resource(other.m_resource)
    {

    }

    void set(std::unique_ptr<T>* resource)
    {
        m_resource = resource;
    }
    const T& get() const
    {
        return **m_resource;
    }

    const T* operator->() const
    {
        return m_resource->get();
    }
    T* operator->()
    {
        return m_resource->get();
    }

    operator bool() const
    {
        return m_resource != nullptr;
    }

    ~ResourceHandle()
    {
    }

protected:
    std::unique_ptr<T>* m_resource; //non owning
};

template <class T>
class ResourceManager
{
public:
    static ResourceManager<T>& instance()
    {
        static ResourceManager<T> resourceManager;
        return resourceManager;
    }

    ResourceHandle<T> get(const std::string& name)
    {
        return ResourceHandle<T>(getImpl(name));
    }

    template <class... Args>
    ResourceHandle<T> loadWithName(const char* name, Args&&... args) //this name has bigger priority than name given by resource loader
    {
        return loadWithName<Args...>(std::string(name), std::forward<Args>(args)...);
    }
    template <class... Args>
    ResourceHandle<T> loadWithName(const std::string& name, Args&&... args) //this name has bigger priority than name given by resource loader
    {
        std::pair<std::string, std::unique_ptr<T>> resource = ResourceLoader<T>::load(std::forward<Args>(args)...);
        if (resource.second.get() == nullptr) throw std::runtime_error(std::string("Could not load resource with name ") + name);
        return ResourceHandle<T>(add(name, std::move(resource.second)));
    }
    ResourceHandle<T> load(const std::string& path)
    {
        std::pair<std::string, std::unique_ptr<T>> resource = ResourceLoader<T>::load(path);
        if (resource.second.get() == nullptr) throw std::runtime_error(std::string("Could not load resource from path ") + path);
        return ResourceHandle<T>(add(resource.first, std::move(resource.second)));
    }

    ~ResourceManager()
    {
    }
private:

    std::unique_ptr<T>* getImpl(const std::string& name)
    {
        return &(resources()[name]);
    }
    std::unique_ptr<T>* add(const std::string& name, std::unique_ptr<T> resource)
    {
        auto& ptr = resources()[name];
        ptr = std::move(resource);
        return &ptr;
    }
    std::map<std::string, std::unique_ptr<T>>& resources()
    {
        static std::map<std::string, std::unique_ptr<T>> res;
        return res;
    }
};

#endif // RESOURCEMANAGER_H
