#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include "Configuration.h"

class Tile;

class TileController
{
public:
    TileController(Tile* owner);
    virtual ~TileController();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Tile* owner() const;
protected:
    Tile* m_owner;
};

#endif // TILECONTROLLER_H
