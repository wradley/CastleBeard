#include "Resources.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cmath>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct MshRef
{
    gp::Mesh *mesh;
    unsigned int refs;
};

struct TxtRef
{
    gp::Texture *texture;
    unsigned int refs;
};

struct ShdrRef
{
	gp::Shader *shader;
	unsigned int refs;
};

static std::map<std::string, MshRef> _meshes;
static std::map<gp::Mesh*, std::string> _meshFiles;

static std::map<std::string, TxtRef> _textures;
static std::map<gp::Texture*, std::string> _textureFiles;

static std::map<std::string, ShdrRef> _shaders;
static std::map<gp::Shader*, std::string> _shaderFiles;

static unsigned int _capFBO, _capRBO;
static gp::Mesh *_capCube, *_capQuad;
static glm::mat4 _capProj;
static glm::mat4 _capViews[6];

static gp::Mesh* GenQuad()
{
	std::vector<gp::Vertex> verts;
	std::vector<unsigned int> indices;

	// 2--3
	// | /|
	// |/ |
	// 0--1

	gp::Vertex v0; v0.position = glm::vec3(-1.0f, -1.0f, 0.0f); v0.uv = glm::vec2(0.0f, 0.0f);
	gp::Vertex v1; v1.position = glm::vec3( 1.0f, -1.0f, 0.0f); v1.uv = glm::vec2(1.0f, 0.0f);
	gp::Vertex v2; v2.position = glm::vec3(-1.0f,  1.0f, 0.0f); v2.uv = glm::vec2(0.0f, 1.0f);
	gp::Vertex v3; v3.position = glm::vec3( 1.0f,  1.0f, 0.0f); v3.uv = glm::vec2(1.0f, 1.0f);
	verts.push_back(v0); verts.push_back(v1); verts.push_back(v2); verts.push_back(v3);

	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	indices.push_back(3); indices.push_back(1); indices.push_back(0);

	return new gp::Mesh(verts, indices);
}

static void InitCapture()
{
	// only init once
	static bool called = false;
	if  (called) return;
	else called = true;

	//   6--------7
	//  /|       /|
	// 4-+------5 |
	// | |      | |
	// | 2------+-3
	// |/       |/
	// 0--------1

	const float sz = 7.5f;
	std::vector<gp::Position> positions;
	std::vector<unsigned int> indices;

	positions.push_back(glm::vec3(-sz, -sz, sz)); // 0
	positions.push_back(glm::vec3(sz, -sz, sz)); // 1
	positions.push_back(glm::vec3(-sz, -sz, -sz)); // 2
	positions.push_back(glm::vec3(sz, -sz, -sz)); // 3
	positions.push_back(glm::vec3(-sz, sz, sz)); // 4
	positions.push_back(glm::vec3(sz, sz, sz)); // 5
	positions.push_back(glm::vec3(-sz, sz, -sz)); // 6
	positions.push_back(glm::vec3(sz, sz, -sz)); // 7

	indices.push_back(0); indices.push_back(4); indices.push_back(5); // front
	indices.push_back(5); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(5); indices.push_back(7); // right
	indices.push_back(7); indices.push_back(3); indices.push_back(1);
	indices.push_back(3); indices.push_back(7); indices.push_back(6); // back
	indices.push_back(6); indices.push_back(2); indices.push_back(3);
	indices.push_back(2); indices.push_back(6); indices.push_back(4); // left
	indices.push_back(4); indices.push_back(0); indices.push_back(2);
	indices.push_back(4); indices.push_back(6); indices.push_back(7); // top
	indices.push_back(7); indices.push_back(5); indices.push_back(4);
	indices.push_back(0); indices.push_back(1); indices.push_back(3); // bottom
	indices.push_back(3); indices.push_back(2); indices.push_back(0);

	_capCube = new gp::Mesh(positions, indices);
	_capQuad = GenQuad();

	glGenFramebuffers(1, &_capFBO);
	glGenRenderbuffers(1, &_capRBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _capRBO);

	_capProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	_capViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_capViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_capViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f));
	_capViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f));
	_capViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	_capViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f));
	
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

// default cube
//   6---------7
//  /|        /|
// 4-+-------5 |
// | |       | |
// | 2-------+-3
// |/        |/
// 0---------1

