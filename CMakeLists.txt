cmake_minimum_required(VERSION 3.20)

project(engine)

add_executable(engine)

target_sources(engine
  PRIVATE
    src/main.cpp
    src/glad.c
    src/texture.cpp
    src/texture.hpp
    src/shader.cpp
    src/shader.hpp
    src/camera.cpp
    src/camera.hpp
    src/object.cpp
    src/object.hpp
    src/stb_image.cpp
    include
)

if (WIN32)
target_link_libraries(engine
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/windows/glfw3.lib
)
else()
target_link_libraries(engine
  PRIVATE
   ${CMAKE_SOURCE_DIR}/lib/linux/libglfw3.a
)
endif()
