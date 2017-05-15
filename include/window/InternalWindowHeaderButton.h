#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "../LibS/Shapes.h"

class InternalWindow;

class InternalWindowHeaderButton
{
private:
    InternalWindow* m_parentWindow;
    ls::Rectangle2I m_spriteRect;
    std::function<void(InternalWindow&)> m_callback;
    int m_id;
    bool m_isEnabled;

public:
    InternalWindowHeaderButton(InternalWindow& parent, int id, const ls::Rectangle2I& sprite, bool enabled);

    template <class CallbackType>
    InternalWindowHeaderButton(InternalWindow& parent, int id, const ls::Rectangle2I& sprite, bool enabled, CallbackType&& callback) :
        m_parentWindow(&parent),
        m_spriteRect(sprite),
        m_callback(std::forward<CallbackType>(callback)),
        m_id(id),
        m_isEnabled(enabled)
    {

    }

    InternalWindowHeaderButton(const InternalWindowHeaderButton&) = default;
    InternalWindowHeaderButton(InternalWindowHeaderButton&&) = default;
    InternalWindowHeaderButton& operator=(const InternalWindowHeaderButton&) = default;
    InternalWindowHeaderButton& operator=(InternalWindowHeaderButton&&) = default;

    int width() const;
    int height() const;
    int id() const;
    bool isEnabled() const;

    void makeCallback();
    template <class CallbackType>
    void setCallback(CallbackType&& callback)
    {
        m_callback = std::forward<CallbackType>(callback);
    }
    void setEnabled(bool enable);

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const ls::Vec2I& pos);
};