static gp::Mesh* MakeCube()
{
    std::vector<gp::Vertex> vertices;
    std::vector<unsigned int> indices;
    gp::Vertex v0; gp::Vertex v1; gp::Vertex v2; gp::Vertex v3;
    gp::Vertex v4; gp::Vertex v5; gp::Vertex v6; gp::Vertex v7;
    
    const float S = 10.0f;
    
    v0.position = glm::vec3(-S, -S,  S); vertices.push_back(v0);
    v1.position = glm::vec3( S, -S,  S); vertices.push_back(v1);
    v2.position = glm::vec3(-S, -S, -S); vertices.push_back(v2);
    v3.position = glm::vec3( S, -S, -S); vertices.push_back(v3);
    v4.position = glm::vec3(-S,  S,  S); vertices.push_back(v4);
    v5.position = glm::vec3( S,  S,  S); vertices.push_back(v5);
    v6.position = glm::vec3(-S,  S, -S); vertices.push_back(v6);
    v7.position = glm::vec3( S,  S, -S); vertices.push_back(v7);
    
    indices.push_back(0); indices.push_back(4); indices.push_back(5); // front
    indices.push_back(5); indices.push_back(1); indices.push_back(0);
    indices.push_back(1); indices.push_back(5); indices.push_back(7); // right
    indices.push_back(7); indices.push_back(3); indices.push_back(1);
    indices.push_back(3); indices.push_back(7); indices.push_back(6); // back
    indices.push_back(6); indices.push_back(2); indices.push_back(3);
    indices.push_back(2); indices.push_back(6); indices.push_back(4); // left
    indices.push_back(4); indices.push_back(0); indices.push_back(2);
    indices.push_back(4); indices.push_back(6); indices.push_back(7); // top
    indices.push_back(7); indices.push_back(5); indices.push_back(4);
    indices.push_back(2); indices.push_back(0); indices.push_back(1); // bottom
    indices.push_back(1); indices.push_back(3); indices.push_back(2);
    
    return new gp::Mesh(vertices, indices);
}


static gp::InstancedMesh* MakeInstancedCube()
{
	std::vector<gp::Vertex> vertices;
	std::vector<unsigned int> indices;
	gp::Vertex v0; gp::Vertex v1; gp::Vertex v2; gp::Vertex v3;
	gp::Vertex v4; gp::Vertex v5; gp::Vertex v6; gp::Vertex v7;

	const float S = 10.0f;

	v0.position = glm::vec3(-S, -S, S); vertices.push_back(v0);
	v1.position = glm::vec3(S, -S, S); vertices.push_back(v1);
	v2.position = glm::vec3(-S, -S, -S); vertices.push_back(v2);
	v3.position = glm::vec3(S, -S, -S); vertices.push_back(v3);
	v4.position = glm::vec3(-S, S, S); vertices.push_back(v4);
	v5.position = glm::vec3(S, S, S); vertices.push_back(v5);
	v6.position = glm::vec3(-S, S, -S); vertices.push_back(v6);
	v7.position = glm::vec3(S, S, -S); vertices.push_back(v7);

	indices.push_back(0); indices.push_back(4); indices.push_back(5); // front
	indices.push_back(5); indices.push_back(1); indices.push_back(0);
	indices.push_back(1); indices.push_back(5); indices.push_back(7); // right
	indices.push_back(7); indices.push_back(3); indices.push_back(1);
	indices.push_back(3); indices.push_back(7); indices.push_back(6); // back
	indices.push_back(6); indices.push_back(2); indices.push_back(3);
	indices.push_back(2); indices.push_back(6); indices.push_back(4); // left
	indices.push_back(4); indices.push_back(0); indices.push_back(2);
	indices.push_back(4); indices.push_back(6); indices.push_back(7); // top
	indices.push_back(7); indices.push_back(5); indices.push_back(4);
	indices.push_back(2); indices.push_back(0); indices.push_back(1); // bottom
	indices.push_back(1); indices.push_back(3); indices.push_back(2);

	return new gp::InstancedMesh(vertices, indices);
}


void processMesh(aiMesh *mesh,
                 const aiScene *scene,
                 std::vector<gp::Vertex> &vertices,
                 std::vector<unsigned int> &indices
){
    // vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        gp::Vertex vertex;
        
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

		vertex.tangent.x = mesh->mTangents[i].x;
		vertex.tangent.y = mesh->mTangents[i].y;
		vertex.tangent.z = mesh->mTangents[i].z;
        
        if(mesh->mTextureCoords[0])
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.uv.x = 0.0f;
            vertex.uv.y = 0.0f;
        }
        
        vertices.push_back(vertex);
    }
    
    // indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
}


void processNode(aiNode *node,
                 const aiScene *scene,
                 std::vector<gp::Vertex> &vertices,
                 std::vector<unsigned int> &indices
                 ){
    // process all the node’s meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, vertices, indices);
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, vertices, indices);
}


