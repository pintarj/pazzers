#include <pazzers/garbage.hxx>
#include <time.h>
#include <SDL/SDL_image.h>
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
        SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_KP0,
        SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_TAB,
        SDLK_i, SDLK_k, SDLK_j, SDLK_l, SDLK_SPACE,
        SDLK_KP_DIVIDE, SDLK_KP8, SDLK_KP7, SDLK_KP9, SDLK_KP0
    };
    const int initxy[4][2] = {214, 69, 934, 710, 934, 69, 214, 710};
    XY pacXY = {-1, -1};
    SDL_Joystick* stick = NULL;
    SDL_Surface* screen = NULL;
    SDL_Surface* field = NULL;
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


    void ApplySurfaceSDL(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
    {
        SDL_Rect offset;

        offset.x = x;
        offset.y = y - 40;
        SDL_BlitSurface(source, clip, destination, &offset);
    }


    void InitSDL(const char* title)
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        screen = SDL_SetVideoMode(0, 0, DEPTH, SDL_HWSURFACE | SDL_FULLSCREEN);
        SDL_WM_SetCaption(title, NULL);
        srand(time(NULL));
        TTF_Init();
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
            ApplySurfaceSDL(*x + (i * 10), *y, number, screen, &numbs[10][1]);
            i++;
            while (str[i])
            {
                ApplySurfaceSDL(*x + (i * 10), *y, number, screen, &numbs[str[i] - '0'][1]);
                i++;
            }
        }
        else
        {
            if (str[i] == '+')
            {
                ApplySurfaceSDL(*x + (i * 10), *y, number, screen, &numbs[10][0]);
                i++;
            }
            while (str[i])
            {
                ApplySurfaceSDL(*x + (i * 10), *y, number, screen, &numbs[str[i] - '0'][0]);
                i++;
            }
        }
        *y = *y - 1;
    }



    void no_shadow_on_my_head(int* tab, int size, Men* inf)
    {
        Uint8 i, j;
        int* min, to;

        for (i = 0; i < size; i++) tab[i] = inf[i].xy[1].y;
        for (i = 0; i < size; i++)
        {
            min = &tab[i];
            for (j = i; j < size; j++)
            {
                if (tab[j] < *min) min = &tab[j];
            }
            to = *min;
            *min = tab[i];
            tab[i] = to;
        }
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                if (inf[i].xy[1].y == tab[j])
                {
                    tab[j] = inf[i].id;
                    break;
                }
    }


    const char* bmp_name(int i)
    {
        switch (i)
        {
            case 0:
                return "men/men1.bmp";
            case 1:
                return "men/men2.bmp";
            case 2:
                return "men/men3.bmp";
            case 3:
                return "men/men4.bmp";
        }
    }

    void Game(int players)
    {
        Terrain nature;
        bool quit = false;
        Men men[players];
        int clock, turn[3];
        int joysticks = SDL_NumJoysticks();
        int joy_id[players];
        SDL_Joystick* stick[joysticks];
        Uint8 i, j = players;


        // Initialize..
        status_img = LoadImageSDL("arena/status.bmp");
        life_img = LoadImageSDL("arena/life.bmp");
        number = LoadImageSDL("arena/number.bmp");
        field = LoadImageSDL("arena/Field.bmp");
        pacman = LoadImageSDL("arena/pac.bmp");
        skyfall = LoadImageSDL("arena/skyfall.bmp");
        font1 = TTF_OpenFont("arena/Ubuntu-R.ttf", 16);
        font2 = TTF_OpenFont("arena/Ubuntu-R.ttf", 14);
        for (i = 0; i < players; i++) men[i].initialize(bmp_name(i), i);
        while (j-- > 0 && joysticks-- > 0)
        {
            men[j].alter = true;
            stick[j] = SDL_JoystickOpen(j - players + 1);
            joy_id[j - players + 1] = j;
        }
        //-----------


        // Game loop..
        while (quit == false)
        {
            clock = SDL_GetTicks();
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
                {
                    for (i = 0; i < players; i++)
                    {
                        if (men[i].dead == -1)
                        {
                            men[i].handle(event.key.keysym.sym, event.type);
                        }
                    }
                }
                if (event.type == SDL_JOYAXISMOTION)
                {
                    if (men[joy_id[event.jaxis.which]].dead == -1)
                        men[joy_id[event.jaxis.which]].handle(0, 0);
                }
                if (event.type == SDL_JOYBUTTONDOWN)
                {
                    if (men[joy_id[event.button.which]].dead == -1)
                        men[joy_id[event.button.which]].handle(0, 0);
                }
                if (event.type == SDL_QUIT) quit = true;
            }
            SDL_FillRect(screen, &screen->clip_rect, 0x000000);
            for (i = 0; i < players; i++) men[i].status();
            ApplySurfaceSDL(174, 39, field, screen, NULL);
            nature.cicle(men);
            no_shadow_on_my_head(turn, players, men);
            for (i = 0; i < players; i++)
            {
                if (men[turn[i]].dead == -1)
                {
                    men[turn[i]].move();
                    men[turn[i]].show();
                }
                else if (men[turn[i]].dead != -2)
                {
                    men[turn[i]].make_fun(SDL_GetTicks());
                }
            }
            SDL_Flip(screen);
            if ((clock = SDL_GetTicks() - clock) < 1000 / FPS)
            {
                SDL_Delay(1000 / FPS - clock);
            }
        }
        //----------


        // Killing..
        SDL_FreeSurface(field);
        SDL_FreeSurface(status_img);
        SDL_FreeSurface(life_img);
        SDL_FreeSurface(pacman);
        SDL_FreeSurface(number);
        SDL_FreeSurface(skyfall);
        nature.kill();
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
        for (j = players; j-- > 0 && joysticks-- > 0;)
        {
            SDL_JoystickClose(stick[j]);
        }
        for (i = 0; i < players; i++)
        {
            men[i].kill();
        }
        //---------

    }
}
