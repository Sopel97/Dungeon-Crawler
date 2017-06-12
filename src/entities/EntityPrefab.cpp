#include "..\..\include\entities\EntityPrefab.h"

#include "entities/controllers/EntityController.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
#include "entities/Entity.h"

int EntityPrefab::m_lastId = -1;

EntityPrefab::EntityPrefab(
    const ComponentFactory<Entity, EntityModel>& modelFac,
    const ComponentFactory<Entity, EntityRenderer>& rendererFac,
    const ComponentFactory<Entity, EntityController>& controllerFac)
{
    m_entity = std::make_unique<Entity>(++m_lastId, modelFac, rendererFac, controllerFac);

    EntityModel& model = m_entity->model();
    EntityRenderer& renderer = m_entity->renderer();
    EntityController& controller = m_entity->controller();

    m_modelCommonData = model.createCommonDataStorage();
    m_rendererCommonData = renderer.createCommonDataStorage();
    m_controllerCommonData = controller.createCommonDataStorage();

    model.setCommonDataStorage(*m_modelCommonData);
    renderer.setCommonDataStorage(*m_rendererCommonData);
    controller.setCommonDataStorage(*m_controllerCommonData);
}

EntityPrefab::~EntityPrefab()
{
}

void EntityPrefab::loadFromConfiguration(ConfigurationNode& config)
{
    m_entity->loadFromConfiguration(config);
}

std::unique_ptr<Entity> EntityPrefab::instantiate() const
{
    return m_entity->instantiate();
}
