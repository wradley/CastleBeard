cmake_minimum_required(VERSION 3.11.0)
project(CastleBeard)
set(CMAKE_CXX_STANDARD 14)

# defines
set(ASSETS_DIR_FILEPATH CACHE PATH "Filepath to assets directory")
configure_file(${PROJECT_SOURCE_DIR}/Include/Defines.h.in ${PROJECT_SOURCE_DIR}/Include/Defines.h)

# glfw
add_subdirectory(Libraries/glfw-3.2.1)
set(GLFW_INCLUDE_DIR Libraries/glfw-3.2.1/include)

# glad
set(GLAD_INCLUDE_DIR Libraries/glad/include)

# fbx sdk
set(FBX_SDK_INCLUDE_DIR "" CACHE PATH "FBX SDK include directory")
set(FBX_SDK_RELEASE_LIB "" CACHE FILEPATH "FBX SDK static multithreaded release library")
set(FBX_SDK_DEBUG_LIB "" CACHE FILEPATH "FBX SDK static multithreaded debug library")

# set flags
if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -framework Carbon -framework SystemConfiguration")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -framework Carbon -framework SystemConfiguration")
endif()

# gather source files
set(GLAD_SRC Libraries/glad/src/glad.c)
set(ALL_SRC_FILES ${GLAD_SRC}
    Source/main.cpp
    Source/Vector.cpp
    Source/Matrix.cpp
    Source/Quaternion.cpp
    Source/Graphics/Vertex.cpp
    Source/Graphics/MeshData.cpp
    Source/Graphics/Mesh.cpp
    Source/Graphics/Model.cpp
    Source/Graphics/Importer.cpp
)

# build
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${FBX_SDK_INCLUDE_DIR})
add_executable(CastleBeard ${ALL_SRC_FILES})
target_link_libraries(CastleBeard glfw ${GLFW_LIBRARIES} debug ${FBX_SDK_DEBUG_LIB} optimized ${FBX_SDK_RELEASE_LIB})
