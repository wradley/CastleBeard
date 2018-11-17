#pragma once
#include <map>
#include "../System.h"
#include "Renderer.h"

namespace Engine
{
    class GraphicsSystem : public Engine::System
    {
    public:

        GraphicsSystem();
        ~GraphicsSystem();

        void init(EventManager & em) override;
        void update(EventManager & em) override;
        void stop(EventManager & em) override;

    private:

        void handleEvents();
        void onEvent(std::shared_ptr<const Event> e);
        void onCreateEntity(const CreateEntityEvent *e);
        void onUnloadEntity(const UnloadEntityEvent *e);
        void onAddModel(const AddModelComponentEvent *e);
        void onAddCamera(const AddCameraComponentEvent *e);
        void onModCamera(const ModCameraComponentEvent *e);
        void onTransformEntity(const TransformEntityEvent *e);
        void onTransformEntities(const TransformEntitiesEvent *e);
        void onResizeWindow(const ResizeWindowEvent *e);

        void createSceneToEntity(
            unsigned int entity, const Math::Transform &transform,
            const std::vector<std::tuple<unsigned int, Math::Transform>> &lineage
        );

    private:

        EventQueue _eventQueue;
        Renderer::Renderer *_renderer;
        
        std::map<unsigned int, Renderer::Scene*> _entitiesToScenes;
        std::map<unsigned int, Renderer::Model*> _componentsToModels;
        std::map<unsigned int, Renderer::Camera*> _componentsToCameras;

    };
}