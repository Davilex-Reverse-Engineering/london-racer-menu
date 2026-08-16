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
  src/IniHandler.cpp
  src/Utils.cpp
  src/ui/UiButton.cpp
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
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)

set_target_properties(${PROJECT_NAME} PROPERTIES
  SUFFIX ".exe"
)