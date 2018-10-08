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


//  2---3
//  |  /|
//  |/  |
//  0---1
float q_sz = 1.0f;
float q_dst = 0.0f;
Math::Vec3 verts[] = {
    {-q_sz, -q_sz, -q_dst},
    { q_sz, -q_sz, -q_dst},
    {-q_sz,  q_sz, -q_dst},
    { q_sz,  q_sz, -q_dst}
};

unsigned int indices[] = {
    0, 2, 3,
    3, 1, 0
};

unsigned int vao, vbo, ebo;
std::vector<Graphics::Model*> models;

void CreateQuad()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
}

void CreateCube()
{
    Graphics::Importer importer;
    importer.loadFile(ASSETS_DIR + "/Cube.fbx");
    models.push_back(new Graphics::Model(importer.getMeshDataPointers()));
}

unsigned int shader;

void CreateShader()
{
    int success;
    char infoLog[1024];

    std::string vertexString = LoadTextFile(ASSETS_DIR + "/vertex.glsl");
    std::string fragmentString = LoadTextFile(ASSETS_DIR + "/fragment.glsl");

    const char *vShaderCode = vertexString.c_str();
    const char *fShaderCode = fragmentString.c_str();
    
    // vertex
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderCode, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader, 1024, NULL, infoLog);
        std::cout << "Error compiling vertex shader: " << infoLog << std::endl;
        return;
    }

    // fragment
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderCode, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
        std::cout << "Error compiling fragment shader: " << infoLog << std::endl;
        glDeleteShader(vShader);
        return;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vShader);
    glAttachShader(shader, fShader);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "Error linking shader program: " << infoLog << std::endl;
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
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

    float r = 0.0f;
    int rdir = 1;
    float g = 0.0f;
    int gdir = 1;
    float b = 0.0f;
    int bdir = 1;

    //glViewport(0, 0, 800, 600);
    glClearColor(r, g, b, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CreateShader();
    CreateCube();
    glUseProgram(shader);

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
        glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, model.values);
        glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, proj.values);
        glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, view.values);
        glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f - r, 1.0f - g, 1.0f - b);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        for (auto &m : models) m->draw();
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
	    if (r >= 1.0f) rdir = -1;
	    if (r <= 0.0f) rdir = 1;
        
	    if (g >= 1.0f) gdir = -1;
	    if (g <= 0.0f) gdir = 1;
        
	    if (b >= 1.0f) bdir = -1;
	    if (b <= 0.0f) bdir = 1;

        r += 0.001f * (float) rdir * 2.0f;
	    g += 0.002f * (float) gdir * 2.0f;
        b += 0.003f * (float) bdir * 2.0f;
            
    }

    return 0;
}
