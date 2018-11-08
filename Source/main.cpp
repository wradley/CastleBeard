#include "../Include/Math/Vector.h"
#include "../Include/Core/EventManager.h"
#include "../Include/Core/Events/ComponentEvents.h"
#include "../Include/Core/Events/InputEvents.h"
#include "../Include/Core/Events/EntityEvents.h"
#include "../Include/Graphics/System.h"
#include "../Include/PlayerController/System.h"
#include "../Include/Physics/System.h"
#include "../Include/Window/WindowManager.h"
#include <iostream>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main(int argc, char **argv)
{
    Core::EventManager eventManager;
    Core::EventQueue eventQueue;

    // create systems and managers
    Window::Manager windowManager;
    Graphics::System graphicsSystem;
    PlayerController::System playerControllerSystem;
    //Physics::System physicsSystem;

    // init systems
    windowManager.init(eventManager, 800, 600);
    graphicsSystem.init(eventManager);
    playerControllerSystem.init(eventManager);
    //physicsSystem.init(eventManager);

    // listen for engine shutdown
    eventManager.listenFor(Core::EventType::eShutdown, &eventQueue);
    eventManager.listenFor(Core::EventType::eControllerInput, &eventQueue);

    // send initial events
    Math::Transform initialPlayerTransform;
    Math::Transform initialCubeTransform;
    initialPlayerTransform.position.z = 10.0f;
    unsigned int playerEntity    = 1;
    unsigned int cubeEntity      = 2;
    unsigned int cameraComponent = 1;
    unsigned int pcComponent     = 2;

    {   // PLAYER
        auto ce = new Core::CreateEntityEvent;
        ce->entity = playerEntity;
        ce->parent = 0;
        ce->transform = initialPlayerTransform;

        // camera
        auto ac = new Core::AddCameraComponentEvent;
        ac->entity = playerEntity;
        ac->entityTform = initialPlayerTransform;
        ac->component = cameraComponent;
        ac->nearPlane = 0.1f;
        ac->farPlane = 1000.0f;
        ac->aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        ac->fieldOfView = 60.0f;

        // player controller
        auto pc = new Core::AddPlayerControllerComponentEvent;
        pc->entity = playerEntity;
        pc->component = pcComponent;
        pc->cameraComponent = cameraComponent;
        pc->entityTform = initialPlayerTransform;

        ce->components.push_back(std::shared_ptr<const Core::AddCameraComponentEvent>(ac));
        ce->components.push_back(std::shared_ptr<const Core::AddPlayerControllerComponentEvent>(pc));
        eventManager.send(std::shared_ptr<const Core::CreateEntityEvent>(ce), &eventQueue);
    }

    {   // CUBE
        auto ce = new Core::CreateEntityEvent;
        ce->entity = cubeEntity;
        ce->parent = 0;
        ce->transform = initialCubeTransform;

        auto am = new Core::AddModelComponentEvent;
        am->entity = cubeEntity;
        am->entityTform = initialCubeTransform;
        am->filepath = "Models/Cube.mdl";
        ce->components.push_back(std::shared_ptr<const Core::AddModelComponentEvent>(am));

        eventManager.send(std::shared_ptr<const Core::CreateEntityEvent>(ce), &eventQueue);
    }

    bool running = true;
    while(running)
    {
        // check events
        unsigned int size = eventQueue.size();
        for (unsigned int i = 0; i < size; ++i) {
            auto e = eventQueue.popFront();
            if (e->getType() == Core::EventType::eControllerInput) {
                Core::ControllerInputEvent *input = (Core::ControllerInputEvent*)e.get();
                if (input->pause) running = false;
            }
            if (e->getType() == Core::EventType::eShutdown)
                running = false;
        }

        // update systems
        windowManager.update(eventManager);
        playerControllerSystem.update(eventManager);
        //physicsSystem.update(eventManager);
        graphicsSystem.update(eventManager);
        windowManager.swapBuffers();

    }

    graphicsSystem.stop(eventManager);
    //physicsSystem.update(eventManager);
    playerControllerSystem.stop(eventManager);
    windowManager.stop(eventManager);

    return 0;
}
