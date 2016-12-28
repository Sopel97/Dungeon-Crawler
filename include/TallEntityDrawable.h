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
    virtual ~TallEntityDrawable();

    virtual const ls::Rectangle2F& boundingRectangle() const;
    virtual const ls::Vec2F& center() const;

    virtual TallDrawable::DrawableType type() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    Entity* m_entity;
    ls::Rectangle2F m_boundingRectangle;
    ls::Vec2F m_center;
};

#endif // TALLENTITYDRAWABLE_H
