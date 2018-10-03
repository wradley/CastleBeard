#pragma once
#include <string>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "CubeMap.h"

namespace gp
{    
    class Resources
    {
	public:
        static Mesh* LoadMesh(const std::string &fp);
		static InstancedMesh* LoadInstancedMesh(const std::string &fp);
		static Texture* LoadTexture(const std::string &fp);
		static Texture* LoadHDRTexture(const std::string &fp);
		static Texture* CreateBrdfLut(unsigned int res = 512);
		static Shader* LoadShader(
			const std::string &vShader, 
			const std::string &fShader, 
			const std::string &gShader = "");
		static CubeMap* CreateCubeMapFromHDRerMap(const std::string &fp, unsigned int resolution = 512);
		static CubeMap* CreateIrradianceMap(CubeMap *c, unsigned int res = 32);
		static CubeMap* CreatePreFilteredMap(CubeMap *c, unsigned int res = 128);
        
		static void FreeMesh(Mesh *m);
		static void FreeInstancedMesh(InstancedMesh *m);
		static void FreeTexture(Texture *t);
		static void FreeShader(Shader *s);
		static void FreeCubeMap(CubeMap *c);
	};
}
