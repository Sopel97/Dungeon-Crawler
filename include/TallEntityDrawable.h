#ifndef TALLENTITYDRAWABLE_H
#define TALLENTITYDRAWABLE_H

#include "TallDrawable.h"

#include "../LibS/GeometryLight.h"

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

    virtual bool isEntity() const;
    virtual const Geo::RectangleF& boundingRectangle() const;
    virtual const Geo::Vec2F& center() const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    Entity* m_entity;
    Geo::RectangleF m_boundingRectangle;
    Geo::Vec2F m_center;
};

#endif // TALLENTITYDRAWABLE_H
