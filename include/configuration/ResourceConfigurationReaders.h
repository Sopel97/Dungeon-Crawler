#pragma once

#include "ConfigurationReader.h"

#include "ResourceManager.h"

namespace ConfigurationReaders
{
    template <class T>
    struct Reader<ResourceHandle<T>>
    {
        static void read(ResourceHandle<T>& resourceHandle, ConfigurationNode& config)
        {
            if (config.exists())
            {
                resourceHandle = ResourceManager<T>::instance().get(config.get<std::string>());
            }
            else
            {
                resourceHandle = nullptr;
            }
        }
    };
}