#pragma once

#include <SFML/Graphics.hpp>

#include "../LibS/Shapes.h"

#include "Camera.h"

#include "ResourceManager.h"

class Root;
class World;
class WindowSpaceManager;
class TallDrawable;
class TileLocation;

class WorldRenderer
{
public:
    WorldRenderer(Root& root, World& world);

    void moveCamera(const ls::Vec2F& displacement);
    void setCamera(const ls::Vec2F& position);

    const Camera& camera() const;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    Root& m_root;
    World& m_world;
    WindowSpaceManager& m_windowSpaceManager;

    Camera m_camera;
    
    sf::RenderTexture m_intermidiateRenderTarget;
    sf::RenderTexture m_lightMap;
    sf::RenderTexture m_metaTexture;
    sf::Shader m_prettyStretchShader;
    sf::Shader m_lightShader;

    ResourceHandle<sf::Texture> m_lightTexture;

    static constexpr int m_viewWidth = 15;
    static constexpr int m_viewHeight = 15;
    
protected:
    void prepareIntermidiateRenderTarget();
    void prepareLightMap();
    void prepareMetaTexture();
    void updateShaderUniforms();
    void drawOuterBorder(sf::RenderTarget & renderTarget, sf::RenderStates & renderStates, const TileLocation & tileLocation);
    void drawMeta(sf::RenderStates& renderStates, const std::vector<TallDrawable*>& tallDrawables);
    void drawIntermidiate(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawLightMapToIntermidiate(sf::RenderStates& renderStates);
    void drawLightsToLightMap();
};