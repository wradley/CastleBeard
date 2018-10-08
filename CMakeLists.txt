cmake_minimum_required(VERSION 3.11.0)
project(CastleBeard)
set(CMAKE_CXX_STANDARD 14)

# defines
set(ASSETS_DIR_FILEPATH "REQUIRED" CACHE PATH "Filepath to assets directory")
configure_file(${PROJECT_SOURCE_DIR}/Include/Defines.h.in ${PROJECT_SOURCE_DIR}/Include/Defines.h)

# glfw
set(GLFW_INCLUDE_DIR "REQUIRED" CACHE PATH "GLFW include directory")
set(GLFW_RELEASE_LIB "REQUIRED" CACHE FILEPATH "GLFW static release library")
set(GLFW_DEBUG_LIB "REQUIRED" CACHE FILEPATH "GLFW static debug library")

# glad
set(GLAD_INCLUDE_DIR Libraries/glad/include)

# fbx sdk
set(FBX_SDK_INCLUDE_DIR "REQUIRED" CACHE PATH "FBX SDK include directory")
set(FBX_SDK_RELEASE_LIB "REQUIRED" CACHE FILEPATH "FBX SDK static multithreaded release library")
set(FBX_SDK_DEBUG_LIB "REQUIRED" CACHE FILEPATH "FBX SDK static multithreaded debug library")

# set windows flags
if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(GLAD_SRC Libraries/glad/src/glad.c)
set(ALL_SRC_FILES
    ${GLAD_SRC}
    Source/main.cpp
    Source/Math/Vector.cpp
    Source/Math/Matrix.cpp
    Source/Math/Quaternion.cpp
    Source/Graphics/Vertex.cpp
    Source/Graphics/MeshData.cpp
    Source/Graphics/Mesh.cpp
    Source/Graphics/Model.cpp
    Source/Graphics/Importer.cpp
    Source/Graphics/Shader.cpp
)

# build
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${FBX_SDK_INCLUDE_DIR})
add_executable(CastleBeard ${ALL_SRC_FILES})


target_link_libraries(CastleBeard
    debug ${FBX_SDK_DEBUG_LIB}
    optimized ${FBX_SDK_RELEASE_LIB}
)

target_link_libraries(CastleBeard
    debug ${GLFW_DEBUG_LIB}
    optimized ${GLFW_RELEASE_LIB}
)

if (APPLE)
    #target_link_libraries(CastleBeard "-framework Carbon")
    #target_link_libraries(CastleBeard "-framework SystemConfiguration")
    target_link_libraries(CastleBeard "-framework Cocoa")
    target_link_libraries(CastleBeard "-framework OpenGL")
    target_link_libraries(CastleBeard "-framework IOKit")
    target_link_libraries(CastleBeard "-framework CoreVideo")
endif()
