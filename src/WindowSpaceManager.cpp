#include "WindowSpaceManager.h"

#include "WindowSpaceUser.h"

using namespace ls;
/*

WindowSpaceManager::Region::Region(const Rectangle2I& rect, WindowSpaceManager::Region::Id id, const std::vector<WindowSpaceManager::Scene>& parentScenes, int zIndex) :
	m_rect(rect),
	m_id(id),
	m_parentScenes(parentScenes),
	m_zIndex(zIndex)
{

}


Vec2I WindowSpaceManager::Region::localCoords(const Vec2I& windowCoords) const
{
	return windowCoords - m_rect.min;
}
const Rectangle2I& WindowSpaceManager::Region::rect() const
{
	return m_rect;
}
WindowSpaceManager::Region::Id WindowSpaceManager::Region::id() const
{
	return m_id;
}
const std::vector<WindowSpaceManager::Scene>& WindowSpaceManager::Region::parentScenes() const
{
	return m_parentScenes;
}
bool WindowSpaceManager::Region::isPresentOnScene(WindowSpaceManager::Scene scene) const
{
	return std::find(m_parentScenes.begin(), m_parentScenes.end(), scene) != m_parentScenes.end();
}
int WindowSpaceManager::Region::zIndex() const
{
	return m_zIndex;
}

WindowSpaceManager::WindowSpaceManager(sf::RenderWindow& window) :
	m_window(window),
	m_currentScene(Scene::Main)
{
	updateRegions();
}

sf::View WindowSpaceManager::viewOfRect(const ls::Rectangle2I& windowViewRect, const ls::Rectangle2F& worldViewRect)
{
	Vec2F windowSize{(float)m_window.getSize().x, (float)m_window.getSize().y};
	Vec2F windowViewTopLeft = Vec2F(windowViewRect.min);
	Vec2F windowViewSize = Vec2F(windowViewRect.max - windowViewRect.min);
	Vec2F windowViewTopLeftRelativeToWindow = windowViewTopLeft / windowSize;
	Vec2F windowViewSizeRelativeToWindow = windowViewSize / windowSize;

	sf::FloatRect view(windowViewTopLeftRelativeToWindow.x, windowViewTopLeftRelativeToWindow.y, windowViewSizeRelativeToWindow.x, windowViewSizeRelativeToWindow.y);
	sf::View panelView(sf::Vector2f(worldViewRect.min.x + worldViewRect.width() / 2.0f, (worldViewRect.min.y + worldViewRect.height() / 2.0f)), sf::Vector2f(worldViewRect.width(), worldViewRect.height()));
	panelView.setViewport(view);

	return panelView;
}
sf::View WindowSpaceManager::viewOfRegion(Region::Id regionId)
{
	const Rectangle2I& rect = regionRect(regionId);
	return viewOfRect(rect, Rectangle2F::withSize(Vec2F(0.0f, 0.0f), static_cast<float>(rect.width()), static_cast<float>(rect.height())));
}
sf::View WindowSpaceManager::viewOfRegion(Region::Id regionId, const ls::Rectangle2F& worldViewRect)
{
	const Rectangle2I& rect = regionRect(regionId);
	return viewOfRect(rect, worldViewRect);
}
void WindowSpaceManager::setViewToRect(const Rectangle2I& windowViewRect, const Rectangle2F& worldViewRect)
{
	m_window.setView(viewOfRect(windowViewRect, worldViewRect));
}
void WindowSpaceManager::setViewToRegion(WindowSpaceManager::Region::Id regionId)
{
	m_window.setView(viewOfRegion(regionId));
}
void WindowSpaceManager::setViewToRegion(WindowSpaceManager::Region::Id regionId, const Rectangle2F& worldViewRect)
{
	m_window.setView(viewOfRegion(regionId, worldViewRect));
}

void WindowSpaceManager::setDefaultView()
{
	Vec2F windowSize {(float)m_window.getSize().x, (float)m_window.getSize().y};

	sf::FloatRect view(0.0f, 0.0f, 1.0f, 1.0f);
	sf::View panelView(sf::Vector2f((windowSize.x) / 2.0f, ((windowSize.y) / 2.0f)), sf::Vector2f(windowSize.x, windowSize.y));
	panelView.setViewport(view);
	m_window.setView(panelView);
}
void WindowSpaceManager::updateRegions()
{
	constexpr int playerUiWidth = 230;

	Vec2I windowSize {(int)m_window.getSize().x, (int)m_window.getSize().y};

	int worldViewSize = std::min(windowSize.x - playerUiWidth, windowSize.y);

	m_regions[Region::PlayerUi] = Region
	{
		Rectangle2I::withSize
		(
			Vec2I
			(
				windowSize.x - playerUiWidth,
				0
			),
			playerUiWidth,
			windowSize.y
		),
		Region::PlayerUi,
		std::vector<Scene>
		{
			Scene::Main
		},
		0
	};
	m_regions[Region::World] = Region
	{
		Rectangle2I::withSize
		(
			Vec2I
			(
				(windowSize.x - playerUiWidth - worldViewSize) / 2,
				(windowSize.y - worldViewSize) / 2
			),
			worldViewSize,
			worldViewSize
		),
		Region::World,
		std::vector<Scene>
		{
			Scene::Main
		},
		0
	};
}

WindowSpaceManager::Scene WindowSpaceManager::currentScene() const
{
	return m_currentScene;
}
void WindowSpaceManager::setCurrentScene(Scene scene)
{
	m_currentScene = scene;
}

const std::map<WindowSpaceManager::Region::Id, WindowSpaceManager::Region>& WindowSpaceManager::regions() const
{
	return m_regions;
}
const WindowSpaceManager::Region& WindowSpaceManager::region(WindowSpaceManager::Region::Id regionId) const
{
	return m_regions.at(regionId);
}
const WindowSpaceManager::Region* WindowSpaceManager::pointedRegion(const Vec2I& windowCoords) const
{
	const Region* bestCandidate = nullptr;
	int highestZ = std::numeric_limits<int>::min();
	for(const auto& region : m_regions)
	{
		const Region& candidate = region.second;
		if(!candidate.isPresentOnScene(m_currentScene)) continue;
		if(candidate.zIndex() < highestZ) continue;
		if(ls::intersect(candidate.rect(), windowCoords))
		{
			bestCandidate = &candidate;
			highestZ = candidate.zIndex();
		}
	}
	return bestCandidate;
}
const Rectangle2I& WindowSpaceManager::regionRect(WindowSpaceManager::Region::Id regionId) const
{
	return region(regionId).rect();
}

void WindowSpaceManager::onWindowResized(const sf::Event& windowResizedEvent)
{
	updateRegions();
}
*/


