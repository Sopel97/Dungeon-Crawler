#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "EntityView.h"

#include "Configuration.h"

#include "ResourceManager.h"

#include <memory>

#include "../LibS/GeometryLight.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class Entity;
class Player;

class PlayerView : public EntityView
{
public:
    PlayerView(Entity* owner, Player* player);
    PlayerView(const PlayerView& other);
    virtual ~PlayerView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const;

    const sf::Texture& texture() const;

    virtual std::unique_ptr<EntityView> clone() const;
    virtual std::unique_ptr<EntityView> create(Entity* owner) const;
protected:
    Player* m_playerOwner;
    ResourceHandle<sf::Texture> m_texture;
    Geo::Vec2I m_sprites;
};

#endif // PLAYERVIEW_H
