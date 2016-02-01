#include "ResourceManager.h"

ResourceManager& ResourceManager::instance()
{
    static ResourceManager resourceManager;
    return resourceManager;
}

ResourceManager::~ResourceManager()
{

}
