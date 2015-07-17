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

class EntityView //must be functions (ie. all methods return resonable values and there is not pure virtual member functions)
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

    static constexpr int tileSize = 32; //TODO: think about what this should belong to
    static constexpr int tileFullSpriteSize = 34; //includes the boundary size that is required to prevent tearing
};

#endif // ENTITYVIEW_H