gp::Mesh* gp::Resources::LoadMesh(const std::string &fp)
{
    if (_meshes.find(fp) == _meshes.end()) {
        _meshes[fp].refs = 0;
        Mesh *loadedmesh;
        
        // load vertes and indices
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(fp, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        if (!scene ||
            scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode)
        {
            std::cout << "Assimp: could not import mesh: " << importer.GetErrorString() << std::endl;
            loadedmesh = MakeCube();
        } else {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            processNode(scene->mRootNode, scene, vertices, indices);
            loadedmesh = new Mesh(vertices, indices);
        }
        
        _meshes[fp].mesh = loadedmesh;
        _meshFiles[_meshes[fp].mesh] = fp;
    }
    
    _meshes[fp].refs += 1;
    return _meshes[fp].mesh;
}


gp::InstancedMesh * gp::Resources::LoadInstancedMesh(const std::string & fp)
{
	InstancedMesh *loadedmesh;

	// load vertes and indices
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fp, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		std::cout << "Assimp: could not import mesh: " << importer.GetErrorString() << std::endl;
		loadedmesh = MakeInstancedCube();
	}
	else {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		processNode(scene->mRootNode, scene, vertices, indices);
		loadedmesh = new InstancedMesh(vertices, indices);
	}

	return loadedmesh;
}


gp::Texture* gp::Resources::LoadTexture(const std::string &fp)
{
    // create new texture if it's not already loaded
    if (_textures.find(fp) == _textures.end()) {
        _textures[fp].refs = 0;
        
        int x, y, n;
        unsigned char *data = stbi_load(fp.c_str(), &x, &y, &n, 0);
		if (!data) { // load color pink
			_textures[fp].texture = new Texture(1.0f, 0.0f, 1.0f);
			std::cout << "Could not load texture: " << fp << std::endl;
		}
        else {
            _textures[fp].texture = new Texture(data, x, y, n);
            stbi_image_free(data);
        }
        
        _textureFiles[_textures[fp].texture] = fp;
    }
    
    _textures[fp].refs += 1;
    return _textures[fp].texture;
}


gp::Texture * gp::Resources::LoadHDRTexture(const std::string & fp)
{
	// create new texture if it's not already loaded
	if (_textures.find(fp) == _textures.end()) {
		_textures[fp].refs = 0;

		int x, y, n;
		float *data = stbi_loadf(fp.c_str(), &x, &y, &n, 0);
		if (!data) { // load color pink
			_textures[fp].texture = new Texture(1.0f, 0.0f, 1.0f);
			std::cout << "Could not load hdr texture: " << fp << std::endl;
		}
		else {
			_textures[fp].texture = new Texture(data, x, y, n);
			stbi_image_free(data);
		}

		_textureFiles[_textures[fp].texture] = fp;
	}

	_textures[fp].refs += 1;
	return _textures[fp].texture;
}


gp::Texture * gp::Resources::CreateBrdfLut(unsigned int res)
{
	std::string fp("CreateBrdfLut-");
	fp.append(std::to_string(res));

	if (_textures.find(fp) == _textures.end()) {
		static Shader *brdfShader = LoadShader("Assets/Shaders/brdf_lut_vs.glsl", "Assets/Shaders/brdf_lut_fs.glsl");
		InitCapture();

		unsigned int id;
		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, res, res, 0, GL_RG, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, _capFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, _capRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

		glViewport(0, 0, res, res);
		brdfShader->bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_capQuad->draw();

		_textures[fp].texture = new gp::Texture(id);
		_textures[fp].refs = 0;
		_textureFiles[_textures[fp].texture] = fp;
	}
	
	_textures[fp].refs += 1;
	return _textures[fp].texture;
}


gp::Shader* gp::Resources::LoadShader(const std::string &vShader, const std::string &fShader, const std::string &gShader)
{
	std::string fp(vShader);
	fp.append(gShader);
	fp.append(fShader);

	if (_shaders.find(fp) == _shaders.end()) {
		bool hasErrors = false;
		std::string vError, fError, gError, linkError;

		std::ifstream vIn(vShader);
		std::string vCode((std::istreambuf_iterator<char>(vIn)), (std::istreambuf_iterator<char>()));
		if (vCode.size() == 0) { std::cout << "[" << vShader << "] Vertex shader file does not exist" << std::endl; hasErrors = true; }

		std::ifstream fIn(fShader);
		std::string fCode((std::istreambuf_iterator<char>(fIn)), (std::istreambuf_iterator<char>()));
		if (vCode.size() == 0) { std::cout << "[" << fShader << "] Fragment shader file does not exist" << std::endl; hasErrors = true; }

		std::string gCode = "";
		if (gShader.size() > 0) {
			std::ifstream gIn(gShader);
			std::string code((std::istreambuf_iterator<char>(gIn)), (std::istreambuf_iterator<char>()));
			if (vCode.size() == 0) { std::cout << "[" << gShader << "] Geometry shader file does not exist" << std::endl; hasErrors = true; }
			gCode = code;
		}

		
		Shader *s = new Shader(vCode, vError, fCode, fError, gCode, gError, linkError);

		// check for errors
		if (vError.size() > 0) { std::cout << "[" << vShader << "] " << vError << std::endl;    hasErrors = true; }
		if (fError.size() > 0) { std::cout << "[" << fShader << "] " << fError << std::endl;    hasErrors = true; }
		if (gError.size() > 0) { std::cout << "[" << gShader << "] " << gError << std::endl;    hasErrors = true; }
		if (linkError.size() > 0) { std::cout << "[Linking]" << linkError << std::endl; hasErrors = true; }

		if (hasErrors) {
			delete s;
			s = new Shader(0);
		}

		_shaders[fp].refs = 0;
		_shaders[fp].shader = s;
		_shaderFiles[_shaders[fp].shader] = fp;
	}

	_shaders[fp].refs += 1;
	return _shaders[fp].shader;
}


