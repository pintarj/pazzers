#include <utility>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <pazzers/game.hxx>

using namespace pazzers;

static bool initialize()
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

    window = new Image(SDL_SetVideoMode(1200, 800, DEPTH, SDL_HWSURFACE));

    if (window == nullptr)
    {
        printf("error: could not create SDL window\n");
        return false;
    }

    SDL_WM_SetCaption("Pazzers", nullptr);
    srand(SDL_GetTicks());
    return true;
}

static void destroy()
{
    delete window;
    TTF_Quit();
    SDL_Quit();
}

static std::vector<const PazzerDescriptor*> load_pazzers_descriptors()
{
    std::vector<const PazzerDescriptor*> pazzer_descriptors;
    auto all = PazzerDescriptor::get_all();
    pazzer_descriptors.emplace_back(all.at(0));
    pazzer_descriptors.emplace_back(all.at(1));
    pazzer_descriptors.emplace_back(all.at(2));
    pazzer_descriptors.emplace_back(all.at(3));
    return std::move(pazzer_descriptors);
}

int main(int argc, char* argv[])
{
    if (!initialize())
        return 1;

    auto         game       = new Game(load_pazzers_descriptors());
    bool         quit       = false;
    const int    target_fps = 60;
    const Uint32 delta_ms   = 1000 / target_fps;
    float        delta      = 1.0f / (float) target_fps;
    Uint32       timestamp  = SDL_GetTicks();

    while (true)
    {
        quit = game->update_scene(delta);

        if (quit)
            break;

        game->draw_frame();
        SDL_Flip(window->surface);

        auto new_timestamp = SDL_GetTicks();
        auto length = new_timestamp - timestamp;

        if (length < delta_ms)
            SDL_Delay(delta_ms - length);

        timestamp = new_timestamp;
    }

    delete game;
    destroy();
    return 0;
}
  
