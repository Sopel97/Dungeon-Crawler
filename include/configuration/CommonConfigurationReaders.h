#pragma once

#include "ConfigurationReader.h"

#include <optional>
#include <vector>
#include <set>

namespace ConfigurationReaders
{
    template <class T>
    struct Reader<T>
    {
        static void read(T& val, ConfigurationNode& config)
        {
            val = config.get<T>();
        }
    };

    template <class T>
    struct Reader<std::optional<T>>
    {
        static void read(std::optional<T>& opt, ConfigurationNode& config)
        {
            if (config.exists())
            {
                opt.emplace();
                ConfigurationReaders::read(opt.value(), config);
            }
            else
            {
                opt.reset();
            }
        }
    };

    template <class T>
    struct Reader<std::vector<T>>
    {
        static void read(std::vector<T>& vector, ConfigurationNode& config)
        {
            if (config.exists())
            {
                const int numEntries = config.length();
                for (int i = 1; i <= numEntries; ++i)
                {
                    T val;
                    ConfigurationReaders::read(val, config[i]);
                    vector.emplace_back(std::move(val));
                }
            }
        }
    };

    template <class T>
    struct Reader<std::set<T>>
    {
        static void read(std::set<T>& set, ConfigurationNode& config)
        {
            if (config.exists())
            {
                const int numEntries = config.length();
                for (int i = 1; i <= numEntries; ++i)
                {
                    T val;
                    ConfigurationReaders::read(val, config[i]);
                    set.emplace(std::move(val));
                }
            }
        }
    };
}