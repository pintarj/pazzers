
#ifndef PAZZERS_GARBAGE_HXX
#define PAZZERS_GARBAGE_HXX

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <pazzers/image.hxx>
#include <pazzers/pazzer.hxx>
#include <pazzers/xy.hxx>

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
    extern const int WIDTH;
    extern const int HEIGHT;
    extern const int DEPTH;
    extern const int button[4][5];
    extern const int initxy[4][2];
    extern XY pacXY;
    extern SDL_Joystick* stick;
    extern Image* window;
    extern Image* field;
    extern SDL_Surface* number;
    extern SDL_Surface* skyfall;
    extern SDL_Surface* status_img;
    extern SDL_Surface* pacman;
    extern SDL_Surface* life_img;
    extern SDL_Surface* text;
    extern SDL_Rect numbs[11][2];
    extern TTF_Font* font1, * font2;
    extern SDL_Event event;
    extern SDL_Color black;
    extern SDL_Color white;

    int sqr(int n, int x);
    void NegativeSDL(SDL_Surface* surface);
    SDL_Surface* LoadImageSDL(const char* filename);
    void ApplySurfaceSDL(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
    void InitSDL(const char* title);
    void show_num(int* x, int* y, const char* str, int t);
    void no_shadow_on_my_head(int* tab, int size, Men* inf);
    const char* bmp_name(int i);
    void Game(int players);
}

#endif
