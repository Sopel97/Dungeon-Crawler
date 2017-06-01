#pragma once

#include "TileAttributeSet.h"
#include "Configuration.h"

class TileAttributeRandomizer
{
private:
public:
    void loadFromConfiguration(ConfigurationNode& config);
    TileAttributeSet randomize() const;
};