#include <pazzers/garbage.hxx>
#include <time.h>
#include <SDL/SDL_image.h>
#include <pazzers/image.hxx>
#include <pazzers/terrain.hxx>

namespace pazzers
{
    struct _terrain area[19][17];
    int FPS = 30;
    int pro_pac = 0;
    int pro_block = 75;
    const int WIDTH = 1014;
    const int HEIGHT = 759;
    const int DEPTH = 32;
    const int button[4][5] = {
        SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RCTRL,
        SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_TAB,
        SDLK_i, SDLK_k, SDLK_j, SDLK_l, SDLK_SPACE,
        SDLK_KP_DIVIDE, SDLK_KP8, SDLK_KP7, SDLK_KP9, SDLK_KP0
    };
    const int initxy[4][2] = {214, 69, 934, 710, 934, 69, 214, 710};
    XY pacXY = {-1, -1};
    Image* window = nullptr;
    SDL_Joystick* stick = NULL;
    Image* field = nullptr;
    SDL_Surface* number = NULL;
    SDL_Surface* skyfall = NULL;
    SDL_Surface* status_img = NULL;
    SDL_Surface* pacman = NULL;
    SDL_Surface* life_img = NULL;
    SDL_Surface* text = NULL;
    SDL_Rect numbs[11][2];
    TTF_Font* font1, * font2;
    SDL_Event event;
    SDL_Color black = {0, 0, 0};
    SDL_Color white = {255, 255, 255};


    int sqr(int n, int x)
    {
        int i, j = n;

        if (!x) return 1;
        for (i = 1; i < x; i++) n *= j;
        return n;
    }


    void NegativeSDL(SDL_Surface* surface)
    {
        Uint32* pixels = (Uint32*) surface->pixels;
        int i;

        for (i = 0; i < (surface->w * surface->h); i++)
        {
            if (pixels[i] != SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF))
                pixels[i] = SDL_MapRGB(surface->format,
                                       0xff - (pixels[i] >> 16),
                                       0xff - pixels[i] % 256,
                                       0xff - (pixels[i] >> 8) % 256
                );
        }
    }


    SDL_Surface* LoadImageSDL(const char* filename)
    {
        SDL_Surface* loadedImage = NULL;
        SDL_Surface* optimizedImage = NULL;

        loadedImage = IMG_Load(filename);
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0xFF, 0x00, 0xFF));
        return optimizedImage;
    }


    void ApplySurfaceSDL(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
        SDL_Rect offset;

        offset.x = x;
        offset.y = y - 40;
        SDL_BlitSurface(source, clip, destination, &offset);
    }


    void show_num(int* x, int* y, const char* str, int t)
    {
        int i = 0;

        t = 1250 - (SDL_GetTicks() - t);
        if (t < 0) t = 0;
        else if (t > 1000) t = 1000;
        SDL_SetAlpha(number, SDL_SRCALPHA, t / 4);
        if (str[i] == '-')
        {
            ApplySurfaceSDL(*x + (i * 10), *y, number, window->surface, &numbs[10][1]);
            i++;
            while (str[i])
            {
                ApplySurfaceSDL(*x + (i * 10), *y, number, window->surface, &numbs[str[i] - '0'][1]);
                i++;
            }
        }
        else
        {
            if (str[i] == '+')
            {
                ApplySurfaceSDL(*x + (i * 10), *y, number, window->surface, &numbs[10][0]);
                i++;
            }
            while (str[i])
            {
                ApplySurfaceSDL(*x + (i * 10), *y, number, window->surface, &numbs[str[i] - '0'][0]);
                i++;
            }
        }
        *y = *y - 1;
    }
}
