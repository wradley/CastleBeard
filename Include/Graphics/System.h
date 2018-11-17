#pragma once
#include <map>
#include "../Core/System.h"
#include "Renderer.h"
#include "../Core/Events/EntityEvents.h"
#include "../Core/Events/ComponentEvents.h"
#include "../Core/Events/WindowEvents.h"

namespace Graphics
{
    class System : public Core::System
    {
    public:

        System();
        ~System();

        void init(Core::EventManager & em) override;
        void update(Core::EventManager & em) override;
        void stop(Core::EventManager & em) override;

    private:

        void handleEvents();
        void onEvent(std::shared_ptr<const Core::Event> e);
        void onCreateEntity(const Core::CreateEntityEvent *e);
        void onUnloadEntity(const Core::UnloadEntityEvent *e);
        void onAddModel(const Core::AddModelComponentEvent *e);
        void onAddCamera(const Core::AddCameraComponentEvent *e);
        void onModCamera(const Core::ModCameraComponentEvent *e);
        void onTransformEntity(const Core::TransformEntityEvent *e);
        void onTransformEntities(const Core::TransformEntitiesEvent *e);
        void onResizeWindow(const Core::ResizeWindowEvent *e);

        void createSceneToEntity(
            unsigned int entity, const Math::Transform &transform,
            const std::vector<std::tuple<unsigned int, Math::Transform>> &lineage
        );

    private:

        Core::EventQueue _eventQueue;
        Renderer *_renderer;
        
        std::map<unsigned int, Scene*> _entitiesToScenes;
        std::map<unsigned int, Model*> _componentsToModels;
        std::map<unsigned int, Camera*> _componentsToCameras;

    };
}