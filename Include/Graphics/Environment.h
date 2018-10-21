#pragma once
#include <vector>
#include <string>
#include <deque>
#include <map>
#include "Environment/EnvironmentNode.h"
#include "Environment/Model.h"
#include "Environment/Camera.h"
#include "../Math/Transform.h"


namespace Graphics
{
    class Environment
    {
        Camera *_mainCamera;
        
        std::vector<EnvironmentNode*> _nodes;
        std::deque<unsigned int> _recycledNodeIDs;
        std::vector<Model*> _models;
        std::deque<unsigned int> _recycledModelIDs;

        std::vector<Camera*> _cameras;
        std::deque<unsigned int> _recycledCameraIDs;

        // cached data
        std::map<std::string, std::shared_ptr<Shader>> _shaderCache;
        std::map<std::string, std::shared_ptr<Material>> _materialCache;
        std::map<std::string, std::shared_ptr<std::vector<Mesh*>>> _meshCache;

    public:

        Environment();
        ~Environment();

        void draw();

        // --------------------------------------------------- nodes
        unsigned int createNode();
        void addNodeChild(unsigned int node, unsigned int childNode);
        void addNodeModel(unsigned int node, unsigned int model);
        void addNodeCamera(unsigned int node, unsigned int camera);
        void setNodeTransform(unsigned int node, const Math::Transform &t);
        std::vector<unsigned int> removeNodeAndChildren(unsigned int node); // returns list of deleted children

        // --------------------------------------------------- models
        unsigned int createModel();
        void loadModel(unsigned int model, const std::string &filepath);
        void setModelTransform(unsigned int model, const Math::Transform &t);

        // --------------------------------------------------- cameras
        unsigned int createCamera();
        void setCameraAsMain(unsigned int camera);
        void setCameraTransform(unsigned int camera, const Math::Transform &t);
        void setCameraNearPlane(unsigned int camera, float d);
        void setCameraFarPlane(unsigned int camera, float d);
        void setCameraAspectRatio(unsigned int camera, float a);
        void setCameraFieldOfView(unsigned int camera, float fov);
       

    private:

        // recursively check children, children's children, ect... for relation
        bool doesNodeHaveChild(EnvironmentNode *parent, EnvironmentNode *child);

    private:

        Environment(const Environment &) = delete;
    };
}