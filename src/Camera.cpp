#include "Camera.h"

#include <cmath>

using namespace ls;

Camera::Camera(const Vec2F& center, float width, float height) :
    m_center(center),
    m_width(width),
    m_height(height)
{

}
Camera::~Camera()
{

}

void Camera::move(const Vec2F& displacement)
{
    m_center += displacement;
}
void Camera::setCenter(const Vec2F& newCenter)
{
    m_center = newCenter;
}
void Camera::setWidth(float newWidth)
{
    m_width = newWidth;
}
void Camera::setHeight(float newHeight)
{
    m_height = newHeight;
}

const Vec2F& Camera::center() const
{
    return m_center;
}
float Camera::width() const
{
    return m_width;
}
float Camera::height() const
{
    return m_height;
}

Rectangle2F Camera::viewRectangle() const
{
    Vec2F halfSize = Vec2F(m_width, m_height) / 2.0f;
    Vec2F center(std::floor(m_center.x), std::floor(m_center.y));
    return Rectangle2F(center - halfSize, center + halfSize);
}
