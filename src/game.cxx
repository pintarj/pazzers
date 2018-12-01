#include <pazzers/game.hxx>
#include <algorithm>

namespace pazzers
{
    Game::Game(std::vector<Pazzer*>&& players):
        players(std::move(players))
    {
        status_img = new resources::Image("res/arena/status.bmp");
        life_img = new resources::Image("res/arena/life.bmp");
        number = new resources::Image("res/arena/number.bmp");
        _field = new resources::Image("res/arena/Field.bmp");
        pacman = new resources::Image("res/arena/pac.bmp");
        skyfall = new resources::Image("res/arena/skyfall.bmp");
        font1 = TTF_OpenFont("res/arena/Ubuntu-R.ttf", 16);
        font2 = TTF_OpenFont("res/arena/Ubuntu-R.ttf", 14);

        terrain = new Terrain();
        field = new game::Field({100, 100}, this->players);
    }

    Game::~Game()
    {
        delete field;
        delete terrain;

        for (auto player : players)
            delete player;

        delete skyfall;
        delete pacman;
        delete _field;
        delete number;
        delete life_img;
        delete status_img;

        TTF_CloseFont(font1);
        TTF_CloseFont(font2);
    }

    void Game::on_keydown(int key)
    {
        flow::KeyListener::on_keydown(key);
    }

    void Game::on_keyup(int key)
    {
        flow::KeyListener::on_keyup(key);
    }

    bool Game::update_scene(float delta)
    {
        for (auto& player : players)
            player->update(delta);

        /*for (auto player : players)
            player->status();*/
/*
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
        }*/

        return false;
    }

    void Game::draw_frame()
    {
        window->clear();
        field->draw();

        for (auto player : players)
            player->draw();
    }
}
