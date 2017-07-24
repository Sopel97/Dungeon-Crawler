#include "WorldRenderer.h"

#include "World.h"

#include "Root.h"
#include "Game.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/models/TileModel.h"
#include "tiles/controllers/TileController.h"

#include "TileLocation.h"

#include "entities/Entity.h"
#include "entities/EntityPrefab.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"

#include "projectiles/Projectile.h"
#include "projectiles/ProjectilePrefab.h"
#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/controllers/ProjectileController.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "GameTime.h"

#include "window/WindowSpaceManager.h"

#include "Light.h"

#include "TileTransferMediator.h"

#include "SpriteBatch.h"

#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

#include "../LibS/Util.h"

#include "GameConstants.h"

#include "Logger.h"

using namespace ls;

WorldRenderer::WorldRenderer(Root& root, World& world) :
    m_root(root),
    m_world(world),
    m_camera(Vec2F(World::m_worldWidth * GameConstants::tileSize / 2.0f, World::m_worldHeight * GameConstants::tileSize / 2.0f), m_viewWidth * GameConstants::tileSize, m_viewHeight * GameConstants::tileSize),
    m_windowSpaceManager(root.windowSpaceManager())
{
    m_intermidiateRenderTarget.create(m_viewWidth * GameConstants::tileSize, m_viewHeight * GameConstants::tileSize, true);
    m_intermidiateRenderTarget.resetGLStates();
    m_intermidiateRenderTarget.setActive(true);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_metaTexture.create((m_viewWidth + 2) * GameConstants::tileSize, (m_viewHeight + 2) * GameConstants::tileSize, true);
    m_metaTexture.resetGLStates();
    m_metaTexture.setActive(true);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //light map stretches one more tile, because some of the visible tiles need lighting of the tiles not seen
    //also adds one tile length on top and left to keep symmetry
    m_lightMap.create((m_viewWidth + 2) * GameConstants::tileSize, (m_viewHeight + 2) * GameConstants::tileSize);

    m_intermidiateDepthShader.loadFromFile("assets/shaders/intermidiate_depth.vert", "assets/shaders/intermidiate_depth.frag");
    m_metaDepthShader.loadFromFile("assets/shaders/meta_depth.vert", "assets/shaders/meta_depth.frag");

    //TODO: maybe as a resource
    m_prettyStretchShader.loadFromFile("assets/shaders/pretty_stretch.vert", "assets/shaders/pretty_stretch.frag");
    m_prettyStretchShader.setParameter("sourceTextureSize", sf::Vector2f(m_intermidiateRenderTarget.getSize().x, m_intermidiateRenderTarget.getSize().y));

    m_lightTexture = ResourceManager::instance().get<sf::Texture>("LightDisc");

    m_lightShader.loadFromFile("assets/shaders/light.vert", "assets/shaders/light.frag");
    m_lightShader.setParameter("lightTextureSize", sf::Vector2f(m_lightMap.getSize().x, m_lightMap.getSize().y));
    m_lightShader.setParameter("metaTextureSize", sf::Vector2f(m_metaTexture.getSize().x, m_metaTexture.getSize().y));
    m_lightShader.setParameter("metaTexture", m_metaTexture.getTexture());
}

void WorldRenderer::moveCamera(const Vec2F& displacement)
{
    m_camera.move(displacement);
}
void WorldRenderer::setCamera(const Vec2F& position)
{
    m_camera.setCenter(position);
}
const Camera& WorldRenderer::camera() const
{
    return m_camera;
}

void WorldRenderer::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    prepareIntermidiateRenderTarget();
    prepareLightMap();
    prepareMetaTexture();
    updateShaderUniforms();

    drawMain(renderStates);
    drawMeta(renderStates);
    drawLightsToLightMap();

    drawLightMapToIntermidiate(renderStates);
    drawIntermidiate(renderTarget, renderStates);
}

