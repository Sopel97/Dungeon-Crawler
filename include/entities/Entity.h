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
class EntityView;
class EntityController;

class Entity
{
public:
    Entity(std::unique_ptr<EntityModel>&& model, std::unique_ptr<EntityView>&& view, std::unique_ptr<EntityController>&& controller);
    Entity(const Entity& other);
    virtual ~Entity();

    void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const EntityModel& model() const;
    EntityModel& model();
    const EntityView& view() const;
    EntityView& view();
    const EntityController& controller() const;
    EntityController& controller();

    int id() const;

    virtual std::unique_ptr<Entity> clone() const;
protected:
    std::unique_ptr<EntityModel> m_model;
    std::unique_ptr<EntityView> m_view;
    std::unique_ptr<EntityController> m_controller;
    int m_id;

    static int m_lastId;
};

#endif // ENTITY_H
