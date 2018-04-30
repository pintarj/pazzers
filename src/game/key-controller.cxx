#include <pazzers/game/key-controller.hxx>
#include <SDL/SDL_keysym.h>

namespace pazzers
{
    namespace game
    {
        static const int maps[4][5] =
            {
                {SDLK_UP,        SDLK_RIGHT, SDLK_DOWN, SDLK_LEFT, SDLK_RCTRL},
                {SDLK_w,         SDLK_d,     SDLK_s,    SDLK_a,    SDLK_TAB},
                {SDLK_i,         SDLK_l,     SDLK_k,    SDLK_j,    SDLK_SPACE},
                {SDLK_KP_DIVIDE, SDLK_KP9,   SDLK_KP8,  SDLK_KP7,  SDLK_KP0}
            };

        KeyController::KeyController(KeyControllerMap map):
            map(map)
        {

        }

        void KeyController::on_keydown(int key)
        {
            const int* m = maps[(int) map];

            if (key == m[0])
                set_direction(geometry::Direction::UP);
            else if (key == m[1])
                set_direction(geometry::Direction::RIGHT);
            else if (key == m[2])
                set_direction(geometry::Direction::DOWN);
            else if (key == m[3])
                set_direction(geometry::Direction::LEFT);
            else if (key == m[4])
                require_bomb_drop();
        }

        void KeyController::on_keyup(int key)
        {
            const int* m = maps[(int) map];

            const auto reset_direction = [this] (geometry::Direction released)
                {
                    if (released != this->get_direction())
                        return;

                    this->set_direction(geometry::Direction::NONE);
                };

            if (key == m[0])
                reset_direction(geometry::Direction::UP);
            else if (key == m[1])
                reset_direction(geometry::Direction::RIGHT);
            else if (key == m[2])
                reset_direction(geometry::Direction::DOWN);
            else if (key == m[3])
                reset_direction(geometry::Direction::LEFT);
        }
    }
}
