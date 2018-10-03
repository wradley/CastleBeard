#pragma once
#include <glad/glad.h>
#include <vector>
#include "glmIncludes.h"

namespace gp
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
		glm::vec3 tangent;
        glm::vec2 uv;
    };

	typedef glm::vec3 Position;
    
    typedef std::vector<Vertex> Vertices;
	typedef std::vector<Position> Positions;
    typedef std::vector<unsigned int> Indices;
    

    class Mesh
    {
    public:
        
        Mesh(const Vertices &v, const Indices &i);
		Mesh(const Positions &v, const Indices &i);
        Mesh(Mesh &&m);
        ~Mesh();
        
		inline virtual bool isInstanced() { return false; }
        virtual void draw();
        
    protected:
        
        unsigned int _vao, _vbo, _ebo, _numIndices;
        
    private:
        
        Mesh(const Mesh &m) = delete;
        Mesh& operator= (const Mesh &m) = delete;
        
    };


	class InstancedMesh : public Mesh
	{
	public:

		InstancedMesh(const Vertices &v, const Indices &i);
		InstancedMesh(const Positions &v, const Indices &i);

		inline bool isInstanced() override { return true; }
		void setMatrices(const std::vector<glm::mat4> &ms);
		void draw();

	private:


		unsigned int _numInstances;
		unsigned int _mBuffer;

	};
}
