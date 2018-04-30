
#ifndef PAZZERS_GAME_HXX
#define PAZZERS_GAME_HXX

#include <vector>
#include <pazzers/pazzer.hxx>
#include <pazzers/terrain.hxx>

namespace pazzers
{
    /**
     * \brief Class that represents a single game.
     * */
    class Game
    {
    public:
        /**
         * \brief Create a new game.
         * \param pazzer_descriptors The pazzer descriptors.
         * */
        Game(std::vector<const PazzerDescriptor*>&& pazzer_descriptors);

        /**
         * \brief Release the used resources.
         * */
        virtual ~Game();

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
         * \brief The terrain of the game.
         * */
        Terrain* terrain;
    };
}

#endif
