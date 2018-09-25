cmake_minimum_required(VERSION 3.11.0)
project(CastleBeard)
set(CMAKE_CXX_STANDARD 14)

# glfw
add_subdirectory(Libraries/glfw-3.2.1)
set(GLFW_INCLUDE_DIR Libraries/glfw-3.2.1/include)


# glad
set(GLAD_INCLUDE_DIR Libraries/glad/include)


# gather source files
set(GLAD_SRC Libraries/glad/src/glad.c)
set(ALL_SRC_FILES ${GLAD_SRC}
    Source/main.cpp
    Source/Vector.cpp
    Source/Matrix.cpp
    Source/Quaternion.cpp
)


include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})
add_executable(CastleBeard ${ALL_SRC_FILES})
target_link_libraries(CastleBeard glfw ${GLFW_LIBRARIES})
