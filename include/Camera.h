#ifndef CAMERA_H
#define CAMERA_H

#include "../LibS/Geometry.h"

class Camera
{
public:
    Camera(const ls::Vec2F& position, float width, float height);
    ~Camera();

    void move(const ls::Vec2F& displacement);
    void setPosition(const ls::Vec2F& newPosition);
    void setWidth(float newWidth);
    void setHeight(float newHeight);

    const ls::Vec2F& position() const;
    float width() const;
    float height() const;

    ls::RectangleF viewRectangle() const;
protected:
    ls::Vec2F m_position; //center
    float m_width;
    float m_height;
};

#endif // CAMERA_H
