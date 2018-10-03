#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Resources.h"
#include "Window.h"
#include "FrameBuffer.h"
#include "Model.h"
#include "CubeMap.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <cstdlib> 
#include <ctime>

const int WINDOWED_WIDTH = 1200;
const int WINDOWED_HEIGHT = 720;
const int FULLSCREEN_WIDTH = 1920;
const int FULLSCREEN_HEIGHT = 1080;

namespace gp
{
    class Light
    {
    public:
        
        void draw(gp::Shader *s) {}
        
    private:
        
        Mesh *_mesh;
    };
}

gp::GeometryBuffer *_geomBuffer;
gp::LightBuffer *_lightBuffer;

//gp::Shader *_geomShader;
gp::Shader *_lightShader;
gp::Shader *_ambientShader;
gp::Shader *_skyShader;

glm::mat4 _proj;
glm::mat4 _view;

std::vector<gp::Model*> _models;
std::vector<gp::Light*> _lights;
gp::Mesh *_quad;
gp::Mesh *_cube;
gp::CubeMap *_skybox;
gp::CubeMap *_irrbox;
gp::CubeMap *_prefilteredBox;
gp::Texture *_brdfLutTexture;

void DrawGeom();
void DrawLights();
void DrawSkyBox();
gp::Mesh* CreateQuad();
gp::Mesh* CreateCube();
void Tick();

glm::vec3 camPos;
float camPitch, camYaw;

