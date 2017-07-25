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
    ResourceHandle(T* resource) :
        m_resource(resource)
    {
    }
    ResourceHandle(const ResourceHandle& other) :
        m_resource(other.m_resource)
    {

    }

    void set(T* resource)
    {
        m_resource = resource;
    }
    const T& get() const
    {
        return *m_resource;
    }

    const T* operator->() const
    {
        return m_resource;
    }
    T* operator->()
    {
        return m_resource;
    }

    operator bool() const
    {
        return m_resource != nullptr;
    }

    ~ResourceHandle()
    {
    }

protected:
    T* m_resource; //non owning
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
        T* resource = getImpl(name);
        if (resource != nullptr) return ResourceHandle<T>(resource);
        else throw std::runtime_error(std::string("No resource found with name ") + name);
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
        T* ptr = resource.second.get();
        if (ptr == nullptr) throw std::runtime_error(std::string("Could not load resource with name ") + name);
        add(name, std::move(resource.second));
        return ResourceHandle<T>(ptr);
    }
    ResourceHandle<T> load(const std::string& path)
    {
        std::pair<std::string, std::unique_ptr<T>> resource = ResourceLoader<T>::load(path);
        T* ptr = resource.second.get();
        if (ptr == nullptr) throw std::runtime_error(std::string("Could not load resource from path ") + path);
        add(resource.first, std::move(resource.second));
        return ResourceHandle<T>(ptr);
    }

    ~ResourceManager()
    {
    }
private:

    T* getImpl(const std::string& name)
    {
        auto iter = resources().find(name);
        if (iter == resources().end()) return nullptr;
        else return iter->second.get();
    }
    void add(const std::string& name, std::unique_ptr<T> resource)
    {
        resources().insert(std::make_pair(name, std::move(resource)));
    }
    std::map<std::string, std::unique_ptr<T>>& resources()
    {
        static std::map<std::string, std::unique_ptr<T>> res;
        return res;
    }
};

#endif // RESOURCEMANAGER_H
