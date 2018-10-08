#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include "../Include/Math/Vector.h"
#include "../Include/Math/Math.h"
#include "../Include/Math/Matrix.h"
#include "../Include/Math/Quaternion.h"
#include "../Include/Graphics/Model.h"
#include "../Include/Defines.h"
#include "../Include/Graphics/Importer.h"
#include "../Include/Graphics/Shader.h"

const std::string ASSETS_DIR(ASSETS_DIR_FILEPATH);

struct UserInput {
    double scrollOffset;
    double lastMouseX;
    double lastMouseY;
    double currMouseX;
    double currMouseY;
    bool isClicking;
    bool isRClicking;
    int clickTime;
    UserInput()
    {
        scrollOffset = 0.0;
        lastMouseX = 0.0;
        lastMouseY = 0.0;
        currMouseX = 0.0;
        currMouseY = 0.0;
        isClicking = false;
        isRClicking = false;
        clickTime = 0;
    }
} userInput;

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    userInput.scrollOffset = yOffset;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS) userInput.isClicking = true;
        if (action == GLFW_RELEASE) {
            userInput.isClicking = false; userInput.clickTime = 0;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS) userInput.isRClicking = true;
        if (action == GLFW_RELEASE) userInput.isRClicking = false;
    }
}

std::string LoadTextFile(const std::string &filepath)
{
    // open and check file
    std::ifstream input(filepath, std::ios::in | std::ios::binary);
    if (!input.is_open()) {
        std::cout << "Could not open file: " << filepath << std::endl;
        return std::string();
    }

    // alloc space in string
    std::string text;
    input.seekg(0, std::ios::end);
    text.resize(input.tellg());
    input.seekg(0, std::ios::beg);

    // load and return string
    input.read(&text[0], text.size());
    return text;
}



Graphics::Model *model1;
Graphics::Model *model2;

void CreateCube()
{
    Graphics::Importer importer;
    importer.loadFile(ASSETS_DIR + "/SphereFlat.fbx");
    model1 = new Graphics::Model(importer.getMeshDataPointers());

    importer.loadFile(ASSETS_DIR + "/Sphere.fbx");
    model2 = new Graphics::Model(importer.getMeshDataPointers());
}

int main(int argc, char **argv) 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "CMakeTest", NULL, NULL);
    if (!window) {
        std::cout << "Could not create glfw window" << std::endl;
       glfwTerminate();
      return -1; 
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not init glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    // resize for mac retna
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    float r = 0.5f;
    int rdir = 1;
    float g = 0.0f;
    int gdir = 1;
    float b = 0.0f;
    int bdir = 1;

    //glViewport(0, 0, 800, 600);
    glClearColor(r, g, b, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //CreateShader();
    std::string vCode = LoadTextFile(ASSETS_DIR + "/vertex.glsl");
    std::string fCode = LoadTextFile(ASSETS_DIR + "/fragment.glsl");
    const char *vCodeC = vCode.c_str();
    const char *fCodeC = fCode.c_str();
    Graphics::Shader shader;
    shader.compileFiles(vCodeC, fCodeC);
    shader.bind();
    CreateCube();
    //glUseProgram(shader);

    float zrot = 0.0f;
    const Math::Vec3 OG_CAM_POS(0.0f, 0.0f, 5.0f);
    Math::Vec3 camPos(OG_CAM_POS);
    Math::Vec3 camUp(OG_CAM_POS);
    float pitch = 0.0f;
    float yaw = 0.0f;
    Math::Mat4 model(Math::Mat4::FromQuat(Math::Quat(Math::Vec3(0.0f, zrot, 0.0f))));
    Math::Mat4 proj(Math::Perspective(Math::ToRadians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f));
    Math::Mat4 view(Math::LookAt(camPos, Math::Vec3(0.0f, 0.0f, 0.0f), Math::Vec3(0.0f, 1.0f, 0.0f)));
    auto result = proj * view * model * Math::Vec4(Math::Vec3(-1.0f, -1.0f, 0.0f), 1.0f);

    while(!glfwWindowShouldClose(window))
    {
        // move model
        if (userInput.isClicking) {
            
            userInput.lastMouseX = userInput.currMouseX;
            userInput.lastMouseY = userInput.currMouseY;
            glfwGetCursorPos(window, &userInput.currMouseX, &userInput.currMouseY);
            if (userInput.clickTime == 0) {
                userInput.lastMouseX = userInput.currMouseX;
                userInput.lastMouseY = userInput.currMouseY;
            }

            double xDiff = (userInput.currMouseX - userInput.lastMouseX);
            double yDiff = (userInput.currMouseY - userInput.lastMouseY);
            yaw -= (float) xDiff;
            pitch -= (float) yDiff;
            // clamp horizontal rotation
            if (pitch >  89.0) pitch =  89.0;
            if (pitch < -89.0) pitch = -89.0;
            //Math::Vec2 mouse(Math::ToRadians((float)pitch), Math::ToRadians((float)yaw));

            Math::Quat hor(Math::Vec3(0.0f, Math::ToRadians((float)yaw), 0.0f));
            Math::Quat vert(Math::Vec3(Math::ToRadians((float)pitch), 0.0f, 0.0f));
            //view = view * Math::Mat4::FromQuat(xRot * yRot);
            //camPos.x += xDiff/100.0;
            camPos = Math::Mat4::FromQuat(hor*vert) * OG_CAM_POS;// *Math::Vec4(camPos, 1.0f);
            view = Math::LookAt(camPos, Math::Vec3(0.0f, 0.0f, 0.0f), Math::Vec3(0.0f, 1.0f, 0.0f));

            ++userInput.clickTime;
            std::cout << camPos.x << ", " << camPos.y << ", " << camPos.z << std::endl;
        }

        // reset model
        if (userInput.isRClicking) {
            camPos = OG_CAM_POS;
            view = Math::LookAt(camPos, Math::Vec3(0.0f, 0.0f, 0.0f), Math::Vec3(0.0f, 1.0f, 0.0f));
        }

        //zrot += Math::ToRadians(1.0f);
        //if (isClicking) zrot -= Math::ToRadians(2.0f);
        //Math::Mat4 model(Math::Mat4::FromQuat(Math::Quat(Math::Vec3(zrot, 0.0f, 0.0f))));

        shader.setMat4("uProj", proj);
        shader.setMat4("uView", view);
        shader.setVec3("uColor", Math::Vec3(1.0f - r, 1.0f - g, 1.0f - b));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, 1.0f);
        //glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        
        shader.setMat4("uModel", Math::Transpose(model.values, Math::Vec3(1.1f, 1.1f, 0.0f)));
        model1->draw();
        
        shader.setMat4("uModel", Math::Transpose(model.values, Math::Vec3(-1.1f, 1.1f, 0.0f)));
        model2->draw();

        shader.setMat4("uModel", Math::Transpose(model.values, Math::Vec3(-1.1f, -1.1f, 0.0f)));
        model1->draw();

        shader.setMat4("uModel", Math::Transpose(model.values, Math::Vec3(1.1f, -1.1f, 0.0f)));
        model2->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
        
	    if (r >= 1.0f) rdir = -1;
	    if (r <= 0.1f) rdir = 1;
        
	    if (g >= 1.0f) gdir = -1;
	    if (g <= 0.1f) gdir = 1;
        
	    if (b >= 1.0f) bdir = -1;
	    if (b <= 0.1f) bdir = 1;

        r += 0.001f * (float) rdir * 2.0f;
	    g += 0.0009f * (float) gdir * 2.0f;
        b += 0.003f * (float) bdir * 2.0f;
            
    }

    return 0;
}
