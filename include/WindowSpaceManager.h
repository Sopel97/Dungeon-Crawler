#ifndef WINDOWSPACEMANAGER_H
#define WINDOWSPACEMANAGER_H

#include "../LibS/Geometry.h"
#include "../LibS/BinaryTree.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <map>
#include <optional>
#include <utility>

#include "SfmlEventHandler.h"

class WindowSpaceUser;

class WindowSpaceManager
{
	/*
public:
	enum class Scene
	{
		Main
	};
	class Region
	{
	public:
		enum Id
		{
			World,
			PlayerUi
		};

		Region() = default;
		Region(const ls::Rectangle2I& rect, Id id, const std::vector<Scene>& parentScenes, int zIndex = 0);
		Region(const Region&) = default;
		Region(Region&&) = default;

		Region& operator=(const Region&) = default;
		Region& operator=(Region &&) = default;

		ls::Vec2I localCoords(const ls::Vec2I& windowCoords) const;
		const ls::Rectangle2I& rect() const;
		Id id() const;
		const std::vector<Scene>& parentScenes() const;
		bool isPresentOnScene(Scene scene) const;
		int zIndex() const;
	protected:
		ls::Rectangle2I m_rect;
		Id m_id;
		std::vector<Scene> m_parentScenes;
		int m_zIndex; //determines priority. Higher means it will be more to the top
	};
public:
	WindowSpaceManager(sf::RenderWindow& window);

	sf::View viewOfRect(const ls::Rectangle2I& windowViewRect, const ls::Rectangle2F& worldViewRect);
	sf::View viewOfRegion(Region::Id regionId);
	sf::View viewOfRegion(Region::Id regionId, const ls::Rectangle2F& worldViewRect);

	void setViewToRect(const ls::Rectangle2I& windowViewRect, const ls::Rectangle2F& worldViewRect);
	void setViewToRegion(Region::Id regionId);
	void setViewToRegion(Region::Id regionId, const ls::Rectangle2F& worldViewRect);

	void setDefaultView();
	void updateRegions();

	Scene currentScene() const;
	void setCurrentScene(Scene scene);

	const std::map<Region::Id, Region>& regions() const;
	const Region& region(Region::Id regionId) const;
	const Region* pointedRegion(const ls::Vec2I& windowCoords) const;
	const ls::Rectangle2I& regionRect(Region::Id regionId) const;

	void onWindowResized(const sf::Event& windowResizedEvent);
protected:
	sf::RenderWindow& m_window;
	Scene m_currentScene;

	std::map<Region::Id, Region> m_regions;

	*/
public:
	class WindowRegion;
	class Scene;
	class SubdivisionParams;

	using WindowRegionStorageNode = std::pair<WindowRegion, std::optional<SubdivisionParams>>;
	using WindowRegionStorage = ls::BinaryTree<WindowRegionStorageNode>;
	using WindowRegionHandle = typename WindowRegionStorage::NodeHandle;
	static constexpr WindowRegionHandle invalidHandle = WindowRegionStorage::invalidHandle;

	struct WindowRegionFullLocalization
	{
		WindowSpaceManager* windowSpaceManager;
		Scene* scene;
		WindowRegionHandle regionHandle;
	};

	class SubdivisionParams
	{
	public:
		enum class Orientation
		{
			Horizontal,
			Vertical
		};
		enum class Subject
		{
			First,
			Second
		};

	private:
		enum class AmountType
		{
			Pixels,
			Ratio
		};
		union Amount
		{
			int pixels;
			float ratio;
		};

		Amount m_amount;
		AmountType m_amountType;
		Subject m_subject;
		Orientation m_orientation;
	public:

		static SubdivisionParams withPixels(Orientation orientation, Subject subject, int pixels);
		static SubdivisionParams withRatio(Orientation orientation, Subject subject, float ratio);

		SubdivisionParams(const SubdivisionParams& other) = default;
		SubdivisionParams(SubdivisionParams&& other) = default;

		SubdivisionParams& operator=(const SubdivisionParams& other) = default;
		SubdivisionParams& operator=(SubdivisionParams&& other) = default;

		std::pair<ls::Rectangle2I, ls::Rectangle2I> calculateSubRects(const ls::Rectangle2I& rect) const;

		void setPixels(int newPixels);
		void setRatio(float newRatio);

		void moveByPixels(const ls::Rectangle2I& rect, int deltaPixels);
		void moveByRatio(const ls::Rectangle2I& rect, float deltaRatio);

		int asPixels(const ls::Rectangle2I& rect) const;
		float asRatio(const ls::Rectangle2I& rect) const;

	private:
		SubdivisionParams(Orientation orientation, Subject subject, int pixels);
		SubdivisionParams(Orientation orientation, Subject subject, float ratio);
	};

	class WindowRegion
	{
	private:
		ls::Rectangle2I m_rect;
		std::string m_name;
		Scene* m_parentScene;
		WindowSpaceUser* m_spaceUser;

	public:
		WindowRegion(const ls::Rectangle2I& rect, const std::string& name, Scene& parentScene);

		const ls::Rectangle2I& rect() const;
		const std::string& name() const;
		const Scene& parentScene() const;
		Scene& parentScene();

		bool hasEventHandler() const;
		SfmlEventHandler& eventHandler();

