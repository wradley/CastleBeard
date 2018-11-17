#include <iostream>
#include "GraphicsSystem.h"
#include "Debug.h"


namespace Engine
{
    GraphicsSystem::GraphicsSystem()
    {}


    GraphicsSystem::~GraphicsSystem()
    {}


    void GraphicsSystem::init(EventManager & em)
    {
        em.listenFor(EventType::eCreateEntity, &_eventQueue);
        em.listenFor(EventType::eUnloadEntity, &_eventQueue);
        em.listenFor(EventType::eAddModelComponent, &_eventQueue);
        em.listenFor(EventType::eAddCameraComponent, &_eventQueue);
        em.listenFor(EventType::eModCameraComponent, &_eventQueue);
        em.listenFor(EventType::eTransformEntity, &_eventQueue);
        em.listenFor(EventType::eTransformEntities, &_eventQueue);
        em.listenFor(EventType::eResizeWindow, &_eventQueue);
        _renderer = new Renderer::Renderer;
    }


    void GraphicsSystem::update(EventManager & em)
    {
        handleEvents();
        _renderer->draw();
    }


    void GraphicsSystem::stop(EventManager & em)
    {
        handleEvents();
    }


    void GraphicsSystem::handleEvents()
    {
        unsigned int numEvents = _eventQueue.size();
        for (unsigned int i = 0; i < numEvents; ++i) {
            onEvent(_eventQueue.popFront());
        }
    }


    void GraphicsSystem::onEvent(std::shared_ptr<const Event> e)
    {
        switch (e->getType())
        {
        case EventType::eCreateEntity:
            onCreateEntity((const CreateEntityEvent*) e.get());
            break;
        case EventType::eUnloadEntity:
            onUnloadEntity((const UnloadEntityEvent*) e.get());
            break;
        case EventType::eAddModelComponent:
            onAddModel((const AddModelComponentEvent*) e.get());
            break;
        case EventType::eAddCameraComponent:
            onAddCamera((const AddCameraComponentEvent*) e.get());
            break;
        case EventType::eModCameraComponent:
            onModCamera((const ModCameraComponentEvent*) e.get());
            break;
        case EventType::eTransformEntity:
            onTransformEntity((const TransformEntityEvent*) e.get());
            break;
        case EventType::eTransformEntities:
            onTransformEntities((const TransformEntitiesEvent*) e.get());
            break;
        case EventType::eResizeWindow:
            onResizeWindow((const ResizeWindowEvent*) e.get());
            break;
        default:
            //    DEBUG_LOG("Default case");
            break;
        }
    }


    void GraphicsSystem::onCreateEntity(const CreateEntityEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        // add components
        for (auto ac : e->components) {
            onEvent(ac);
        }
    }


    void GraphicsSystem::onUnloadEntity(const UnloadEntityEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        DEBUG_LOG("Impl. unload");
    }


    void GraphicsSystem::onAddModel(const AddModelComponentEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        // make sure model component does not exist
        if (_componentsToModels.find(e->component) != _componentsToModels.end()) {
            DEBUG_LOG("Redefined model component");
            return;
        }

        // check if we have created an entity yet
        if (_entitiesToScenes.find(e->entity) == _entitiesToScenes.end()) {
            createSceneToEntity(e->entity, e->entityTform, e->lineage);
        }

        // add model to entity
        Renderer::Scene *scene = _entitiesToScenes[e->entity];
        Renderer::Model *model = scene->createModel();
        model->loadFile(e->filepath);
        _componentsToModels[e->component] = model;
    }


    void GraphicsSystem::onAddCamera(const AddCameraComponentEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        // make sure camera component does not exist
        if (_componentsToCameras.find(e->component) != _componentsToCameras.end()) {
            DEBUG_LOG("Redefined camera component");
            return;
        }

        // check if we have created an entity yet
        if (_entitiesToScenes.find(e->entity) == _entitiesToScenes.end()) {
            createSceneToEntity(e->entity, e->entityTform, e->lineage);
        }

        // add camera to entity
        Renderer::Scene *scene = _entitiesToScenes[e->entity];
        Renderer::Camera *camera = scene->createCamera();
        camera->setAspectRatio(e->aspectRatio);
        camera->setFarPlane(e->farPlane);
        camera->setFieldOfView(e->fieldOfView);
        camera->setLocalTransform(e->componentTform);
        camera->setNearPlane(e->nearPlane);
        _componentsToCameras[e->component] = camera;

        // TODO: better way to set main camera
        _renderer->setCamera(camera);
    }


    void GraphicsSystem::onModCamera(const ModCameraComponentEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        // check if there is proper mapping
        if (_componentsToCameras.find(e->component) == _componentsToCameras.end()) {
            DEBUG_LOG("Camera Component [" + std::to_string(e->component) + "] does not exist in graphics mapping");
            return;
        }

        Renderer::Camera *camera = _componentsToCameras[e->component];
        if (e->modAspectRatio)    camera->setAspectRatio(e->aspectRatio);
        if (e->modComponentTForm) camera->setLocalTransform(e->componentTform);
        if (e->modFarPlane)       camera->setFarPlane(e->farPlane);
        if (e->modFieldOfView)    camera->setFieldOfView(e->fieldOfView);
        if (e->modNearPlane)      camera->setNearPlane(e->nearPlane);
    }


    void GraphicsSystem::onTransformEntity(const TransformEntityEvent *e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        // check if there is proper mapping
        if (_entitiesToScenes.find(e->entity) == _entitiesToScenes.end()) {
            DEBUG_LOG("Entity [" + std::to_string(e->entity) + "] does not exist in graphics mapping");
            return;
        }

        _entitiesToScenes[e->entity]->setLocalTransform(e->transform);
    }


    void GraphicsSystem::onTransformEntities(const TransformEntitiesEvent *e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        for (auto te : e->transformEvents) {
            onTransformEntity(te.get());
        }
    }


    void GraphicsSystem::onResizeWindow(const ResizeWindowEvent * e)
    {
        if (!e) {
            DEBUG_LOG("Given null event");
            return;
        }

        float aspect = (float)e->newWidth / (float)e->newHeight;

        for (auto pair : _componentsToCameras) {
            pair.second->setAspectRatio(aspect);
        }

        _renderer->setViewport_TEMP_((int)e->newWidth, (int)e->newHeight);
    }


    void GraphicsSystem::createSceneToEntity(
        unsigned int entity, const Math::Transform &transform,
        const std::vector<std::tuple<unsigned int, Math::Transform>> &lineage
    ) {
        Renderer::Scene *scene;

        // check if entity has lineage
        if (lineage.size() > 0) {

            // for each parent (oldest to youngest)
            for (unsigned int i = 0; i < lineage.size(); ++i) {
                auto&[pEnt, pTform] = lineage[i];

                // check if we don't already have it
                if (_entitiesToScenes.find(pEnt) == _entitiesToScenes.end()) {
                    if (i == 0) { // if this is a root level scene
                        _entitiesToScenes[pEnt] = _renderer->getRootScene()->createScene();
                    }
                    else { // if this parent has another parent (not root)
                        _entitiesToScenes[pEnt] = _entitiesToScenes[std::get<0>(lineage[i - 1])]->createScene();
                    }
                }

                _entitiesToScenes[pEnt]->setLocalTransform(pTform);
            }

            scene = _entitiesToScenes[std::get<0>(lineage.back())]->createScene();
        }
        else {
            scene = _renderer->getRootScene()->createScene();
        }

        // add entity to mapping
        scene->setLocalTransform(transform);
        _entitiesToScenes[entity] = scene;
    }


}
