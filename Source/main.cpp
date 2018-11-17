#include "Engine.h"
#include "CBMath.h"
#include "Window.h"
#include <iostream>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main(int argc, char **argv)
{
    Engine::EventManager eventManager;
    Engine::EventQueue eventQueue;

    // create systems and managers
    Window::Manager windowManager;
    Engine::GraphicsSystem graphicsSystem;
    Engine::PlayerControllerSystem playerControllerSystem;
    Engine::PhysicsSystem physicsSystem;

    // init systems
    windowManager.init(eventManager, 800, 600);
    graphicsSystem.init(eventManager);
    playerControllerSystem.init(eventManager);
    physicsSystem.init(eventManager);

    // listen for engine shutdown
    eventManager.listenFor(Engine::EventType::eShutdown, &eventQueue);
    eventManager.listenFor(Engine::EventType::eControllerInput, &eventQueue);

    // send initial events
    Math::Transform initialPlayerTransform;
    Math::Transform initialCubeTransform;
    initialPlayerTransform.position.z = 10.0f;
    unsigned int playerEntity    = 1;
    unsigned int cubeEntity      = 2;
    unsigned int cameraComponent = 1;
    unsigned int pcComponent     = 2;

    {   // PLAYER
        auto ce = new Engine::CreateEntityEvent;
        ce->entity = playerEntity;
        ce->parent = 0;
        ce->transform = initialPlayerTransform;

        // camera
        auto ac = new Engine::AddCameraComponentEvent;
        ac->entity = playerEntity;
        ac->entityTform = initialPlayerTransform;
        ac->component = cameraComponent;
        ac->nearPlane = 0.1f;
        ac->farPlane = 1000.0f;
        ac->aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        ac->fieldOfView = 60.0f;

        // player controller
        auto pc = new Engine::AddPlayerControllerComponentEvent;
        pc->entity = playerEntity;
        pc->component = pcComponent;
        pc->cameraComponent = cameraComponent;
        pc->entityTform = initialPlayerTransform;

        ce->components.push_back(std::shared_ptr<const Engine::AddCameraComponentEvent>(ac));
        ce->components.push_back(std::shared_ptr<const Engine::AddPlayerControllerComponentEvent>(pc));
        eventManager.send(std::shared_ptr<const Engine::CreateEntityEvent>(ce), &eventQueue);
    }

    {   // CUBE
        auto ce = new Engine::CreateEntityEvent;
        ce->entity = cubeEntity;
        ce->parent = 0;
        ce->transform = initialCubeTransform;

        auto am = new Engine::AddModelComponentEvent;
        am->entity = cubeEntity;
        am->entityTform = initialCubeTransform;
        am->filepath = "Models/Cube.mdl";
        ce->components.push_back(std::shared_ptr<const Engine::AddModelComponentEvent>(am));

        auto ar = new Engine::AddRigidbodyComponentEvent;
        ar->entity = cubeEntity;
        ar->invertedMass = 1;
        ar->transform = initialCubeTransform;
        ce->components.push_back(std::shared_ptr<const Engine::AddRigidbodyComponentEvent>(ar));

        eventManager.send(std::shared_ptr<const Engine::CreateEntityEvent>(ce), &eventQueue);
    }

    bool running = true;
    while(running)
    {
        // check events
        unsigned int size = eventQueue.size();
        for (unsigned int i = 0; i < size; ++i) {
            auto e = eventQueue.popFront();
            if (e->getType() == Engine::EventType::eControllerInput) {
                Engine::ControllerInputEvent *input = (Engine::ControllerInputEvent*)e.get();
                if (input->pause) running = false;
            }
            if (e->getType() == Engine::EventType::eShutdown)
                running = false;
        }

        // update systems
        windowManager.update(eventManager);
        playerControllerSystem.update(eventManager);
        physicsSystem.update(eventManager);
        graphicsSystem.update(eventManager);
        windowManager.swapBuffers();

    }

    graphicsSystem.stop(eventManager);
    physicsSystem.stop(eventManager);
    playerControllerSystem.stop(eventManager);
    windowManager.stop(eventManager);

    return 0;
}
