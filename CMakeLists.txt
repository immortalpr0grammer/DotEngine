cmake_minimum_required(VERSION 4.3.4)

project(engine)

add_executable(engine)

target_sources(engine
  PRIVATE
    src/main.cpp
    src/glad.c
    include/GLFW/glfw3.h
    include/glad/glad.h
    src/shader.hpp
    src/shader.cpp
)

target_link_libraries(engine
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)
