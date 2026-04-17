set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(dryout WIN32 MACOSX_BUNDLE main.cpp)

set_target_properties(dryout PROPERTIES
    WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)

add_subdirectory(core)
add_subdirectory(renderer)
add_subdirectory(graphics)
add_subdirectory(utils)
add_subdirectory(game)

target_link_libraries(dryout
    PRIVATE
    core
    renderer
    graphics
    utils
    game
    $<IF:$<TARGET_EXISTS:SDL2::SDL2main>,SDL2::SDL2main,>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    glad::glad
    glm::glm
)

install(TARGETS dryout)
