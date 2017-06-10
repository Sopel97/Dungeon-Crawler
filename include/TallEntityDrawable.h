#ifndef TALLENTITYDRAWABLE_H
#define TALLENTITYDRAWABLE_H

#include "TallDrawable.h"

#include "../LibS/Geometry.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;

class TallEntityDrawable : public TallDrawable
{
public:
    TallEntityDrawable(Entity* entity);
    ~TallEntityDrawable() override;

    const ls::Rectangle2F& boundingRectangle() const override;
    const ls::Vec2F& center() const override;

    TallDrawable::DrawableType type() const override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;

protected:
    Entity* m_entity;
    ls::Rectangle2F m_boundingRectangle;
    ls::Vec2F m_center;
};

#endif // TALLENTITYDRAWABLE_H
