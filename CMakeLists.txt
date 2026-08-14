cmake_minimum_required(VERSION 3.20)

project(menu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_SHARED OFF CACHE INTERNAL "" FORCE)
set(SDL_STATIC ON CACHE INTERNAL "" FORCE)
set(SDL_DISABLE_INSTALL ON CACHE INTERNAL "" FORCE)
set(SDL_DISABLE_INSTALL_DOCS ON CACHE INTERNAL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE INTERNAL "" FORCE)
set(SDL_TESTS OFF CACHE INTERNAL "" FORCE)
set(SDL_EXAMPLES OFF CACHE INTERNAL "" FORCE)
add_subdirectory(vendor/SDL)

add_executable(${PROJECT_NAME} WIN32
  src/main.cpp
  src/InputManager.cpp
  src/MenuManager.cpp
  src/ui/UiButton.cpp
  src/ui/UiRectangle.cpp
  src/menu/RaceMenu.cpp
  src/menu/TournamentMenu.cpp
  src/menu/SundayCup.cpp
  src/menu/TimeTrialMenu.cpp
  src/menu/Options.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)