#ifndef ENTITY_H
#define ENTITY_H

#include "configuration/Configuration.h"

#include "ComponentFactory.h"

#include "../LibS/Shapes.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class EntityModel;
class EntityRenderer;
class ComponentCommonData;

class Entity
{
public:
    Entity(
        int id,
        const ComponentFactory<Entity, EntityModel>& modelFac,
        const ComponentFactory<Entity, EntityRenderer>& rendererFac);
    Entity( //temporary to allow easy creation of the player entity
        int id,
        std::unique_ptr<EntityModel> model,
        std::unique_ptr<EntityRenderer> renderer);
    Entity(const Entity& other);
    Entity(Entity&& other);
    Entity& operator=(const Entity& other);
    Entity& operator=(Entity&& other);
    ~Entity();

    void loadFromConfiguration(ConfigurationNode& config);

    const EntityModel& model() const;
    EntityModel& model();
    const EntityRenderer& renderer() const;
    EntityRenderer& renderer();

    int id() const;

    void onEntityInstantiated(const ls::Vec2F& pos);
    void onEntityCloned();

    std::unique_ptr<Entity> instantiate(const ls::Vec2F& pos) const;
    std::unique_ptr<Entity> clone() const;
protected:
    std::unique_ptr<EntityModel> m_model;
    std::unique_ptr<EntityRenderer> m_renderer;
    int m_id;
};

#endif // ENTITY_H
