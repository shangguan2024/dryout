add_library(game
    STATIC
    Game.cpp
    GameMap.cpp
    Tile.cpp
    Player.cpp
)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(game
    PUBLIC
    core
    renderer
    graphics
    utils
)
