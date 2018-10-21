#include "../Include/Math/Vector.h"
#include "../Include/Core/EventManager.h"
#include "../Include/Core/Events/ComponentEvents.h"
#include "../Include/Core/Events/InputEvents.h"
#include "../Include/Core/Events/EntityEvents.h"
#include "../Include/Graphics/System.h"
#include "../Include/Window/WindowManager.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void CheckEvents(Core::EventQueue &q, bool &running)
{
    unsigned int size = q.size();
    for (unsigned int i = 0; i < size; ++i) {
        auto e = q.popFront();
        if (e->getType() == Core::EventType::eShutdown)
            running = false;
    }
}

int main(int argc, char **argv)
{
    Core::EventManager eventManager;
    Core::EventQueue eventQueue;

    // create systems and managers
    Window::Manager windowManager;
    Graphics::System graphicsSystem;

    // init systems
    windowManager.init(eventManager, 800, 600);
    graphicsSystem.init(eventManager);

    // listen for engine shutdown
    eventManager.listenFor(Core::EventType::eShutdown, &eventQueue);

    // send initial events

    {   // PLAYER/CAMERA
        Core::CreateEntityEvent *ce = new Core::CreateEntityEvent;
        ce->entity = 1;
        ce->parent = 0;
        ce->transform.position.z = 10.0f;
        eventManager.send(std::shared_ptr<const Core::CreateEntityEvent>(ce), &eventQueue);

        Core::AddCameraComponentEvent *ac = new Core::AddCameraComponentEvent;
        ac->entity = 1;
        ac->nearPlane = 0.1f;
        ac->farPlane = 1000.0f;
        ac->aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        ac->fieldOfView = 60.0f;
        eventManager.send(std::shared_ptr<const Core::AddCameraComponentEvent>(ac), &eventQueue);
    }

    {   // CUBE
        Core::CreateEntityEvent *ce = new Core::CreateEntityEvent;
        ce->entity = 2;
        ce->parent = 0;
        eventManager.send(std::shared_ptr<const Core::CreateEntityEvent>(ce), &eventQueue);

        Core::AddModelComponentEvent *am = new Core::AddModelComponentEvent;
        am->entity = 2;
        am->filepath = "Models/Cube.mdl";
        eventManager.send(std::shared_ptr<const Core::AddModelComponentEvent>(am), &eventQueue);
    }

    bool running = true;
    while(running)
    {
        CheckEvents(eventQueue, running);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update systems
        windowManager.update(eventManager);
        graphicsSystem.update(eventManager);

        windowManager.swapBuffers();

    }

    graphicsSystem.stop(eventManager);

    return 0;
}
