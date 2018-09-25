#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Include/Math/Vector.h"
#include "../Include/Math/Matrix.h"
#include "../Include/Math/Quaternion.h"


//  2---3
//  |  /|
//  |/  |
//  0---1

Math::Vec3 verts[] = {
    {-0.5f, -0.5f, 0.0f},
    { 0.5f, -0.5f, 0.0f},
    {-0.5f,  0.5f, 0.0f},
    { 0.5f,  0.5f, 0.0f},
};

unsigned int indices[] = {
    0, 2, 3,
    3, 1, 0
};

unsigned int vao, vbo, ebo;

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

const char *vShaderCode = R"(
  #version 410 core
  layout (location = 0) in vec3 aPos;
  
  void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)";

const char *fShaderCode = R"(
  #version 410 core
  out vec4 FragColor;

  void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
)";

unsigned int shader;

void CreateShader()
{
    int success;
    char infoLog[1024];
    
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not init glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    float r = 0.0f;
    int rdir = 1;
    float g = 0.0f;
    int gdir = 1;
    float b = 0.0f;
    int bdir = 1;

    glViewport(0, 0, 800, 600);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    CreateShader();
    CreateQuad();
    glUseProgram(shader);
    glBindVertexArray(vao);

    //Math::Quat qa = Math::Normalize(Math::Quat::FromEuler({ 1.0f, 2.0f, 3.0f }));
    //Math::Quat qb = Math::Normalize(Math::Quat::FromEuler({ 1.2f, 0.2f, 0.1f }));
    Math::Quat qa(Math::Vec3(1.0f, 2.0f, 3.0f));
    Math::Quat qb(Math::Vec3(1.2f, 0.2f, 0.1f));
    
    Math::Quat qc = qa * qb;

    auto mat = Math::Mat4::FromQuat(qc);
    Math::Vec4 pos(1.0f);

    pos = mat * pos;

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(r, g, b, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
	    if (r >= 1.0f) rdir = -1;
	    if (r <= 0.0f) rdir = 1;
        
	    if (g >= 1.0f) gdir = -1;
	    if (g <= 0.0f) gdir = 1;
        
	    if (b >= 1.0f) bdir = -1;
	    if (b <= 0.0f) bdir = 1;

        r += 0.001f * (float) rdir;
	    g += 0.002f * (float) gdir;
        b += 0.003f * (float) bdir;
            
    }

    return 0;
}
