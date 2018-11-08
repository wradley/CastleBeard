#pragma once
#include <vector>
#include "RendererNode.h"
#include "../Math/Matrix.h"

namespace Graphics
{
    class Model;
    class Camera;
    class Renderer;

    class Scene : public RendererNode
    {
    public:

        Scene(Renderer *renderer, Scene *parent);
        ~Scene();

        void destroy() override;

        Scene *createScene();
        Model *createModel();
        Camera *createCamera();

    protected:

        friend class Camera;
        friend class Model;

        void forgetChildCamera(Camera *c);
        void forgetChildModel(Model *c);
        void forgetChildScene(Scene *c);

    private:

        friend class Renderer;
        void draw(const Math::Mat4 &proj, const Math::Mat4 &view);
        
    private:

        std::vector<Scene*> _scenes;
        std::vector<Model*> _models;
        std::vector<Camera*> _cameras;

    };
}


