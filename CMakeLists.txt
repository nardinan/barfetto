cmake_minimum_required(VERSION 3.10)

project(barfetto C)

set(COREMIO_INCLUDE_DIRS coremio/include/) # libcoremio include directories
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

add_executable(barfetto
  src/animation.c
  src/barf_object.c
  src/camera.c
  src/color.c
  src/animation_pack.c
  src/image.c
  src/layer.c
  src/renderer.c
  src/terminator_listener.c
  tainted_sails/src/tainted_sails.c
  include/barfetto/entity.h
  src/entity.c)
add_subdirectory(coremio)

include(FindPkgConfig)

if (NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
  set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_gfx REQUIRED)

include_directories(${PROJECT_NAME} PRIVATE ${STATUS_SDL_INCLUDE} ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS} ${COREMIO_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${STATUS_SDL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES} ${SDL2_GFX_LIBRARIES} coremio pthread m)

include(CMakePrintHelpers)
cmake_print_variables(SDL2_LIBRARIES)
cmake_print_variables(SDL2_GFX_LIBRARIES)
cmake_print_variables(SDL2_MIXER_LIBRARIES)