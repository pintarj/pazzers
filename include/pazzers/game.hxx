
#ifndef PAZZERS_GAME_GAME_HXX
#define PAZZERS_GAME_GAME_HXX

#include <vector>
#include <pazzers/pazzer.hxx>
#include <pazzers/flow/key-listener.hxx>
#include <pazzers/game/field.hxx>
#include <pazzers/geometry/xy.hxx>

namespace pazzers
{
    /**
     * \brief Class that represents a single game.
     * */
    class Game: public flow::KeyListener
    {
    public:
        /**
         * \brief Create a new game.
         * \param pazzers The vector of players.
         * */
        Game(std::vector<Pazzer*>&& players);

        /**
         * \brief Release the used resources.
         * */
        virtual ~Game();

        /**
         * \brief Propagate a "keydown" event.
         * \param key The key.
         * */
        virtual void on_keydown(int key) override;

        /**
         * \brief Propagate a "keyup" event.
         * \param key The key.
         * */
        virtual void on_keyup(int key) override;

        /**
         * \brief Update the scene.
         * \param delta The amount of time that is going to be updated, in seconds.
         * \return True if the game is finished, false otherwise.
         * */
        bool update_scene(float delta);

        /**
         * \brief Draw a single frame.
         * */
        void draw_frame();

    private:
        /**
         * \brief The vector of playing pazzers.
         * */
        std::vector<Pazzer*> players;

        /**
         * \brief The field of the game.
         * */
        game::Field* field;
    };
}

#endif