void WorldRenderer::drawMain(const sf::RenderStates& renderStates)
{
    sf::RenderStates colorRenderStates = renderStates;
    colorRenderStates.shader = &m_intermidiateDepthShader;

    MapLayer& mapLayer = *(m_world.m_mapLayer);
    const Rectangle2F cameraRect = m_camera.viewRectangle();
    const Vec2F& cameraTopLeft = cameraRect.min;
    const Vec2F& cameraBottomRight = cameraRect.max;
    int firstTileX = std::max(Util::fastFloor(cameraTopLeft.x / GameConstants::tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(cameraTopLeft.y / GameConstants::tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(cameraBottomRight.x / GameConstants::tileSize) + 1, m_world.m_width - 1);
    int lastTileY = std::min(Util::fastFloor(cameraBottomRight.y / GameConstants::tileSize) + 1, m_world.m_height - 1);

    SpriteBatch spriteBatch;
    
    //x,y are inverted here because we want to draw top down
    for (int y = firstTileY; y <= lastTileY; ++y)
    {
        for (int x = firstTileX; x <= lastTileX; ++x)
        {
            TileColumn& tileColumn = mapLayer.at(x, y);
            int z = 0;
            for (TileStack& tileStack : tileColumn.tiles())
            {
                TileLocation location(mapLayer, x, y, z);

                if (z == 0)
                {
                    if (tileStack.tile().renderer().coversOuterBorders())
                    {
                        drawOuterBorder(spriteBatch, location);
                        tileStack.tile().renderer().draw(spriteBatch, location);
                    }
                    else
                    {
                        tileStack.tile().renderer().draw(spriteBatch, location);
                        drawOuterBorder(spriteBatch, location);
                    }
                }
                else
                {
                    tileStack.tile().renderer().draw(spriteBatch, location);
                }

                ++z;
            }
        }
    }

    for (Entity* visibleEntity : m_world.m_entitySystem.getVisibleEntities(m_camera))
    {
        visibleEntity->renderer().draw(spriteBatch);
    }
    for (Projectile* visibleProjectile : m_world.m_projectileSystem.getVisibleProjectiles(m_camera))
    {
        visibleProjectile->renderer().draw(spriteBatch);
    }

    m_intermidiateRenderTarget.draw(spriteBatch, colorRenderStates);

    m_intermidiateRenderTarget.display();
}

void WorldRenderer::prepareIntermidiateRenderTarget()
{
    const Rectangle2F cameraRect = m_camera.viewRectangle();

    const auto& cameraCenter = cameraRect.centerOfMass();
    sf::View intermidiateView = m_intermidiateRenderTarget.getDefaultView();
    intermidiateView.setCenter(cameraCenter.x, cameraCenter.y);
    intermidiateView.setSize(cameraRect.width(), cameraRect.height());
    m_intermidiateRenderTarget.setView(intermidiateView);

    m_intermidiateRenderTarget.setActive(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void WorldRenderer::prepareLightMap()
{
    const Rectangle2F cameraRect = m_camera.viewRectangle();
    const Vec2F cameraCenter = cameraRect.centerOfMass();

    sf::View lightMapView = m_lightMap.getDefaultView();
    lightMapView.setCenter(cameraCenter.x, cameraCenter.y);
    lightMapView.setSize(cameraRect.width() + 2 * GameConstants::tileSize, cameraRect.height() + 2 * GameConstants::tileSize);
    m_lightMap.setView(lightMapView);

    m_lightMap.clear(sf::Color::Black);
}
void WorldRenderer::prepareMetaTexture()
{
    const Rectangle2F cameraRect = m_camera.viewRectangle();
    const Vec2F cameraCenter = cameraRect.centerOfMass();

    sf::View metaTextureView = m_metaTexture.getDefaultView();
    metaTextureView.setCenter(cameraCenter.x, cameraCenter.y);
    metaTextureView.setSize(cameraRect.width() + 2 * GameConstants::tileSize, cameraRect.height() + 2 * GameConstants::tileSize);
    m_metaTexture.setView(metaTextureView);

    m_metaTexture.setActive(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void WorldRenderer::updateShaderUniforms()
{
    const Rectangle2F cameraRect = m_camera.viewRectangle();

    m_prettyStretchShader.setParameter("worldOffset", sf::Vector2f(cameraRect.min.x, cameraRect.min.y));
    const auto& viewRect = m_world.window().absoluteContentRect();
    // TODO: for some complicated reason the shader does not work properly when windows have borders (not very problematic since they shouldn't)
    // TODO: investigate
    m_prettyStretchShader.setParameter("viewOffset", sf::Vector2f(viewRect.min.x, viewRect.min.y));
    m_prettyStretchShader.setParameter("destinationTextureSize", sf::Vector2f(viewRect.width(), viewRect.height()));
}
void WorldRenderer::drawMeta(const sf::RenderStates& renderStates)
{
    sf::RenderStates metaRenderStates = renderStates;
    metaRenderStates.shader = &m_metaDepthShader;

    MapLayer& mapLayer = *(m_world.m_mapLayer);
    const Rectangle2F cameraRect = m_camera.viewRectangle();
    const Vec2F& cameraTopLeft = cameraRect.min;
    const Vec2F& cameraBottomRight = cameraRect.max;
    int firstTileX = std::max(Util::fastFloor(cameraTopLeft.x / GameConstants::tileSize), 0);
    int firstTileY = std::max(Util::fastFloor(cameraTopLeft.y / GameConstants::tileSize), 0);
    int lastTileX = std::min(Util::fastFloor(cameraBottomRight.x / GameConstants::tileSize) + 1, m_world.m_width - 1);
    int lastTileY = std::min(Util::fastFloor(cameraBottomRight.y / GameConstants::tileSize) + 1, m_world.m_height - 1);

    SpriteBatch spriteBatch;

    //x,y are inverted here because we want to draw top down
    for (int y = firstTileY; y <= lastTileY; ++y)
    {
        for (int x = firstTileX; x <= lastTileX; ++x)
        {
            TileColumn& tileColumn = mapLayer.at(x, y);
            int z = 0;
            for (TileStack& tileStack : tileColumn.tiles())
            {
                TileLocation location(mapLayer, x, y, z);

                tileStack.tile().renderer().drawMeta(spriteBatch, location);

                ++z;
            }
        }
    }

    for (Entity* visibleEntity : m_world.m_entitySystem.getVisibleEntities(m_camera))
    {
        visibleEntity->renderer().drawMeta(spriteBatch);
    }
    for (Projectile* visibleProjectile : m_world.m_projectileSystem.getVisibleProjectiles(m_camera))
    {
        visibleProjectile->renderer().drawMeta(spriteBatch);
    }

    m_metaTexture.draw(spriteBatch, metaRenderStates);

    m_metaTexture.display();
}
void WorldRenderer::drawIntermidiate(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    sf::Sprite intermidiateFinal(m_intermidiateRenderTarget.getTexture());
    sf::RenderStates intermidiateRenderStates = renderStates;
    intermidiateRenderStates.shader = &m_prettyStretchShader;

    m_windowSpaceManager.setContentView(m_world.window(), Rectangle2F::withSize(Vec2F(0, 0), m_intermidiateRenderTarget.getSize().x, m_intermidiateRenderTarget.getSize().y));

    renderTarget.draw(intermidiateFinal, intermidiateRenderStates);
}
void WorldRenderer::drawLightMapToIntermidiate(const sf::RenderStates& renderStates)
{
    const Rectangle2F cameraRect = m_camera.viewRectangle();
    const Vec2F cameraCenter = cameraRect.centerOfMass();

    sf::RectangleShape lightMapSprite;
    lightMapSprite.setTexture(&(m_lightMap.getTexture()));
    lightMapSprite.setSize(sf::Vector2f(cameraRect.width(), cameraRect.height()));
    //converts to world coordinates
    lightMapSprite.setPosition(sf::Vector2f(cameraCenter.x - cameraRect.width() / 2, cameraCenter.y - cameraRect.height() / 2));
    lightMapSprite.setTextureRect(sf::IntRect(GameConstants::tileSize, GameConstants::tileSize, cameraRect.width(), cameraRect.height()));

    sf::RenderStates lightMapRenderStates = renderStates;
    lightMapRenderStates.shader = &m_lightShader;
    lightMapRenderStates.blendMode = sf::BlendMultiply;

    m_intermidiateRenderTarget.draw(lightMapSprite, lightMapRenderStates);
}

void WorldRenderer::drawLightsToLightMap()
{
    sf::RenderStates lightRenderStates;
    lightRenderStates.blendMode = sf::BlendAdd;

    auto lights = m_world.m_entitySystem.queryLights(m_camera.viewRectangle());
    for (auto& light : lights)
    {
        light.draw(m_lightMap, lightRenderStates, m_lightTexture.get());
    }

    m_lightMap.display();
}

void WorldRenderer::drawOuterBorder(SpriteBatch& spriteBatch, const TileLocation& tileLocation)
{
    auto areTilesEqual = [](const TileStack * lhs, const TileStack * rhs)->bool {return lhs->tile().id() == rhs->tile().id(); };
    auto borderPriorityCompare = [](const TileStack * lhs, const TileStack * rhs)->bool {return lhs->tile().renderer().outerBorderPriority() < rhs->tile().renderer().outerBorderPriority(); };

    int x = tileLocation.x;
    int y = tileLocation.y;
    const MapLayer& map = *tileLocation.map;

    std::vector<const TileStack*> differentNeigbourTiles;
    int thisTileOuterBorderPriority = map.at(x, y, 0).tile().renderer().outerBorderPriority();
    for (int xoffset = -1; xoffset <= 1; ++xoffset)
    {
        for (int yoffset = -1; yoffset <= 1; ++yoffset)
        {
            if (xoffset == 0 && yoffset == 0) continue;
            int xx = x + xoffset;
            int yy = y + yoffset;
            const TileStack& tileStack = map.at(xx, yy, 0);
            if (!tileStack.tile().renderer().hasOuterBorder() || tileStack.tile().renderer().outerBorderPriority() <= thisTileOuterBorderPriority) continue;

            bool firstSuchNeighbour = true;
            for (const auto& neighbour : differentNeigbourTiles)
            {
                if (areTilesEqual(&tileStack, neighbour))
                {
                    firstSuchNeighbour = false;
                    break;
                }
            }
            if (firstSuchNeighbour)
            {
                differentNeigbourTiles.push_back(&tileStack);
            }
        }
    }
    std::sort(differentNeigbourTiles.begin(), differentNeigbourTiles.end(), borderPriorityCompare);

    for (const auto& neighbour : differentNeigbourTiles)
    {
        neighbour->tile().renderer().drawOutside(spriteBatch, tileLocation);
    }
}