#pragma once

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class WindowSpace
{
public:
	WindowSpace();
	virtual ~WindowSpace();

	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) = 0;
};

