#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fbxsdk.h>
#include <vector>
#include <string>
#include "../Include/Math/Vector.h"
#include "../Include/Math/Matrix.h"
#include "../Include/Math/Quaternion.h"
#include "../Include/Graphics/Model.h"


void GetMeshFromFBX(FbxManager *manager, FbxMesh *mesh, Graphics::MeshData &retMesh)
{
    // make sure mesh is triangulated
    if (!mesh->IsTriangleMesh()) {
        FbxGeometryConverter converter(manager);
        mesh = (FbxMesh*)converter.Triangulate(mesh, true);
        if (!mesh) {
            std::cout << "Error triangulating mesh" << std::endl;
            return;
        } if (mesh->GetAttributeType() != FbxNodeAttribute::EType::eMesh) {
            std::cout << "Error triangulating mesh, fbx returned non-mesh attribute" << std::endl;
            return;
        }
    }

    auto numVerts = mesh->GetControlPointsCount();
    auto numIndices = mesh->GetPolygonVertexCount();
    auto fbxverts = mesh->GetControlPoints();
    auto fbxindices = mesh->GetPolygonVertices();
    retMesh.vertices.resize(numVerts);
    retMesh.indices.resize(numIndices);

    for (int i = 0; i < numVerts; ++i) {
        retMesh.vertices[i].position.x = (float) fbxverts[i][0];
        retMesh.vertices[i].position.y = (float) fbxverts[i][1];
        retMesh.vertices[i].position.z = (float) fbxverts[i][2];
    }

    for (int i = 0; i < numIndices; ++i) {
        retMesh.indices[i] = fbxindices[i];
    }
}

void TraverseFBXTree(FbxManager *manager, FbxNode *node, std::vector<Graphics::MeshData*> &retMeshDatas)
{
    for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {
        if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
            Graphics::MeshData *mesh = new Graphics::MeshData;
            GetMeshFromFBX(manager, (FbxMesh*)node->GetNodeAttributeByIndex(i), *mesh);
            retMeshDatas.push_back(mesh);
        }
    }
    
    for (int i = 0; i < node->GetChildCount(); ++i) {
        TraverseFBXTree(manager, node->GetChild(i), retMeshDatas);
    }
}

void LoadFBXFile(const std::string &filepath, std::vector<Graphics::MeshData*> &retMeshDatas)
{
    FbxManager *manager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);

    FbxImporter *importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings())) {
        std::cout << "Could not initialize FBX Importer: " << importer->GetStatus().GetErrorString() << std::endl;
        return;
    }

    FbxScene *scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    importer->Destroy();

    FbxNode *root = scene->GetRootNode();
    TraverseFBXTree(manager, root, retMeshDatas);
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
    std::vector<Graphics::MeshData*> modelData;
    LoadFBXFile("C:/Users/calla/Desktop/Cube.fbx", modelData);
    models.push_back(new Graphics::Model(modelData));
}

const char *vShaderCode = R"(
  #version 410 core
  layout (location = 0) in vec3 aPos;

  uniform mat4 uProj;
  uniform mat4 uView;
  uniform mat4 uModel;

  void main() {
    gl_Position = uProj * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  }
)";

const char *fShaderCode = R"(
  #version 410 core
  out vec4 FragColor;

  uniform vec3 uColor;

  void main() {
    FragColor = vec4(uColor.r, uColor.g, uColor.b, 1.0);
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
    glClear(GL_COLOR_BUFFER_BIT);

    CreateShader();
    //CreateQuad();
    CreateCube();
    glUseProgram(shader);
    //glBindVertexArray(vao);

    float zrot = 0.0f*(3.14159f/180.0f);
    Math::Mat4 model(Math::Mat4::FromQuat(Math::Quat(Math::Vec3(0.0f, zrot, 0.0f))));
    Math::Mat4 proj(Math::Perspective(60.0f*(3.14159f/180.0f), 800.0f/600.0f, 0.1f, 100.0f));
    Math::Mat4 view(Math::LookAt(Math::Vec3(0.0f, 0.0f, 5.0f), Math::Vec3(0.0f, 0.0f, 0.0f), Math::Vec3(0.0f, 1.0f, 0.0f)));
    auto result = proj * view * model * Math::Vec4(Math::Vec3(-1.0f, -1.0f, 0.0f), 1.0f);

    while(!glfwWindowShouldClose(window))
    {
        zrot += 1.0f * (3.14159f/180.0f);
        Math::Mat4 model(Math::Mat4::FromQuat(Math::Quat(Math::Vec3(zrot, 0.0f, 0.0f))));
        glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, model.values);
        glUniformMatrix4fv(glGetUniformLocation(shader, "uProj"), 1, GL_FALSE, proj.values);
        glUniformMatrix4fv(glGetUniformLocation(shader, "uView"), 1, GL_FALSE, view.values);
        glUniform3f(glGetUniformLocation(shader, "uColor"), 1.0f - r, 1.0f - g, 1.0f - b);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(r, g, b, 1.0f);
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
