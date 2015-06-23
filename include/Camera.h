#ifndef CAMERA_H
#define CAMERA_H

#include "../LibS/GeometryLight.h"

class Camera
{
public:
    Camera(const Geo::Vec2F& position, float width, float height);
    ~Camera();

    void move(const Geo::Vec2F& displacement);
    void setPosition(const Geo::Vec2F& newPosition);
    void setWidth(float newWidth);
    void setHeight(float newHeight);

    const Geo::Vec2F& position() const;
    float width() const;
    float height() const;

    Geo::RectangleF viewRectangle() const;
protected:
    Geo::Vec2F m_position; //center
    float m_width;
    float m_height;
};

#endif // CAMERA_H
