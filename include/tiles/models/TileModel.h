#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Configuration.h"

#include <memory>

class Tile;

class TileModel
{
public:
    TileModel(Tile* owner);
    TileModel(const TileModel& other);
    virtual ~TileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Tile* owner() const;

    void setOwner(Tile* newOwner);

    virtual std::unique_ptr<TileModel> clone() const;
protected:
    Tile* m_owner;
};

#endif // TILEMODEL_H
