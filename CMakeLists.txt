set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.26)
project(ABC_Engine VERSION 1.0.0 LANGUAGES C CXX)
# Prefer GLVND on platforms that support it (modern OpenGL handling)
cmake_policy(SET CMP0072 NEW)
# Language standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# On MSVC, use /MD runtime (MultiThreadedDLL)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

# Output directory for the executable (all configs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# OpenGL
find_package(OpenGL REQUIRED)

# Sources
add_executable(ABC_Engine
    src/glad.c
    src/main.cpp
    src/Shader.cpp
)

# ---- Include paths ----
target_include_directories(ABC_Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/glad
    ${CMAKE_SOURCE_DIR}/dependencies/GLFW/include
    ${CMAKE_SOURCE_DIR}/dependencies/stbimage
)

# ---- GLFW linking (cross‑platform) ----
# Assumptions:
#   Windows:  ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib/glfw3.lib (or glfw3dll.lib)
#   Linux:    ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib-linux/libglfw.so
#             (or system glfw in /usr/lib64, /usr/lib, /usr/local/lib)

if(WIN32)
    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib")

    find_library(GLFW_LIB
        NAMES glfw3 glfw3dll
        PATHS "${GLFW_LIB_DIR}"
        NO_DEFAULT_PATH
        REQUIRED
    )

    target_link_libraries(ABC_Engine PRIVATE
        ${GLFW_LIB}
        OpenGL::GL
    )

elseif(UNIX AND NOT APPLE)
    # Try vendored first, then common system paths
    set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib-linux")

    find_library(GLFW_LIB
        NAMES glfw glfw3
        PATHS
            "${GLFW_LIB_DIR}"
            /usr/lib64
            /usr/lib
            /usr/local/lib
        REQUIRED
    )

    target_link_libraries(ABC_Engine PRIVATE
        ${GLFW_LIB}
        OpenGL::GL
        X11
        pthread
        dl
    )
endif()

# Copies the assets/ directory next to the built executable.
add_custom_command(TARGET ABC_Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:ABC_Engine>/assets"
)
