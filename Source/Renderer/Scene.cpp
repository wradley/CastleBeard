#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "Debug.h"


namespace Renderer
{
    Scene::Scene(Renderer *renderer, Scene *parent) :
        RendererNode(renderer, (RendererNode*)parent)
    {}


    Scene::~Scene()
    {
    }


    void Scene::destroy()
    {
        // delete children
        for (auto scene : _scenes) scene->destroy();
        for (auto model : _models) model->destroy();
        for (auto camera : _cameras) camera->destroy();

        // tell parent
        if (_parent) ((Scene*)_parent)->forgetChildScene(this);
    }


    Scene * Scene::createScene()
    {
        Scene *s = new Scene(_renderer, this);
        s->assignID((unsigned int)_scenes.size());
        _scenes.push_back(s);
        return s;
    }


    Model * Scene::createModel()
    {
        Model *m = new Model(_renderer, this);
        m->assignID((unsigned int)_models.size());
        _models.push_back(m);
        return m;
    }


    Camera * Scene::createCamera()
    {
        Camera *c = new Camera(_renderer, this);
        c->assignID((unsigned int)_cameras.size());
        _cameras.push_back(c);
        return c;
    }


    void Scene::forgetChildCamera(Camera * c)
    {
        // make sure it exists
        if (!c) {
            DEBUG_LOG("Attempted to delete null camera");
            return;
        }

        unsigned int ID = c->getID();

        // make sure it is a child of ours
        if (ID >= _cameras.size() && c != _cameras[ID]) {
            DEBUG_LOG("Attempted to delete non-child camera");
            return;
        }

        // if not last element copy over last element
        if (ID != (_cameras.size() - 1)) {
            _cameras[ID] = _cameras.back();
            _cameras[ID]->assignID(ID);
        }

        _cameras.pop_back();
    }


    void Scene::forgetChildModel(Model * m)
    {
        // make sure it exists
        if (!m) {
            DEBUG_LOG("Attempted to delete null model");
            return;
        }

        unsigned int ID = m->getID();

        // make sure it is a child of ours
        if (ID >= _models.size() && m != _models[ID]) {
            DEBUG_LOG("Attempted to delete non-child model");
            return;
        }

        // if not last element copy over last element
        if (ID != (_models.size() - 1)) {
            _models[ID] = _models.back();
            _models[ID]->assignID(ID);
        }

        _models.pop_back();
    }


    void Scene::forgetChildScene(Scene * s)
    {
        // make sure it exists
        if (!s) {
            DEBUG_LOG("Attempted to delete null scene");
            return;
        }

        unsigned int ID = s->getID();

        // make sure it is a child of ours
        if (ID >= _scenes.size() && s != _scenes[ID]) {
            DEBUG_LOG("Attempted to delete non-child scene");
            return;
        }

        // if not last element copy over last element
        if (ID != (_scenes.size() - 1)) {
            _scenes[ID] = _scenes.back();
            _scenes[ID]->assignID(ID);
        }

        _scenes.pop_back();
    }


    void Scene::draw(const Math::Mat4 &proj, const Math::Mat4 &view)
    {
        for (Model *model : _models) {
            model->draw(proj, view);
        }

        for (Scene *scene : _scenes) {
            scene->draw(proj, view);
        }
    }
}