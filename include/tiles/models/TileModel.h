#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Configuration.h"

class Tile;

class TileModel
{
public:
    TileModel(Tile* owner);
    virtual ~TileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Tile* owner() const;
protected:
    Tile* m_owner;
};

#endif // TILEMODEL_H
