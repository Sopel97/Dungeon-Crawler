#ifndef TILE_H
#define TILE_H

#include "Configuration.h"

#include "ComponentFactory.h"

#include "../LibS/Shapes.h"

#include "TileUseResult.h"
#include "TileAttackResult.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class TileModel;
class TileRenderer;
class TileLocation;
class InventorySlotView;
class ComponentCommonData;
class Inventory;
class Player;
class InventorySlotLocation;
class World;

class Tile
{
public:
    Tile(
        int id,
        const ComponentFactory<Tile, TileModel>& modelFac,
        const ComponentFactory<Tile, TileRenderer>& rendererFac);
    Tile(); // temporary for creating 'empty' tile, used to avoid checking if tile exists, tile column just returns this
    Tile(const Tile& other);
    Tile(Tile&& other);
    Tile& operator=(const Tile& other);
    Tile& operator=(Tile&& other);
    ~Tile();

    void loadFromConfiguration(ConfigurationNode& config);

    const TileModel& model() const;
    TileModel& model();
    const TileRenderer& renderer() const;
    TileRenderer& renderer();

    bool equals(const Tile& other) const;

    int maxQuantity() const;

    TileUseResult use(Player& player, const TileLocation& location, int quantity);
    TileUseResult use(Player& player, const InventorySlotLocation& location, int quantity);
    void look(Player& player, const TileLocation& location, int quantity);
    void look(Player& player, const InventorySlotLocation& location, int quantity);
    TileAttackResult attack(World& world, Player& player, const ls::Vec2F& hintedPosition, int quantity);
    void indirectAttack(World& world, Player& player, const ls::Vec2F& hintedPosition, int quantity);

    void onTileQuantityChanged(int oldQuantity, int newQuantity);
    void onTileInstantiated();
    void onTileCloned();

    void onTilePlaced(const TileLocation& location);
    void onTilePlaced(const Inventory& inventory, int slot);
    void onTileRemoved(const TileLocation& location);
    void onTileRemoved(const Inventory& inventory, int slot);

    void onTilePlacedNearby(const TileLocation& thisLocation, const ls::Vec2I& updatedTileOffset);
    void onTileRemovedNearby(const TileLocation& thisLocation, const ls::Vec2I& updatedTileOffset);
    void onTilePlacedOnTop(const TileLocation& thisLocation);
    void onTileRemovedFromTop(const TileLocation& thisLocation);

    int id() const;

    std::unique_ptr<Tile> clone() const;
    std::unique_ptr<Tile> instantiate() const;

protected:
    std::unique_ptr<TileModel> m_model;
    std::unique_ptr<TileRenderer> m_renderer;
    int m_id;
};

#endif // TILE_H
