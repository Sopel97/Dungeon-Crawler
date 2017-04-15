#pragma once 

#include "Configuration.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;

class EntityRenderer //must be functions (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    EntityRenderer(Entity* owner);
    EntityRenderer(const EntityRenderer& other);
    virtual ~EntityRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const Entity* owner() const;

    void setOwner(Entity* newOwner);

    virtual std::unique_ptr<EntityRenderer> clone() const;
    virtual std::unique_ptr<EntityRenderer> create(Entity* owner) const;
protected:
    Entity* m_owner;
};
