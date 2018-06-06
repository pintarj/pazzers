#include <pazzers/system.hxx>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

namespace pazzers
{

    namespace system
    {
        resources::Image* window = nullptr;

        bool initialize()
        {
            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                printf("error: could not initialize SDL");
                return false;
            }

            if (TTF_Init() < 0)
            {
                printf("error: could not initialize TTF: %s\n", TTF_GetError());
                return false;
            }

            window = new resources::Image(SDL_SetVideoMode(1200, 800, 32, SDL_HWSURFACE));

            if (window == nullptr)
            {
                printf("error: could not create SDL window\n");
                return false;
            }

            SDL_WM_SetCaption("Pazzers", nullptr);
            srand(SDL_GetTicks());
            return true;
        }

        void destroy()
        {
            delete window;
            TTF_Quit();
            SDL_Quit();
        }
    }
}
