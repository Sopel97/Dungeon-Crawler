#pragma once

#include "Configuration.h"

namespace ConfigurationLoaders
{
    template <class T>
    inline void load(T& val, ConfigurationNode& config)
    {
        val = config.get<T>();
    }
}