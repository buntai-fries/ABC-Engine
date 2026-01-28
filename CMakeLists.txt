cmake_minimum_required(VERSION 3.26)
project(ABC_Engine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

# Windows GLFW
if(WIN32)
    set(GLFW_DIR "${CMAKE_SOURCE_DIR}/dependencies/GLFW")
    find_library(GLFW_LIB NAMES glfw3 glfw3d PATHS "${GLFW_DIR}/lib" NO_DEFAULT_PATH)
    if(NOT GLFW_LIB)
        message(FATAL_ERROR "GLFW library not found in ${GLFW_DIR}/lib")
    endif()
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")  # /MD
endif()

# Output directories
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
    ${CMAKE_SOURCE_DIR}/src     
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/glad
    ${CMAKE_SOURCE_DIR}/dependencies/GLFW/GLFW
    ${CMAKE_SOURCE_DIR}/dependencies/stbimage
)

# Cross-platform linking
if(WIN32)
    target_link_libraries(ABC_Engine PRIVATE 
    ${GLFW_LIB} 
    OpenGL::GL
)
elseif(LINUX)
    set_target_properties(ABC_Engine PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib-linux"
        INSTALL_RPATH "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib-linux"
    )
    # Copy command (run once): cp /usr/lib64/libglfw.so* dependencies/GLFW/lib-linux/
    target_link_options(ABC_Engine PRIVATE 
        -L${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib-linux 
        -lglfw 
        -lX11 -lpthread -ldl
    )
    target_link_libraries(ABC_Engine PRIVATE OpenGL::GL)
endif()
