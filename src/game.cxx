#include <pazzers/game.hxx>
#include <algorithm>

namespace pazzers
{
    Game::Game(std::vector<Pazzer*>&& players):
        players(players),
        field(new game::Field())
    {

    }

    Game::~Game()
    {
        for (auto player : players)
            delete player;

        delete field;
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

        std::sort(players.begin(), players.end(), [] (Pazzer* left, Pazzer* right) -> bool
            {
                return left->field_y < right->field_y;
            });

        return false;
    }

    void Game::draw_frame()
    {
        const XY field_origin =
            {
                (system::window->get_width() - field->width * PAZZERS_GAME_CELL_SIZE) / 2,
                (system::window->get_height() - field->height * PAZZERS_GAME_CELL_SIZE) / 2
            };

        SDL_FillRect(system::window->surface, &system::window->surface->clip_rect, 0x000000);
        field->draw(field_origin);

        for (auto& player : players)
            player->draw(field_origin);
    }
}
