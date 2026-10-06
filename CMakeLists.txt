cmake_minimum_required(VERSION 3.20)

project(engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

    src/engine.cpp
    src/engine.hpp

    src/mesh.cpp
    src/mesh.hpp

    src/model.cpp
    src/model.hpp

    src/stb_image.cpp

    include
)

#TODO: check for windows and do something else than this if so

target_include_directories(engine
  PRIVATE
   ${CMAKE_SOURCE_DIR}/include/linux
   ${CMAKE_SOURCE_DIR}/include
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
   ${CMAKE_SOURCE_DIR}/lib/linux/libassimp.so.6
)
endif()