gp::CubeMap * gp::Resources::CreateCubeMapFromHDRerMap(const std::string & fp, unsigned int res)
{
	static Shader *toCubeShader = LoadShader("Assets/Shaders/to_cubemap_vs.glsl", "Assets/Shaders/to_cubemap_fs.glsl");
	InitCapture();

	// check if image exists
	Texture *ermap = LoadHDRTexture(fp);
	if (!ermap) return nullptr;

	// bind frame buffer and depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _capFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _capRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);

	// alloc cube map
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, res, res, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	toCubeShader->bind();
	toCubeShader->setTexture("uERMap", 0, *ermap);
	toCubeShader->setMat4("uProj", _capProj);
	glViewport(0, 0, res, res);
	for (unsigned int i = 0; i < 6; ++i) {
		toCubeShader->setMat4("uView", _capViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_capCube->draw();
	}

	FreeTexture(ermap);
	return new CubeMap(cubemap);
}


gp::CubeMap * gp::Resources::CreateIrradianceMap(CubeMap * c, unsigned int res)
{
	static Shader *irrShader = LoadShader("Assets/Shaders/to_cubemap_vs.glsl", "Assets/Shaders/to_irradiance_fs.glsl");
	InitCapture();

	// bind frame buffer and depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _capFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _capRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);

	// alloc cube map
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, res, res, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	irrShader->bind();
	irrShader->setCubeMap("uEnvMap", 0, *c);
	irrShader->setMat4("uProj", _capProj);
	glViewport(0, 0, res, res);
	for (unsigned int i = 0; i < 6; ++i) {
		irrShader->setMat4("uView", _capViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_capCube->draw();
	}

	return new CubeMap(cubemap);
}


gp::CubeMap* gp::Resources::CreatePreFilteredMap(CubeMap * c, unsigned int res)
{
	static Shader *prefilterShader = LoadShader("Assets/Shaders/to_cubemap_vs.glsl", "Assets/Shaders/to_prefilter_fs.glsl");
	InitCapture();

	// bind frame buffer and depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _capFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, _capRBO);

	// alloc cube map
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, res, res, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	prefilterShader->bind();
	prefilterShader->setCubeMap("uEnvMap", 0, *c);
	prefilterShader->setMat4("uProj", _capProj);

	const unsigned int NUM_MIP_MAPS = 5;
	for (unsigned int mip = 0; mip < NUM_MIP_MAPS; ++mip) {

		unsigned int mipSize = res * std::pow(0.5, mip);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);
		glViewport(0, 0, mipSize, mipSize);

		float roughness = (float)mip / (float)(NUM_MIP_MAPS - 1);
		prefilterShader->setFloat("uRoughness", roughness);
		for (unsigned int i = 0; i < 6; ++i) {
			prefilterShader->setMat4("uView", _capViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			_capCube->draw();
		}
	}

	return new CubeMap(cubemap);
}


void gp::Resources::FreeMesh(Mesh *m)
{
    if (_meshFiles.find(m) != _meshFiles.end()) {
        std::string fp = _meshFiles[m];
        _meshes[fp].refs -= 1;
        
        if (_meshes[fp].refs == 0) {
            _meshFiles.erase(m);
            delete m;
            _meshes.erase(fp);
        }
    }
}


void gp::Resources::FreeInstancedMesh(InstancedMesh * m)
{
	delete m;
}


void gp::Resources::FreeTexture(Texture *t)
{
    if (_textureFiles.find(t) != _textureFiles.end()) {
        std::string fp = _textureFiles[t];
        _textures[fp].refs -= 1;
        
        if (_textures[fp].refs == 0) {
            _textureFiles.erase(t);
            delete t;
            _textures.erase(fp);
        }
    }
}


void gp::Resources::FreeShader(Shader *s)
{
	if (_shaderFiles.find(s) != _shaderFiles.end()) {
		std::string fp = _shaderFiles[s];
		_shaders[fp].refs -= 1;

		if (_shaders[fp].refs == 0) {
			_shaderFiles.erase(s);
			delete s;
			_textures.erase(fp);
		}
	}
}


void gp::Resources::FreeCubeMap(CubeMap * c)
{
	delete c;
}


