
#ifndef PAZZERS_GARBAGE_HXX
#define PAZZERS_GARBAGE_HXX

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <pazzers/pazzer.hxx>
#include <pazzers/geometry/xy.hxx>
#include <pazzers/resources/image.hxx>

#define FX1 214
#define FX2 974
#define FY1 79
#define FY2 760
#define _FREE 0
#define BOMB 1
#define EXPL 2
#define _WALL 3
#define DESTROY 4
#define BS_BOMB 5
#define BS_FIRE 6
#define BS_ROLL 7
#define BS_PAC 8
#define BS_ATK 9
#define BS_DEF 10
#define BS_SKY 11
#define SKY 12

namespace pazzers
{
    struct _terrain
    {
        int type;
        int time;
        int power;
        int img;
        int owner;
        int atk;
    };

    extern struct _terrain area[19][17];
    extern int FPS;
    extern int pro_pac;
    extern int pro_block;
    extern const int initxy[4][2];
    extern XY pacXY;
    extern resources::Image* window;
    extern resources::Image* field;
    extern resources::Image* number;
    extern resources::Image* skyfall;
    extern resources::Image* status_img;
    extern resources::Image* pacman;
    extern resources::Image* life_img;
    extern geometry::Rectangle* numbs[2][11];
    extern geometry::Rectangle* skyfall_clip[5];
    extern TTF_Font* font1, * font2;
}

#endif
