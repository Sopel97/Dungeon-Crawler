#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include "Configuration.h"

#include <memory>

class Tile;

class TileController
{
public:
    TileController(Tile* owner);
    TileController(const TileController& other);
    virtual ~TileController();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Tile* owner() const;

    void setOwner(Tile* newOwner);

    virtual std::unique_ptr<TileController> clone() const = 0;
protected:
    Tile* m_owner;
};

#endif // TILECONTROLLER_H
