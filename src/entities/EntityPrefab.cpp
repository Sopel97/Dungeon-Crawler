#include "..\..\include\entities\EntityPrefab.h"

#include "entities/controllers/EntityController.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
#include "entities/Entity.h"

int EntityPrefab::m_lastId = -1;

EntityPrefab::EntityPrefab(std::unique_ptr<EntityModel> model, std::unique_ptr<EntityRenderer> renderer, std::unique_ptr<EntityController> controller)
{
    m_modelCommonData = model->createCommonDataStorage();
    m_rendererCommonData = renderer->createCommonDataStorage();
    m_controllerCommonData = controller->createCommonDataStorage();

    model->setCommonDataStorage(*m_modelCommonData);
    renderer->setCommonDataStorage(*m_rendererCommonData);
    controller->setCommonDataStorage(*m_controllerCommonData);

    m_entity = std::make_unique<Entity>(++m_lastId, std::move(model), std::move(renderer), std::move(controller));
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
