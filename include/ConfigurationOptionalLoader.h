#pragma once

#include "Configuration.h"
#include "ConfigurationDefaultLoaders.h"

#include <optional>

namespace ConfigurationLoaders
{
    template <class T>
    inline void load(std::optional<T>& opt, ConfigurationNode& config)
    {
        if (!config.exists())
        {
            opt = std::nullopt;
            return;
        }

        opt.emplace();
        load(opt.value(), config);
    }
}