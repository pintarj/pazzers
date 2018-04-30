#include <pazzers/game.hxx>
#include <algorithm>

namespace pazzers
{
    Game::Game(std::vector<const PazzerDescriptor*>&& pazzer_descriptors)
    {
        for (auto descriptor : pazzer_descriptors)
            players.emplace_back(new Pazzer(*descriptor));

        terrain = new Terrain();

        status_img = LoadImageSDL("res/arena/status.bmp");
        life_img = LoadImageSDL("res/arena/life.bmp");
        number = LoadImageSDL("res/arena/number.bmp");
        field = new resources::Image("res/arena/Field.bmp");
        pacman = LoadImageSDL("res/arena/pac.bmp");
        skyfall = LoadImageSDL("res/arena/skyfall.bmp");
        font1 = TTF_OpenFont("res/arena/Ubuntu-R.ttf", 16);
        font2 = TTF_OpenFont("res/arena/Ubuntu-R.ttf", 14);
    }

    Game::~Game()
    {
        delete terrain;

        for (auto player : players)
            delete player;

        delete field;

        SDL_FreeSurface(status_img);
        SDL_FreeSurface(life_img);
        SDL_FreeSurface(pacman);
        SDL_FreeSurface(number);
        SDL_FreeSurface(skyfall);
        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
    }

    bool Game::update_scene(float delta)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                for (auto player : players)
                {
                    if (player->dead == -1)
                        player->handle(event.key.keysym.sym, event.type);
                }
            }

            if (event.type == SDL_QUIT)
                return true;
        }
        SDL_FillRect(window->surface, &window->surface->clip_rect, 0x000000);

        for (auto player : players)
            player->status();

        window->apply(*field, 174, 0);
        terrain->cicle(players.data());

        std::sort(players.begin(), players.end(), [] (Pazzer* left, Pazzer* right) -> bool
        {
            return left->xy[1].y < right->xy[1].y;
        });

        for (auto player : players)
        {
            if (player->dead == -1)
            {
                player->move();
                player->show();
            }
            else if (player->dead != -2)
            {
                player->make_fun(SDL_GetTicks());
            }
        }

        return false;
    }

    void Game::draw_frame()
    {

    }
}
