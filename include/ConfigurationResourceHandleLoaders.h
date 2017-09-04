#pragma once

#include "ResourceManager.h"

#include "Configuration.h"
#include "ConfigurationDefaultLoaders.h"

namespace ConfigurationLoaders
{
    template <class T>
    inline void load(ResourceHandle<T>& resourceHandle, ConfigurationNode& config)
    {
        if (!config.exists())
        {
            resourceHandle = nullptr;
            return;
        }

        resourceHandle = ResourceManager<T>::instance().get(config.get<std::string>());
    }
}