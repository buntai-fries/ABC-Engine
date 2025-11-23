cmake_minimum_required(VERSION 3.26)
project(ABC_Engine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

find_package(OpenGL REQUIRED)

# Manager folder directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_SOURCE_DIR}/bin)

add_executable(ABC_Engine
    src/glad.c
    src/main.cpp
    src/Shader.cpp
)

target_include_directories(ABC_Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/glad
    ${CMAKE_SOURCE_DIR}/dependencies/GLFW
)

# Link glfw manually (since it's not system-installed)
target_link_directories(ABC_Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib
)

target_link_libraries(ABC_Engine PRIVATE
    glfw3
    OpenGL::GL
)
