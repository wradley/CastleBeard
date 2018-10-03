#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "Model.h"
#include "Resources.h"


gp::Model::Model() :
	_color (nullptr), _delColor (false),
	_normal(nullptr), _delNormal(false),
	_mra   (nullptr), _delMRA   (false),
	_mesh  (nullptr), _delMesh  (false),
	_shader(nullptr), _delShader(false)
{}


gp::Model::~Model()
{
    if (_delColor  && _color ) Resources::FreeTexture(_color);
    if (_delNormal && _normal) Resources::FreeTexture(_normal);
    if (_delMRA    && _mra   ) Resources::FreeTexture(_mra);
    if (_delMesh   && _mesh  ) Resources::FreeMesh(_mesh);
	if (_delShader && _shader) Resources::FreeShader(_shader);
}


void gp::Model::loadFile(const std::string &s)
{
	// load json file as string
	std::ifstream ifs(s);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	// convert json text into json object
	auto json = nlohmann::json::parse(content);
	json.parse(content);

	if (!json.is_null()) {

		std::string meshFile    = !json["mesh"].is_null()    ? json["mesh"]    : "";
		std::string colorFile   = !json["color"].is_null()   ? json["color"]   : "";
		std::string normalFile  = !json["normal"].is_null()  ? json["normal"]  : "";
		std::string mraFile     = !json["mra"].is_null()     ? json["mra"]     : "";
		std::string vshaderFile = !json["vshader"].is_null() ? json["vshader"] : "";
		std::string gshaderFile = !json["gshader"].is_null() ? json["gshader"] : "";
		std::string fshaderFile = !json["fshader"].is_null() ? json["fshader"] : "";

		if (meshFile.size()) {
			_delMesh = true;
			_mesh = Resources::LoadMesh(meshFile);
		}

		if (colorFile.size()) {
			_delColor = true;
			_color = Resources::LoadTexture(colorFile);
		}

		if (normalFile.size()) {
			_delNormal = true;
			_normal = Resources::LoadTexture(normalFile);
		}

		if (mraFile.size()) {
			_delMRA = true;
			_mra = Resources::LoadTexture(mraFile);
		}

		if (vshaderFile.size() && fshaderFile.size()) {
			_delShader = true;
			_shader = Resources::LoadShader(vshaderFile, fshaderFile, gshaderFile);
		}
	}

	else
		std::cout << "Could not read json file: " << s << std::endl;
}


void gp::Model::setMesh(Mesh *mesh)
{
    _mesh = mesh;
    _delMesh = false;
}


void gp::Model::setColor(Texture *color)
{
    _color = color;
    _delColor = false;
}


void gp::Model::setNormal(Texture *normal)
{
    _normal = normal;
    _delNormal = false;
}


void gp::Model::setMRA(Texture *mra)
{
    _mra = mra;
    _delMRA = false;
}


void gp::Model::setShader(Shader * s)
{
	_shader = s;
	_delShader = false;
}


void gp::Model::draw(const glm::mat4 &proj, const glm::mat4 &view)
{
	_shader->bind();
	_shader->setMat4("uProj", proj);
	_shader->setMat4("uView", view);
	_shader->setVec3("uCamPos", glm::vec3(view[3]));
	_shader->setTexture("uColor", 0, *_color);
	_shader->setTexture("uNormal", 1, *_normal);
	_shader->setTexture("uMRA", 2, *_mra);
	_shader->setMat4("uModel", _transform.genMatrix());
    _mesh->draw();
}


void gp::InstancedModel::loadFile(const std::string & fp)
{
	// load json file as string
	std::ifstream ifs(fp);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	// convert json text into json object
	auto json = nlohmann::json::parse(content);
	json.parse(content);

	if (!json.is_null()) {

		std::string meshFile    = !json["mesh"].is_null()    ? json["mesh"]    : "";
		std::string colorFile   = !json["color"].is_null()   ? json["color"]   : "";
		std::string normalFile  = !json["normal"].is_null()  ? json["normal"]  : "";
		std::string mraFile     = !json["mra"].is_null()     ? json["mra"]     : "";
		std::string vshaderFile = !json["vshader"].is_null() ? json["vshader"] : "";
		std::string gshaderFile = !json["gshader"].is_null() ? json["gshader"] : "";
		std::string fshaderFile = !json["fshader"].is_null() ? json["fshader"] : "";

		if (meshFile.size()) {
			_delMesh = true;
			_mesh = Resources::LoadInstancedMesh(meshFile);
		}

		if (colorFile.size()) {
			_delColor = true;
			_color = Resources::LoadTexture(colorFile);
		}

		if (normalFile.size()) {
			_delNormal = true;
			_normal = Resources::LoadTexture(normalFile);
		}

		if (mraFile.size()) {
			_delMRA = true;
			_mra = Resources::LoadTexture(mraFile);
		}

		if (vshaderFile.size() && fshaderFile.size()) {
			_delShader = true;
			_shader = Resources::LoadShader(vshaderFile, fshaderFile, gshaderFile);
		}
	}

	else
		std::cout << "Could not read json file: " << fp << std::endl;
}


void gp::InstancedModel::setMesh(Mesh * mesh)
{
	if (mesh->isInstanced()) {
		_mesh = mesh;
	}

	std::cout << "Mesh given to InstancedModel was not InstancedMesh" << std::endl;
}


void gp::InstancedModel::setInstanceMatrices(const std::vector<glm::mat4>& mats)
{
	((InstancedMesh*)_mesh)->setMatrices(mats);
}
