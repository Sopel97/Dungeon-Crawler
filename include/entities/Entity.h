#ifndef ENTITY_H
#define ENTITY_H

#include "Configuration.h"

#include "ComponentFactory.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class EntityModel;
class EntityRenderer;
class EntityController;
class ComponentCommonData;

class Entity
{
public:
    Entity(
        int id,
        const ComponentFactory<Entity, EntityModel>& modelFac,
        const ComponentFactory<Entity, EntityRenderer>& rendererFac,
        const ComponentFactory<Entity, EntityController>& controllerFac);
    Entity( //temporary to allow easy creation of the player entity
        int id,
        std::unique_ptr<EntityModel> model,
        std::unique_ptr<EntityRenderer> renderer,
        std::unique_ptr<EntityController> controller);
    Entity(const Entity& other);
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    ~Entity();

    void loadFromConfiguration(ConfigurationNode& config);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const EntityModel& model() const;
    EntityModel& model();
    const EntityRenderer& renderer() const;
    EntityRenderer& renderer();
    const EntityController& controller() const;
    EntityController& controller();

    int id() const;

    void onEntityInstantiated();
    void onEntityCloned();

    std::unique_ptr<Entity> clone() const;
    std::unique_ptr<Entity> instantiate() const;
protected:
    std::unique_ptr<EntityModel> m_model;
    std::unique_ptr<EntityRenderer> m_renderer;
    std::unique_ptr<EntityController> m_controller;
    int m_id;
};

#endif // ENTITY_H
