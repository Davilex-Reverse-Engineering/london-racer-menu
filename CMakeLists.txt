cmake_minimum_required(VERSION 3.20)

project(menu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  set(CMAKE_CXX_FLAGS_DEBUG "-g3")
endif()

set(SDL_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDL_STATIC ON CACHE INTERNAL "" FORCE)
set(SDL_DISABLE_INSTALL ON CACHE INTERNAL "" FORCE)
set(SDL_DISABLE_INSTALL_DOCS ON CACHE INTERNAL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE INTERNAL "" FORCE)
set(SDL_TESTS OFF CACHE INTERNAL "" FORCE)
set(SDL_EXAMPLES OFF CACHE INTERNAL "" FORCE)
add_subdirectory(vendor/SDL)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_VENDORED ON CACHE INTERNAL "" FORCE)
set(SDLTTF_SAMPLES OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_FREETYPE ON CACHE INTERNAL "" FORCE)
set(SDLTTF_HARFBUZZ OFF CACHE INTERNAL "" FORCE)
set(SDLTTF_PLUTOSVG OFF CACHE INTERNAL "" FORCE)
add_subdirectory(vendor/SDL_ttf)

add_executable(${PROJECT_NAME} WIN32
  src/main.cpp
  src/InputManager.cpp
  src/MenuManager.cpp
  src/IniHandler.cpp
  src/Utils.cpp
  src/ScoresHandler.cpp
  src/FontHandler.cpp
  src/ImageHandler.cpp
  src/ui/UiButton.cpp
  src/ui/UiCheckbox.cpp
  src/ui/UiRectangle.cpp
  src/ui/UiText.cpp
  src/ui/UiImage.cpp
  src/menu/RaceMenu.cpp
  src/menu/TournamentMenu.cpp
  src/menu/SundayCup.cpp
  src/menu/TimeTrialMenu.cpp
  src/menu/Showroom.cpp
  src/menu/Garage.cpp
  src/menu/HallOfFame.cpp
  src/menu/PlayerMenu.cpp
  src/menu/Options.cpp
  src/menu/Settings.cpp
  src/menu/Controls.cpp
  src/menu/TrackInfo.cpp
  src/menu/League.cpp
  src/menu/Results.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static SDL3_ttf::SDL3_ttf-static)

set_target_properties(${PROJECT_NAME} PROPERTIES
  SUFFIX ".exe"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources)