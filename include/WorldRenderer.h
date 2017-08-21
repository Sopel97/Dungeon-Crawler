#pragma once

#include <SFML/Graphics.hpp>

#include "../LibS/Shapes.h"

#include "Camera.h"

#include "ResourceManager.h"

#include "Light.h"

#include "tiles/TileOuterBorderCache.h"

#include "WorkerThread.h"

#include <vector>

class Root;
class World;
class WindowSpaceManager;
class TallDrawable;
class TileLocation;
class SpriteBatch;
class Spritesheet;
class TileStack;
class Light;

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
        ls::Vec2I tileStackPos;
        TileOuterBorderCache rendererCache;
    };

    
    struct LightOccluderCache
    {
        ls::Array2<std::optional<ls::Rectangle2F>> occluders;
        ls::Rectangle2I range;
    };

    struct LightGeometryEntry
    {
        Light light;
        std::vector<sf::Vertex> vertices;
        bool toRemove;
    };
    
    struct LightGeometryCache
    {
        std::vector<LightGeometryEntry> lights;
        int current;
    };

    Root& m_root;
    World& m_world;
    WindowSpaceManager& m_windowSpaceManager;

    int m_numFramesDrawn;

    ls::Array2<std::vector<TileOuterBorderCacheEntry>> m_outerBorderCache;
    bool m_isOuterBorderCached;

    LightOccluderCache m_lightOccluderCache;

    LightGeometryCache m_lightGeometryCache;

    Camera m_camera;

    WorkerThread m_workerThread;
    
    sf::RenderTexture m_intermidiateRenderTarget;
    sf::RenderTexture m_metaTexture;
    sf::RenderTexture m_lightMap;
    sf::RenderTexture m_lightMapVerticalBlur;
    sf::RenderTexture m_lightMapBlur;

    sf::Shader m_intermidiateDepthShader;
    sf::Shader m_metaDepthShader;
    sf::Shader m_prettyStretchShader;
    sf::Shader m_lightShader;
    sf::Shader m_blurShader;

    ResourceHandle<Spritesheet> m_lightTexture;

    static constexpr int m_viewWidth = 15;
    static constexpr int m_viewHeight = 15;

    static constexpr int m_tileResolution = 32;

    static constexpr int m_maxLightRadius = 4;

    static constexpr int m_lightOccluderCacheUpdateInterval = 2;

    static constexpr int m_lightGeometryUpdatesPerFrame = 10;
    
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
    std::vector<sf::Vertex> generateLightGeometry(const Light& light);
    void drawLightGeometryToLightMap();
    void alignVertices(SpriteBatch& batch) const;
    void updateLightOccluderCache();
    std::vector<ls::Rectangle2F> queryLightOccluders(const Light& light) const;
    void updateLightGeometryCache();
};