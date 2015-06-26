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

class EntitySystem
{
public:
    EntitySystem();
    ~EntitySystem();

    std::vector<Entity*> queryRegion(const Geo::RectangleF& rect);
    const std::vector<Entity*>& entities() const;

    void addEntity(Entity* newEntity);
    void removeEntity(Entity* entityToRemove);

    void updateEntities(float dt);
    void drawVisibleEntities(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const Camera& camera);
protected:
    std::vector<Entity*> m_entities; //eventually will be some spatial storage
};

#endif // ENTITYSYSTEM_H
