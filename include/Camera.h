#ifndef CAMERA_H
#define CAMERA_H

#include "../LibS/Geometry.h"

class Camera
{
public:
    Camera(const ls::Vec2F& center, float width, float height);
    ~Camera();

    void move(const ls::Vec2F& displacement);
    void setCenter(const ls::Vec2F& newCenter);
    void setWidth(float newWidth);
    void setHeight(float newHeight);

    const ls::Vec2F& center() const;
    float width() const;
    float height() const;

    ls::Rectangle2F viewRectangle() const;
protected:
    ls::Vec2F m_center; //center
    float m_width;
    float m_height;
};

#endif // CAMERA_H
