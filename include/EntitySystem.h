#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <memory>

#include "../LibS/Geometry.h"

#include "tiles/TileStack.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;
class Camera;
class World;
class Player;
class EntityPrefab;

class EntitySystem
{
public:
    EntitySystem(Player& player);

    std::vector<Entity*> query(const ls::Rectangle2F& rect);
    const std::vector<std::unique_ptr<Entity>>& entities() const;

    Entity& entity(int i);
    const Entity& entity(int i) const;

    void addEntity(std::unique_ptr<Entity>&& newEntity, const ls::Vec2F& position);
    void emplaceEntity(const EntityPrefab& prefab, const ls::Vec2F& position);
    void removeEntity(Entity& entityToRemove);

    void updateEntities(World& world, float dt);

    std::vector<Entity*> getVisibleEntities(const Camera& camera);
protected:
    Player* m_player;
    std::vector<std::unique_ptr<Entity>> m_entities;
    
    void moveEntity(World& world, Entity& entity, float dt);
    void removeDeadEntities();
    void createCorpsesForDeadEntities(World& world) const;
    TileStack createCorpse(const Entity& entity) const;

    bool isDead(const Entity& entity) const;
};

#endif // ENTITYSYSTEM_H
