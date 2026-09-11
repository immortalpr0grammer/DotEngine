cmake_minimum_required(VERSION 3.20)

project(engine)

add_executable(engine)

target_sources(engine
  PRIVATE
    src/main.cpp
    src/glad.c
    src/shader.cpp
    src/shader.hpp
    src/stb_image.cpp
    include
)

target_link_libraries(engine
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)