		void setRect(const ls::Rectangle2I& newRect);
		void setUser(WindowSpaceUser& newUser);

		ls::Vec2I localCoords(const ls::Vec2I& windowCoords) const;

		void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
	};


	class Scene
	{
	private:
		WindowSpaceManager* m_windowSpaceManager;
		WindowRegionStorage m_windowRegions;
		std::string m_name;
		std::vector<WindowRegionHandle> m_topmostRegions;
		WindowRegionHandle m_focusedRegionHandle;

	public:
		Scene(WindowSpaceManager& windowSpaceManager, const ls::Rectangle2I& rect, const std::string& name);

		WindowRegion& windowRegion(WindowRegionHandle h);
		const WindowRegion& windowRegion(WindowRegionHandle h) const;

		bool isSubdivided(WindowRegionHandle h) const;

		SubdivisionParams& subdivisionParams(WindowRegionHandle h);
		const SubdivisionParams& subdivisionParams(WindowRegionHandle h) const;

		std::pair<WindowRegionHandle, WindowRegionHandle> subdivide(WindowRegionHandle h, const SubdivisionParams& params, const std::string& nameFirst, const std::string& nameSecond);

		WindowRegionHandle firstChild(WindowRegionHandle h) const;
		WindowRegionHandle secondChild(WindowRegionHandle h) const;
		WindowRegionHandle parent(WindowRegionHandle h) const;
		bool hasChildren(WindowRegionHandle h) const;
		bool hasParent(WindowRegionHandle h) const;

		WindowRegionHandle rootHandle() const;

		WindowRegionHandle find(const std::string& name) const;

		WindowRegionFullLocalization fullLocalizationOf(const std::string& name);

		void update(const ls::Rectangle2I& rect);

		WindowRegionHandle queryRegion(const ls::Vec2I& pos) const;

		bool tryDispatchEvent(sf::Event& event, const ls::Vec2I& mousePos);

		void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

	private:
		void setSubdivisionParams(WindowRegionHandle h, const SubdivisionParams& params);
		void update(WindowRegionHandle h, const ls::Rectangle2I& rect);

		template <class EventType>
		void dispatchEvent(EventType& event, SfmlEventHandler::EventResult(SfmlEventHandler::*handler)(EventType&, SfmlEventHandler::EventContext), const Vec2I& mousePos)
		{
			{
				WindowRegionHandle focusedRegionHandle = m_focusedRegionHandle; //stored because can be changed midway
				WindowRegion& focused = windowRegion(focusedRegionHandle);
				if (focused.hasEventHandler())
				{
					const SfmlEventHandler::EventContext context{ true, ls::intersect(focused.rect(), mousePos) };
					const SfmlEventHandler::EventResult result = (focused.eventHandler().*handler)(event, context);
					if (result.consumeEvent)
					{
						return;
					}
				}
			}

			// other regions

			WindowRegionHandle mouseOverRegionHandle = queryRegion(mousePos);
			if (mouseOverRegionHandle != m_focusedRegionHandle)
			{
				WindowRegion& mouseOverRegion = windowRegion(mouseOverRegionHandle);
				if (mouseOverRegion.hasEventHandler())
				{
					const SfmlEventHandler::EventContext context{ false, true };
					const SfmlEventHandler::EventResult result = (mouseOverRegion.eventHandler().*handler)(event, context);
					if (result.takeFocus)
					{
						m_focusedRegionHandle = mouseOverRegionHandle;
					}
					if (result.consumeEvent)
					{
						return;
					}
				}
			}

			for (WindowRegionHandle h : m_topmostRegions)
			{
				if (h == focusedRegionHandle || h == mouseOverRegionHandle) continue;

				WindowRegion& region = windowRegion(h);
				if (region.hasEventHandler())
				{
					const SfmlEventHandler::EventContext context{ false, false };
					const SfmlEventHandler::EventResult result = (region.eventHandler().*handler)(event, context);
					if (result.takeFocus)
					{
						m_focusedRegionHandle = h;
					}
					if (result.consumeEvent)
					{
						return;
					}
				}
			}
		}
	};

	WindowSpaceManager(sf::RenderWindow& window);

	Scene& createScene(const std::string& name);

	Scene& scene(const std::string& name);
	const Scene& scene(const std::string& name) const;

	void changeScene(const std::string& name);

	ls::Rectangle2I rect() const;

	void onWindowResized();

	sf::View getRectView(const ls::Rectangle2I& rect, const ls::Rectangle2F& viewport) const;
	sf::View getRegionView(const WindowRegion& region) const;
	sf::View getRegionView(const WindowRegion& region, const ls::Rectangle2F& viewport) const;
	void setRectView(const ls::Rectangle2I& rect, const ls::Rectangle2F& viewport);
	void setRegionView(const WindowRegion& region);
	void setRegionView(const WindowRegion& region, const ls::Rectangle2F& viewport);
	void setDefaultView();

	bool tryDispatchEvent(sf::Event& event);

	void drawCurrentScene(sf::RenderStates& renderStates);

private:
	sf::RenderWindow& m_window;
	std::map<std::string, Scene> m_scenes;
	Scene* m_currentScene;
};


#endif // WINDOWSPACEMANAGER_H