WindowSpaceManager::SubdivisionParams WindowSpaceManager::SubdivisionParams::
withPixels(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, int pixels)
{
	return SubdivisionParams(orientation, subject, pixels);
}

WindowSpaceManager::SubdivisionParams WindowSpaceManager::SubdivisionParams::
withRatio(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, float ratio)
{
	return SubdivisionParams(orientation, subject, ratio);
}

std::pair<ls::Rectangle2I, ls::Rectangle2I> WindowSpaceManager::SubdivisionParams::
calculateSubRects(const ls::Rectangle2I& rect) const
{
	const int fullWidth = rect.width();
	const int fullHeight = rect.height();

	int width1 = fullWidth;
	int height1 = fullHeight;
	int width2 = fullWidth;
	int height2 = fullHeight;
	ls::Vec2I offset(0, 0);

	if (m_orientation == Orientation::Horizontal)
	{
		width1 = asPixels(rect);
		width2 = fullWidth - width1;

		if (m_subject == Subject::Second) std::swap(width1, width2);

		offset.x = width1;
	}
	else
	{
		height1 = asPixels(rect);
		height2 = fullHeight - height1;

		if (m_subject == Subject::Second) std::swap(height1, height2);

		offset.y = height1;
	}

	return {
		ls::Rectangle2I::withSize(rect.min, width1, height1),
		ls::Rectangle2I::withSize(rect.min + offset, width2, height2)
	};
}

