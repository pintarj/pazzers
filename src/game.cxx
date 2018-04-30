#include <pazzers/game.hxx>
#include <algorithm>

namespace pazzers
{
    Game::Game(std::vector<const PazzerDescriptor*>&& pazzer_descriptors)
    {
        for (auto descriptor : pazzer_descriptors)
            players.emplace_back(new Pazzer(*descriptor));

        status_img = new resources::Image("res/arena/status.bmp");
        life_img = new resources::Image("res/arena/life.bmp");
        number = new resources::Image("res/arena/number.bmp");
        field = new resources::Image("res/arena/Field.bmp");
        pacman = new resources::Image("res/arena/pac.bmp");
        skyfall = new resources::Image("res/arena/skyfall.bmp");
        font1 = TTF_OpenFont("res/arena/Ubuntu-R.ttf", 16);
        font2 = TTF_OpenFont("res/arena/Ubuntu-R.ttf", 14);

        terrain = new Terrain();
    }

    Game::~Game()
    {
        delete terrain;

        for (auto player : players)
            delete player;

        delete skyfall;
        delete pacman;
        delete field;
        delete number;
        delete life_img;
        delete status_img;

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
