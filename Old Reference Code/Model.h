#pragma once
#include <string>
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Object.h"

namespace gp
{
    class Model : public Object
    {
    public:
        
        Model();
        ~Model();
        
        virtual void loadFile(const std::string &fp);
        virtual void setMesh(Mesh *mesh);
        void setColor(Texture *color);
        void setNormal(Texture *normal);
        void setMRA(Texture *mra);
		void setShader(Shader *s);
        
        void draw(const glm::mat4 &proj, const glm::mat4 &view);
        
    protected:
        
        bool _delColor, _delNormal, _delMRA, _delMesh, _delShader;
        Texture *_color, *_normal, *_mra;
        Mesh *_mesh;
		Shader *_shader;
                
    private:
        
        Model(const Model &m) = delete;
        Model& operator= (const Model &m) = delete;
        
    };


	class InstancedModel : public Model
	{
	public:

		void loadFile(const std::string &fp) override;
		void setMesh(Mesh *mesh) override;
		void setInstanceMatrices(const std::vector<glm::mat4> &mats);

	};
}
