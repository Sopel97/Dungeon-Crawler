#ifndef ENTITY_H
#define ENTITY_H

#include "Configuration.h"

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

class Entity
{
public:
    Entity();
    Entity(std::unique_ptr<EntityModel>&& model, std::unique_ptr<EntityRenderer>&& renderer, std::unique_ptr<EntityController>&& controller);
    Entity(const Entity& other);
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    virtual ~Entity();

    void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const EntityModel& model() const;
    EntityModel& model();
    const EntityRenderer& renderer() const;
    EntityRenderer& renderer();
    const EntityController& controller() const;
    EntityController& controller();

    int id() const;

    virtual std::unique_ptr<Entity> clone() const;
protected:
    std::unique_ptr<EntityModel> m_model;
    std::unique_ptr<EntityRenderer> m_renderer;
    std::unique_ptr<EntityController> m_controller;
    int m_id;

    static int m_lastId;
};

#endif // ENTITY_H
