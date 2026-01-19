set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(src main.cpp)

add_subdirectory(core)
add_subdirectory(renderer)
add_subdirectory(graphics)
add_subdirectory(utils)
add_subdirectory(game)
add_subdirectory(platform)

target_link_libraries(src
    PRIVATE
    core
    renderer
    graphics
    utils
    game
    platform
    SDL2::SDL2 
    glad::glad 
    glm::glm
)

install(TARGETS src)
