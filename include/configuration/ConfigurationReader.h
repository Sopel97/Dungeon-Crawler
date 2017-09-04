#pragma once

#include "Configuration.h"

namespace ConfigurationReaders
{
    template <class...>
    struct Reader;

    template <class T>
    void read(T& destination, ConfigurationNode& config)
    {
        Reader<T>::read(destination, config);
    }

    template <class T, class U>
    void readDefault(T& destination, ConfigurationNode& config, U&& defaultValue)
    {
        if (config.exists())
        {
            ConfigurationReaders::read(destination, config);
        }
        else
        {
            destination = std::forward<U>(defaultValue);
        }
    }
}