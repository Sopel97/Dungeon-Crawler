#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <memory>
#include <optional>

#include "colliders/EntityCollider.h"
#include "colliders/TileCollider.h"

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
class Light;

class EntitySystem
{
public:
    EntitySystem(World& world, Player& player);

    std::vector<Entity*> query(const ls::Rectangle2F& rect);
    std::vector<EntityCollider> queryColliders(const ls::Rectangle2F& rect);
    std::vector<Light> queryLights(const ls::Rectangle2F& rect);
    const std::vector<std::unique_ptr<Entity>>& entities() const;

    Entity& entity(int i);
    const Entity& entity(int i) const;

    Entity& addEntity(std::unique_ptr<Entity>&& newEntity);
    Entity& spawnEntity(const EntityPrefab& prefab, const ls::Vec2F& position);
    void removeEntity(Entity& entityToRemove);

    void update(float dt);

    std::vector<Entity*> getVisibleEntities(const Camera& camera);
protected:
    World* m_world;
    Player* m_player;
    std::vector<std::unique_ptr<Entity>> m_entities;
    
    void updateEntity(Entity& entity, float dt);
    void removeDeadEntities();
    void createCorpsesForDeadEntities(World& world) const;
    TileStack createCorpse(const Entity& entity) const;

    bool isDead(const Entity& entity) const;

    std::optional<ls::Vec2F> penetration(EntityCollider& entityCollider, std::vector<TileCollider>& tileColliders);
    ls::Vec2F penetration(EntityCollider& entityCollider, TileCollider& tileCollider);
    ls::Vec2F resolvePenetration(const ls::Vec2F& pen); 
    ls::Vec2F adjustedVelocityAfterPenetrationResolved(const ls::Vec2F& escape, const ls::Vec2F& velocity);

    bool isAlmostZero(const ls::Vec2F& v, float thr) const;

    ls::Vec2F pushingForce(EntityCollider& entityCollider);
};

#endif // ENTITYSYSTEM_H
