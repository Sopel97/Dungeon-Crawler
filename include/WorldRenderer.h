#pragma once

#include <SFML/Graphics.hpp>

#include "../LibS/Shapes.h"

#include "Camera.h"

#include "ResourceManager.h"

#include "tiles/TileOuterBorderCache.h"

#include <vector>

class Root;
class World;
class WindowSpaceManager;
class TallDrawable;
class TileLocation;
class SpriteBatch;
class Spritesheet;
class TileStack;

// IMPORTANT: SFML makes redundant shader binds that result in very bad fps when many calls are made with the same shader. Should be improved later when vertex array is used.
class WorldRenderer
{
public:
    WorldRenderer(Root& root, World& world);

    void moveCamera(const ls::Vec2F& displacement);
    void setCamera(const ls::Vec2F& position);

    const Camera& camera() const;

    void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates);

    static ls::Vec2F aligned(const ls::Vec2F& pos);

protected:
    struct TileOuterBorderCacheEntry
    {
        const TileStack* tileStack;
        TileOuterBorderCache rendererCache;
    };

    Root& m_root;
    World& m_world;
    WindowSpaceManager& m_windowSpaceManager;

    ls::Array2<std::vector<TileOuterBorderCacheEntry>> m_outerBorderCache;
    bool m_isOuterBorderCached;

    Camera m_camera;
    
    sf::RenderTexture m_intermidiateRenderTarget;
    sf::RenderTexture m_metaTexture;
    sf::RenderTexture m_lightMap;

    sf::Shader m_intermidiateDepthShader;
    sf::Shader m_metaDepthShader;
    sf::Shader m_prettyStretchShader;
    sf::Shader m_lightShader;

    ResourceHandle<Spritesheet> m_lightTexture;

    static constexpr int m_viewWidth = 15;
    static constexpr int m_viewHeight = 15;

    static constexpr int m_tileResolution = 32;
    
protected:
    void prepareIntermidiateRenderTarget();
    void prepareLightMap();
    void prepareMetaTexture();
    void updateShaderUniforms();
    void buildOuterBorderCache();
    void updateOuterBorderCache(const TileLocation & tileLocation);
    void drawOuterBorder(SpriteBatch& spriteBatch, const TileLocation & tileLocation);
    void drawMain(const sf::RenderStates& renderStates);
    void drawMeta(const sf::RenderStates& renderStates);
    void drawIntermidiate(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates);
    void drawLightMapToIntermidiate(const sf::RenderStates& renderStates);
    void drawLightsToLightMap();
    void alignVertices(SpriteBatch& batch) const;
};