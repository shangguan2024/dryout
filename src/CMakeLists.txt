set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(dryout main.cpp)

add_subdirectory(core)
add_subdirectory(renderer)
add_subdirectory(graphics)
add_subdirectory(utils)
add_subdirectory(game)
add_subdirectory(platform)

target_link_libraries(dryout
    PRIVATE
    core
    renderer
    graphics
    utils
    game
    platform
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    glad::glad
    glm::glm
)

install(TARGETS dryout)
