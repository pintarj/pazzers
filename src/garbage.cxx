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
    const int initxy[4][2] = {214, 69, 934, 710, 934, 69, 214, 710};
    XY pacXY = {-1, -1};
    resources::Image* window = nullptr;
    resources::Image* field = nullptr;
    resources::Image* number = nullptr;
    resources::Image* skyfall = nullptr;
    resources::Image* status_img = nullptr;
    resources::Image* pacman = nullptr;
    resources::Image* life_img = nullptr;
    geometry::Rectangle* numbs[2][11];
    geometry::Rectangle* skyfall_clip[5];
    TTF_Font* font1, * font2;
}
