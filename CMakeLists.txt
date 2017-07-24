CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(pazzers CXX)

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(pazzers
        src/garbage.cxx
        src/image.cxx
        src/men.cxx
        src/pazzers.cxx
        src/terrain.cxx
)
SET_PROPERTY(TARGET pazzers PROPERTY CXX_STANDARD 14)
TARGET_LINK_LIBRARIES(pazzers SDL SDLmain SDL_image SDL_ttf)
