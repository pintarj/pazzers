#include <pazzers/garbage.hxx>
#include <time.h>
#include <SDL/SDL_image.h>
#include <pazzers/resources/image.hxx>
#include <pazzers/terrain.hxx>

namespace pazzers
{
    struct _terrain area[19][17];
    int FPS = 35;
    int pro_pac = 0;
    int pro_block = 75;
    const int button[4][5] = {
        SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RCTRL,
        SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_TAB,
        SDLK_i, SDLK_k, SDLK_j, SDLK_l, SDLK_SPACE,
        SDLK_KP_DIVIDE, SDLK_KP8, SDLK_KP7, SDLK_KP9, SDLK_KP0
    };
    const int initxy[4][2] = {214, 69, 934, 710, 934, 69, 214, 710};
    XY pacXY = {-1, -1};
    resources::Image* window = nullptr;
    resources::Image* field = nullptr;
    resources::Image* number = nullptr;
    resources::Image* skyfall = nullptr;
    resources::Image* status_img = nullptr;
    resources::Image* pacman = nullptr;
    resources::Image* life_img = nullptr;
    resources::ImageView* numbs[2][11];
    resources::ImageView* skyfall_clip[5];
    TTF_Font* font1, * font2;
    SDL_Event event;
}
