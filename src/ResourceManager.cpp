#include "ResourceManager.h"


ResourceLoader::~ResourceLoader()
{
}


ResourceManager& ResourceManager::instance()
{
    static ResourceManager resourceManager;
    return resourceManager;
}

ResourceManager::~ResourceManager()
{
    for(auto& resourceLoaderPair : m_resourceLoaders)
    {
        delete resourceLoaderPair.second;
    }
}
