#pragma once

#include "ConfigurationReader.h"

namespace ConfigurationReaders
{
    template <class T>
    struct Reader<ls::Vec2<T>>
    {
        static void read(ls::Vec2<T>& pos, ConfigurationNode& config)
        {
            pos.x = config[1].get<T>();
            pos.y = config[2].get<T>();
        }
    };

    template <class T>
    struct Reader<ls::Rectangle2<T>>
    {
        static void read(ls::Rectangle2<T>& rect, ConfigurationNode& config)
        {
            ConfigurationReaders::read(rect.min, config[1]);
            ConfigurationReaders::read(rect.max, config[2]);
        }
    };
}