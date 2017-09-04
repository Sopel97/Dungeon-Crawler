#pragma once

#include "Configuration.h"
#include "ConfigurationDefaultLoaders.h"

#include "../LibS/Shapes.h"

#include <optional>

namespace ConfigurationLoaders
{
    template <class T>
    inline void load(ls::Vec2<T>& pos, ConfigurationNode& config)
    {
        pos.x = config[1].get<T>();
        pos.y = config[2].get<T>();
    }

    template <class T>
    inline void load(ls::Rectangle2<T>& rect, ConfigurationNode& config)
    {
        load(rect.min, config[1]);
        load(rect.max, config[2]);
    }
}