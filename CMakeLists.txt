cmake_minimum_required(VERSION 3.24)

project(barfetto VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if (NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif ()

add_subdirectory(coremio)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_gfx REQUIRED)

set(barfetto_codebase
  src/animation.c
  src/animation_pack.c
  src/barf_object.c
  src/camera.c
  src/color.c
  src/entity.c
  src/font_manager.c
  src/image.c
  src/layer.c
  src/point.c
  src/rectangle.c
  src/renderer.c
  src/terminator_listener.c
  src/time_manager.c
  src/ui/ui_button.c
  src/ui/ui_label.c
  src/ui/ui_single_line_field.c
  # this is to debug, just an entry point. It is going to disappear
  tainted_sails/src/tainted_sails.c
)
add_executable(barfetto ${barfetto_codebase})

include_directories(barfetto PRIVATE 
  coremio/include
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS}
  ${SDL2_GFX_INCLUDE_DIRS})

target_link_libraries(barfetto PRIVATE
  ${SDL2_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
  ${SDL2_GFX_LIBRARIES}
  coremio
  Threads::Threads
  m
)
