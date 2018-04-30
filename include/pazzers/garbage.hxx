
#ifndef PAZZERS_GARBAGE_HXX
#define PAZZERS_GARBAGE_HXX

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <pazzers/pazzer.hxx>
#include <pazzers/xy.hxx>
#include <pazzers/resources/image.hxx>

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
    extern const int button[4][5];
    extern const int initxy[4][2];
    extern XY pacXY;
    extern resources::Image* window;
    extern resources::Image* field;
    extern resources::Image* number;
    extern resources::Image* skyfall;
    extern resources::Image* status_img;
    extern resources::Image* pacman;
    extern resources::Image* life_img;
    extern resources::ImageView* numbs[2][11];
    extern resources::ImageView* skyfall_clip[5];
    extern TTF_Font* font1, * font2;
    extern SDL_Event event;
}

#endif
