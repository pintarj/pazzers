#include <pazzers/game.hxx>
#include <pazzers/game/key-controller.hxx>
#include <pazzers/resources/cache.hxx>
#include <pazzers/system.hxx>

using namespace pazzers;

int main(int argc, char* argv[])
{
    if (!system::initialize())
        return 1;

    game::KeyController* controllers[4] =
        {
            new game::KeyController(game::KeyControllerMap::ARROWS),
            new game::KeyController(game::KeyControllerMap::WASD),
            new game::KeyController(game::KeyControllerMap::IJKL),
            new game::KeyController(game::KeyControllerMap::NUMPAD)
        };

    auto descriptors = PazzerDescriptor::get_all();
    std::vector<Pazzer*> players;

    for (int i = 0; i < 4; ++i)
        players.emplace_back(new Pazzer(*descriptors.at((unsigned) i), controllers[i]));

    resources::CellDescriptor cd("res/image/field/free.bmp", true, true);
    resources::FieldDescriptor fieldDescriptor(4, 4, std::vector<const resources::CellDescriptor*>(16, &cd));
    auto game = new Game(std::move(players), fieldDescriptor);

    for (auto& controller : controllers)
        game->listeners.insert(controller);

    bool         quit       = false;
    const int    target_fps = 60;
    const Uint32 delta_ms   = 1000 / target_fps;
    float        delta      = 1.0f / (float) target_fps;
    Uint32       timestamp  = SDL_GetTicks();

    while (true)
    {
        static SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    game->on_keydown(event.key.keysym.sym);
                    break;

                case SDL_KEYUP:
                {
                    int key = event.key.keysym.sym;

                    switch (key)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        default:
                            game->on_keyup(key);
                            break;
                    }

                    break;
                }

                default:
                    break;
            }
        }

        if (quit)
            break;

        quit = game->update_scene(delta);

        if (quit)
            break;

        game->draw_frame();
        SDL_Flip(system::window->surface);

        auto new_timestamp = SDL_GetTicks();
        auto length = new_timestamp - timestamp;

        if (length < delta_ms)
            SDL_Delay(delta_ms - length);

        timestamp = new_timestamp;
        delta = length / 1000.0f;
    }

    for (auto& controller : controllers)
        delete controller;

    delete game;
    resources::cache::free_all();
    system::destroy();
    return 0;
}
  
