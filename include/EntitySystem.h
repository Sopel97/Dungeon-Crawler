#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include "../LibS/GeometryLight.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;
class Camera;
class World;

class EntitySystem
{
public:
    EntitySystem();
    ~EntitySystem();

    std::vector<Entity*> queryRegion(const Geo::RectangleF& rect);
    const std::vector<Entity*>& entities() const;

    void addEntity(Entity* newEntity, const Geo::Vec2F& position);
    void removeEntity(Entity* entityToRemove);

    void updateEntities(World* world, float dt);
    std::vector<Entity*> getVisibleEntities(const Camera& camera);
protected:
    std::vector<Entity*> m_entities; //eventually will be some spatial storage
};

#endif // ENTITYSYSTEM_H
