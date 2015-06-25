#ifndef ENTITYVIEW_H
#define ENTITYVIEW_H

#include "Configuration.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;

class EntityView
{
public:
    EntityView(Entity* owner);
    EntityView(const EntityView& other);
    virtual ~EntityView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const Entity* owner() const;

    void setOwner(Entity* newOwner);

    virtual std::unique_ptr<EntityView> clone() const;
    virtual std::unique_ptr<EntityView> create(Entity* owner) const;
protected:
    Entity* m_owner;
};

#endif // ENTITYVIEW_H
