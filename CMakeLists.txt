cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simulator VERSION 1.0)
find_package(OpenGL REQUIRED)

set(HEADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/include)
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(GLFW_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/lib/glfw3.lib)

set (HEADER_FILES
    ${HEADERS_PATH}/GLFW/glfw3.h
    ${HEADERS_PATH}/glad/glad.h
    ${HEADERS_PATH}/stb_image/stb_image.h
    )

set (SOURCE_FILES 
    ${SRC_PATH}/glad.c
    ${SRC_PATH}/DeltaTime.cpp
    ${SRC_PATH}/Shader.cpp
    ${SRC_PATH}/Particles.cpp
    ${SRC_PATH}/image_loader.cpp
    ${SRC_PATH}/main.cpp
    ${SRC_PATH}/Camera.cpp
    ${SRC_PATH}/InputHandler.cpp
    )

add_executable(simulator ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(simulator PUBLIC ${HEADERS_PATH})
target_link_libraries(simulator ${OPENGL_LIBRARIES} ${GLFW_LIBRARY})
