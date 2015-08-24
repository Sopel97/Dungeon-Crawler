#include "Camera.h"

using namespace ls;

Camera::Camera(const Vec2F& position, float width, float height) :
    m_position(position),
    m_width(width),
    m_height(height)
{

}
Camera::~Camera()
{

}

void Camera::move(const Vec2F& displacement)
{
    m_position += displacement;
}
void Camera::setPosition(const Vec2F& newPosition)
{
    m_position = newPosition;
}
void Camera::setWidth(float newWidth)
{
    m_width = newWidth;
}
void Camera::setHeight(float newHeight)
{
    m_height = newHeight;
}

const Vec2F& Camera::position() const
{
    return m_position;
}
float Camera::width() const
{
    return m_width;
}
float Camera::height() const
{
    return m_height;
}

RectangleF Camera::viewRectangle() const
{
    Vec2F halfSize = Vec2F(m_width, m_height) / 2.0f;
    return RectangleF(m_position - halfSize, m_position + halfSize);
}