int WindowSpaceManager::SubdivisionParams::asPixels(const ls::Rectangle2I& rect) const
{
	if (m_amountType == AmountType::Pixels) return m_amount.pixels;
	else return m_amount.ratio * (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
}
float WindowSpaceManager::SubdivisionParams::asRatio(const ls::Rectangle2I& rect) const
{
	if (m_amountType == AmountType::Ratio) return m_amount.ratio;
	else return static_cast<float>(m_amount.pixels) / (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
}


void WindowSpaceManager::SubdivisionParams::setPixels(int newPixels)
{
	m_amount.pixels = newPixels;
	m_amountType = AmountType::Pixels;
}

void WindowSpaceManager::SubdivisionParams::setRatio(float newRatio)
{
	m_amount.ratio = newRatio;
	m_amountType = AmountType::Ratio;
}

void WindowSpaceManager::SubdivisionParams::moveByPixels(const ls::Rectangle2I& rect, int deltaPixels)
{
	if (m_amountType == AmountType::Pixels) m_amount.pixels += deltaPixels;
	else
	{
		float deltaRatio = static_cast<float>(deltaPixels) / (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
		if (m_subject == Subject::Second) deltaRatio = -deltaRatio;

		m_amount.ratio += deltaRatio;
	}
}
void WindowSpaceManager::SubdivisionParams::moveByRatio(const ls::Rectangle2I& rect, float deltaRatio)
{
	if (m_amountType == AmountType::Ratio) m_amount.ratio += deltaRatio;
	else
	{
		float deltaPixels = deltaRatio * (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
		if (m_subject == Subject::Second) deltaPixels = -deltaPixels;

		m_amount.pixels = deltaPixels;
	}
}

WindowSpaceManager::SubdivisionParams::
SubdivisionParams(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, int pixels) :
	m_orientation(orientation),
	m_amountType(AmountType::Pixels),
	m_subject(subject)
{
	m_amount.pixels = pixels;
}
WindowSpaceManager::SubdivisionParams::
SubdivisionParams(WindowSpaceManager::SubdivisionParams::Orientation orientation, WindowSpaceManager::SubdivisionParams::Subject subject, float ratio) :
	m_orientation(orientation),
	m_amountType(AmountType::Ratio),
	m_subject(subject)
{
	m_amount.ratio = ratio;
}

WindowSpaceManager::WindowRegion::
WindowRegion(const ls::Rectangle2I& rect, const std::string& name, Scene& parentScene) :
	m_rect(rect),
	m_name(name),
	m_parentScene(&parentScene),
	m_spaceUser(nullptr)
{

}

const ls::Rectangle2I& WindowSpaceManager::WindowRegion::rect() const
{
	return m_rect;
}
const std::string& WindowSpaceManager::WindowRegion::name() const
{
	return m_name;
}
const WindowSpaceManager::Scene& WindowSpaceManager::WindowRegion::parentScene() const
{
	return *m_parentScene;
}
WindowSpaceManager::Scene& WindowSpaceManager::WindowRegion::parentScene()
{
	return *m_parentScene;
}
bool WindowSpaceManager::WindowRegion::hasEventHandler() const
{
	return m_spaceUser != nullptr;
}
SfmlEventHandler& WindowSpaceManager::WindowRegion::eventHandler()
{
	return *m_spaceUser;
}

void WindowSpaceManager::WindowRegion::setRect(const ls::Rectangle2I& newRect)
{
	m_rect = newRect;
}
void WindowSpaceManager::WindowRegion::setUser(WindowSpaceUser& newUser)
{
	m_spaceUser = &newUser;
}
ls::Vec2I WindowSpaceManager::WindowRegion::localCoords(const ls::Vec2I& windowCoords) const
{
	return windowCoords - m_rect.min;
}
void WindowSpaceManager::WindowRegion::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
	if (m_spaceUser != nullptr) m_spaceUser->draw(renderTarget, renderStates);
}


WindowSpaceManager::Scene::Scene(const ls::Rectangle2I& rect, const std::string& name) :
	m_windowRegions(std::make_pair(WindowRegion(rect, "Root", *this), std::nullopt)),
	m_name(name),
	m_topmostRegions({ m_windowRegions.rootHandle() }),
	m_focusedRegionHandle(m_windowRegions.rootHandle())
{
}

WindowSpaceManager::WindowRegion& WindowSpaceManager::Scene::windowRegion(WindowRegionHandle h)
{
	return m_windowRegions.node(h).first;
}

const WindowSpaceManager::WindowRegion& WindowSpaceManager::Scene::windowRegion(WindowRegionHandle h) const
{
	return m_windowRegions.node(h).first;
}

bool WindowSpaceManager::Scene::isSubdivided(WindowRegionHandle h) const
{
	return m_windowRegions.node(h).second.has_value();
}

WindowSpaceManager::SubdivisionParams& WindowSpaceManager::Scene::subdivisionParams(WindowRegionHandle h)
{
	return m_windowRegions.node(h).second.value();
}
const WindowSpaceManager::SubdivisionParams& WindowSpaceManager::Scene::subdivisionParams(WindowRegionHandle h) const
{
	return m_windowRegions.node(h).second.value();
}

std::pair<WindowSpaceManager::WindowRegionHandle, WindowSpaceManager::WindowRegionHandle> WindowSpaceManager::Scene::
subdivide(WindowRegionHandle h, const SubdivisionParams& params, const std::string& nameFirst, const std::string& nameSecond)
{
	const ls::Rectangle2I& rect = windowRegion(h).rect();
	auto subdividedRects = subdivisionParams(h).calculateSubRects(rect);

	WindowRegionHandle left = m_windowRegions.emplaceLeft(h, subdividedRects.first, nameFirst, *this);
	WindowRegionHandle right = m_windowRegions.emplaceRight(h, subdividedRects.second, nameSecond, *this);

	auto it = std::find(m_topmostRegions.begin(), m_topmostRegions.end(), h);
	*it = left;	// it is guaranteed to exist
	m_topmostRegions.push_back(right);

	return { left, right };
}

void WindowSpaceManager::Scene::
setSubdivisionParams(WindowSpaceManager::WindowRegionHandle h, const WindowSpaceManager::SubdivisionParams& params)
{
	m_windowRegions.node(h).second.emplace(params);
}

WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::firstChild(WindowRegionHandle h) const
{
	return m_windowRegions.left(h);
}
WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::secondChild(WindowRegionHandle h) const
{
	return m_windowRegions.right(h);
}
WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::parent(WindowRegionHandle h) const
{
	return m_windowRegions.parent(h);
}
bool WindowSpaceManager::Scene::hasChildren(WindowRegionHandle h) const
{
	return m_windowRegions.hasLeft(h);	// should always have right too
}
bool WindowSpaceManager::Scene::hasParent(WindowRegionHandle h) const
{
	return m_windowRegions.hasParent(h);
}

WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::find(const std::string& name) const
{
	return m_windowRegions.findIf([&name](const WindowRegionStorageNode& node)->bool {return node.first.name() == name; });
}

void WindowSpaceManager::Scene::update(const ls::Rectangle2I& rect)
{
	update(m_windowRegions.rootHandle(), rect);
}
void WindowSpaceManager::Scene::update(WindowRegionHandle h, const ls::Rectangle2I& rect)
{
	windowRegion(h).setRect(rect);
	if (!isSubdivided(h)) return;

	auto subdividedRects = subdivisionParams(h).calculateSubRects(rect);
	if (hasChildren(h))
	{
		update(m_windowRegions.left(h), subdividedRects.first);
		update(m_windowRegions.right(h), subdividedRects.second);
	}
}

WindowSpaceManager::WindowRegionHandle WindowSpaceManager::Scene::queryRegion(const ls::Vec2I& pos) const
{
	WindowRegionHandle currentRegionHandle = m_windowRegions.rootHandle();
	
	// only leaves can be returned
	while (hasChildren(currentRegionHandle))
	{
		// we know it has both left and right child

		if (!ls::intersect(windowRegion(currentRegionHandle).rect(), pos))
		{
			return invalidHandle;
		}

		WindowRegionHandle leftChildHandle = m_windowRegions.left(currentRegionHandle);
		WindowRegionHandle rightChildHandle = m_windowRegions.right(currentRegionHandle);

		//explore further the intersecting region
		if (ls::intersect(windowRegion(leftChildHandle).rect(), pos))
		{
			currentRegionHandle = leftChildHandle;
		}
		else if (ls::intersect(windowRegion(rightChildHandle).rect(), pos))
		{
			currentRegionHandle = rightChildHandle;
		}
		else return invalidHandle;
	}

	return currentRegionHandle;
}

bool WindowSpaceManager::Scene::tryDispatchEvent(sf::Event& event, const Vec2I& mousePos)
{
	switch (event.type)
	{
	case sf::Event::TextEntered:
		dispatchEvent(event.text, &SfmlEventHandler::onTextEntered, mousePos);
		return true;

	case sf::Event::KeyPressed:
		dispatchEvent(event.key, &SfmlEventHandler::onKeyPressed, mousePos);
		return true;

	case sf::Event::KeyReleased:
		dispatchEvent(event.key, &SfmlEventHandler::onKeyReleased, mousePos);
		return true;

	case sf::Event::MouseWheelMoved:
		dispatchEvent(event.mouseWheel, &SfmlEventHandler::onMouseWheelMoved, mousePos);
		return true;

	case sf::Event::MouseButtonPressed:
		dispatchEvent(event.mouseButton, &SfmlEventHandler::onMouseButtonPressed, mousePos);
		return true;

	case sf::Event::MouseButtonReleased:
		dispatchEvent(event.mouseButton, &SfmlEventHandler::onMouseButtonReleased, mousePos);
		return true;

	case sf::Event::MouseMoved:
		dispatchEvent(event.mouseMove, &SfmlEventHandler::onMouseMoved, mousePos);
		return true;

	default:
		return false;
	}
}

WindowSpaceManager::WindowSpaceManager(sf::RenderWindow& window) :
	m_window(window),
	m_scenes(),
	m_currentScene()
{

}

WindowSpaceManager::Scene& WindowSpaceManager::scene(const std::string& name)
{
	return m_scenes.at(name);
}
const WindowSpaceManager::Scene& WindowSpaceManager::scene(const std::string& name) const
{
	return m_scenes.at(name);
}

void WindowSpaceManager::changeScene(const std::string& name)
{
	m_currentScene = &(scene(name));
	m_currentScene->update(rect());
}
ls::Rectangle2I WindowSpaceManager::rect() const
{
	return ls::Rectangle2I::withSize(ls::Vec2I(0, 0), m_window.getSize().x, m_window.getSize().y);
}

void WindowSpaceManager::onWindowResized()
{
	m_currentScene->update(rect());
}

bool WindowSpaceManager::tryDispatchEvent(sf::Event& event)
{
	const sf::Vector2i m = sf::Mouse::getPosition(m_window);
	const Vec2I mousePos{ m.x, m.y };

	return m_currentScene->tryDispatchEvent(event, mousePos);
}