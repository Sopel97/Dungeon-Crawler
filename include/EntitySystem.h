#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "../LibS/Geometry.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;
class Camera;
class World;
class Player;

class EntitySystem
{
public:
    EntitySystem(Player& player);

    std::vector<Entity*> queryRegion(const ls::Rectangle2F& rect);
    const std::vector<Entity>& entities() const;

    Entity& entity(int i);
    const Entity& entity(int i) const;

    void addEntity(Entity&& newEntity, const ls::Vec2F& position);
    void removeEntity(Entity& entityToRemove);

    void updateEntities(World& world, float dt);
    void moveEntity(World& world, Entity& entity, float dt);

    std::vector<Entity*> getVisibleEntities(const Camera& camera);
protected:
    Player* m_player;
    std::vector<Entity> m_entities; //eventually will be some spatial storage
};

#endif // ENTITYSYSTEM_H