int main()
{
    if(!gp::Window::Open("Game Engine", WINDOWED_WIDTH, WINDOWED_HEIGHT)) return -1;
    
	srand(time(0));

    _geomBuffer = new gp::GeometryBuffer(gp::Window::GetWidth(), gp::Window::GetHeight());
    _lightBuffer = new gp::LightBuffer(gp::Window::GetWidth(), gp::Window::GetHeight());
    
    //_geomShader = gp::Resources::LoadShader("Assets/Shaders/geom_vs.glsl", "Assets/Shaders/geom_fs.glsl");
    _lightShader = gp::Resources::LoadShader("Assets/Shaders/light_vs.glsl", "Assets/Shaders/light_fs.glsl");
    _ambientShader = gp::Resources::LoadShader("Assets/Shaders/ambient_vs.glsl", "Assets/Shaders/ambient_fs.glsl");
	_skyShader = gp::Resources::LoadShader("Assets/Shaders/skybox_vs.glsl", "Assets/Shaders/skybox_fs.glsl");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glDepthFunc(GL_LEQUAL);
    //gp::Window::VSyncOff();
    
	gp::Model *woodBall = new gp::Model; woodBall->loadFile("Assets/Models/WoodBall.json");
	_models.push_back(woodBall); woodBall->setPosition(-3.0f, 0.0f, 0.0f); woodBall->setRotation(glm::radians(90.0f), 0.0f, 0.0f);

	gp::Model *metalBall = new gp::Model(); metalBall->loadFile("Assets/Models/MetalBall.json");
	_models.push_back(metalBall); metalBall->setPosition(0.0f, 0.0f, 0.0f); metalBall->setRotation(glm::radians(90.0f), 0.0f, 0.0f);

	gp::Model *paintedBall = new gp::Model(); paintedBall->loadFile("Assets/Models/PaintedBall.json");
	_models.push_back(paintedBall); paintedBall->setPosition(3.0f, 0.0f, 0.0f); paintedBall->setRotation(glm::radians(90.0f), 0.0f, 0.0f);

	gp::Model *sword = new gp::Model; sword->loadFile("Assets/Models/Sword.json");
	_models.push_back(sword); sword->setPosition(0.0f, 2.0f, 0.0f); sword->setScale(0.1f, 0.1f, 0.1f); sword->setRotation(0.0f, 0.0f, glm::radians(90.0f));

	gp::InstancedModel *grass = new gp::InstancedModel;
	grass->loadFile("Assets/Models/Grass.json");
	std::vector<glm::mat4> mats;
	const unsigned int NGRASS = 100;
	for (unsigned int row = 0; row < NGRASS; ++row) {
		for (unsigned int col = 0; col < NGRASS; ++col) {
			glm::mat4 matrix(1.0f);
			float x = (((float)(rand() % 1000)) / 1000.0f) - 0.5f;
			float y = (((float)(rand() % 1000)) / 1000.0f) - 0.5f;
			float z = (((float)(rand() % 1000)) / 1000.0f) - 0.5f;
			matrix = glm::translate(matrix, glm::vec3(((float)row)+(x/2.0f), 0.0f, ((float)col)+(z / 2.0f)));
			matrix = glm::scale(matrix, glm::vec3(0.5f + (x / 3.0f), 0.5f + (x / 3.0f), 0.5f + (x / 3.0f)));
			mats.push_back(matrix);
		}
	}
	grass->setInstanceMatrices(mats);
	//_models.push_back(grass);

	// gp::InstancedModel grass;
	// gp::Model grass; 
	// grass->setInstanceMatrices(const std::vector<glm::mat4> &m);
	// grass->setAnimation(gp::Animation);

    _quad = CreateQuad();
	_cube = CreateCube();
	_skybox = gp::Resources::CreateCubeMapFromHDRerMap("Assets/Skymaps/ninomaru_teien_4k.hdr", 1024);
	_prefilteredBox = gp::Resources::CreatePreFilteredMap(_skybox, 128);
	_irrbox = gp::Resources::CreateIrradianceMap(_skybox, 32);
	_brdfLutTexture = gp::Resources::CreateBrdfLut(512);
    
    camPos = glm::vec3(0.0f, 0.0f, 10.0f);
    camYaw = camPitch = 0.0f;
    bool key_w = false;
    bool key_a = false;
    bool key_s = false;
    bool key_d = false;
    bool key_shift = false;
    float walkspeed = 0.1f;
    float runspeed = 1.0f;
    bool menumode = false;
    bool justSwitch = true;

    while (!gp::Window::ShouldClose()) {
        
        std::vector<gp::Window::Key> keysPressed = gp::Window::KeysPressed();
        std::vector<gp::Window::Key> keysReleased = gp::Window::KeysReleased();
        
        for (gp::Window::Key k : keysPressed) {
            if (k == gp::Window::KEY_W) key_w = true;
            if (k == gp::Window::KEY_A) key_a = true;
            if (k == gp::Window::KEY_S) key_s = true;
            if (k == gp::Window::KEY_D) key_d = true;
            if (k == gp::Window::KEY_LEFT_SHIFT) key_shift = true;
        }
        
        for (gp::Window::Key k : keysReleased) {
            if (k == gp::Window::KEY_W) key_w = false;
            if (k == gp::Window::KEY_A) key_a = false;
            if (k == gp::Window::KEY_S) key_s = false;
            if (k == gp::Window::KEY_D) key_d = false;
            if (k == gp::Window::KEY_LEFT_SHIFT) key_shift = false;
            if (k == gp::Window::KEY_ESCAPE) { menumode = !menumode; justSwitch = true; }
        }
        
        if (!menumode) {
            if (justSwitch) { 
				gp::Window::SetFullScreen(FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT); 
				gp::Window::HideCursor(); 
				justSwitch = false; 
			}
            camYaw -= (float) gp::Window::MouseHorizontal() / 2.0f;
            camPitch -= (float) gp::Window::MouseVertical() / 2.0f;
        } else {
			if (justSwitch) { 
				gp::Window::SetWindowed(WINDOWED_WIDTH, WINDOWED_HEIGHT); 
				gp::Window::ShowCursor(); 
				justSwitch = false; 
			}
            gp::Window::MouseHorizontal();
            gp::Window::MouseVertical();
        }
        
        // calculate rotation
        glm::quat camRHorizontal = glm::normalize(glm::quat(glm::vec3(0.0f, glm::radians(camYaw), 0.0f)));
        glm::quat camRVertical = glm::normalize(glm::quat(glm::vec3(glm::radians(camPitch), 0.0f, 0.0f)));
        glm::vec3 camForward = glm::normalize(glm::vec3(camRHorizontal * camRVertical * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
        glm::vec3 camRight = glm::normalize(glm::vec3(camRHorizontal * camRVertical * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
        glm::vec3 camUp = glm::normalize(glm::vec3(camRHorizontal * camRVertical * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
        
        // calculate position
        if (key_w) camPos += key_shift? (camForward * runspeed) : (camForward * walkspeed);
        if (key_a) camPos -= key_shift? (camRight   * runspeed) : (camRight   * walkspeed);
        if (key_s) camPos -= key_shift? (camForward * runspeed) : (camForward * walkspeed);
        if (key_d) camPos += key_shift? (camRight   * runspeed) : (camRight   * walkspeed);
        
        float aspect = ((float) gp::Window::GetWidth()) / ((float) gp::Window::GetHeight());
        _proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
        
        glm::vec3 center = camPos + camForward;
        _view = glm::lookAt(camPos, center, camUp);
        //std::cout << "Pitch: " << camPitch << " " << "Yaw: " << camYaw << std::endl;
        
        gp::Window::PollEvents();
		glViewport(0, 0, gp::Window::GetWidth(), gp::Window::GetHeight());
        DrawGeom();
        DrawLights();
		DrawSkyBox();
        
        _lightBuffer->bindRead();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, gp::Window::GetWidth(), gp::Window::GetHeight(),
                          0, 0, gp::Window::GetWidth(), gp::Window::GetHeight(),
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        gp::Window::SwapBuffers();
        
        Tick();
    }
    
    gp::Window::Close();
    return 0;
}


void DrawGeom()
{
    // bind geometry frame buffer for writing
    _geomBuffer->bindWrite();
    _geomBuffer->resize(gp::Window::GetWidth(), gp::Window::GetHeight());
    _geomBuffer->clear();
    
    // draw the geometry
    for (gp::Model *m : _models) {
        m->draw(_proj, _view);
    }
}


void DrawLights()
{
	glDepthMask(GL_FALSE);

    // bind light frame buffer for writing
    _lightBuffer->bindWrite();
    _lightBuffer->resize(gp::Window::GetWidth(), gp::Window::GetHeight());
    _lightBuffer->clear();
 
    // bind geometry frame buffer for reading
    _geomBuffer->bindRead();
    
    // bind ambient shader
    _ambientShader->bind();
    _ambientShader->setTexture("uPosition", 0, _geomBuffer->getPositionID());
    _ambientShader->setTexture("uNormal", 1, _geomBuffer->getNormalID());
    _ambientShader->setTexture("uColor", 2, _geomBuffer->getColorID());
    _ambientShader->setTexture("uMRA", 3, _geomBuffer->getMRAID());
	_ambientShader->setVec3("uCamPos", camPos);
	_ambientShader->setCubeMap("uIrradiance", 4, *_irrbox);
	_ambientShader->setTexture("uBrdfLut", 5, *_brdfLutTexture);
	_ambientShader->setCubeMap("uPrefilteredMap", 6, *_prefilteredBox);
    _quad->draw();
    
    
    // bind small light shader
    _lightShader->bind();
    _lightShader->setMat4("uProj", _proj);
    _lightShader->setMat4("uView", _view);
    _lightShader->setTexture("uPosition", 0, _geomBuffer->getPositionID());
    _lightShader->setTexture("uNormal", 1, _geomBuffer->getNormalID());
    _lightShader->setTexture("uColor", 2, _geomBuffer->getColorID());
    _lightShader->setTexture("uMRA", 3, _geomBuffer->getMRAID());

    // draw the lights
    for (gp::Light *l : _lights)
        l->draw(_lightShader);
    
	glDepthMask(GL_TRUE);
}


void DrawSkyBox()
{
	_skyShader->bind();
	_skyShader->setMat4("uProj", _proj);
	_skyShader->setMat4("uView", _view);
	//_skyShader->setCubeMap("uERMap", 0, *_irrbox);
	_skyShader->setCubeMap("uERMap", 0, *_skybox);
	//_skyShader->setCubeMap("uERMap", 0, *_prefilteredBox);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _geomBuffer->getID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _lightBuffer->getID());
	glBlitFramebuffer(0, 0, gp::Window::GetWidth(), gp::Window::GetHeight(), 0, 0, gp::Window::GetWidth(), gp::Window::GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	_cube->draw();	
}


gp::Mesh* CreateQuad()
{
    std::vector<gp::Vertex> verts;
    std::vector<unsigned int> indices;
    
    gp::Vertex v0, v1, v2, v3;
    v0.position = glm::vec3(-1.0f, -1.0f, 0.0f); v0.uv = glm::vec2(0.0f, 0.0f); verts.push_back(v0);
    v1.position = glm::vec3( 1.0f, -1.0f, 0.0f); v1.uv = glm::vec2(1.0f, 0.0f); verts.push_back(v1);
    v2.position = glm::vec3(-1.0f,  1.0f, 0.0f); v2.uv = glm::vec2(0.0f, 1.0f); verts.push_back(v2);
    v3.position = glm::vec3( 1.0f,  1.0f, 0.0f); v3.uv = glm::vec2(1.0f, 1.0f); verts.push_back(v3);
    
    indices.push_back(0); indices.push_back(2); indices.push_back(3);
    indices.push_back(3); indices.push_back(1); indices.push_back(0);
    
    return new gp::Mesh(verts, indices);
}


gp::Mesh* CreateCube()
{
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

	positions.push_back(glm::vec3(-sz, -sz,  sz)); // 0
	positions.push_back(glm::vec3( sz, -sz,  sz)); // 1
	positions.push_back(glm::vec3(-sz, -sz, -sz)); // 2
	positions.push_back(glm::vec3( sz, -sz, -sz)); // 3
	positions.push_back(glm::vec3(-sz,  sz,  sz)); // 4
	positions.push_back(glm::vec3( sz,  sz,  sz)); // 5
	positions.push_back(glm::vec3(-sz,  sz, -sz)); // 6
	positions.push_back(glm::vec3( sz,  sz, -sz)); // 7

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

	return new gp::Mesh(positions, indices);
}


void Tick()
{
    static bool hasInit = false;
    static int count = 0;
    static std::chrono::steady_clock::time_point last;
    
    if (!hasInit) {
        hasInit = true;
        last = std::chrono::high_resolution_clock::now();
    }
    
    ++count;
    auto now = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count();
    if (diff >= 1000000) {
        std::cout << "FPS: " << count << std::endl;
        last = now;
        count = 0;
    }
}
