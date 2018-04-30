CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(pazzers CXX)

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(pazzers
    src/game.cxx
    src/garbage.cxx
    src/pazzer.cxx
    src/pazzers.cxx
    src/terrain.cxx
    src/flow/key-listener.cxx
    src/game/key-controller.cxx
    src/game/virtual-controller.cxx
    src/geometry/rectangle.cxx
    src/resources/cache.cxx
    src/resources/image.cxx
)

SET_PROPERTY(TARGET pazzers PROPERTY CXX_STANDARD 14)
TARGET_LINK_LIBRARIES(pazzers SDL SDLmain SDL_image SDL_ttf)
