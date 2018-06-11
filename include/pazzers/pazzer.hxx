
#ifndef PAZZERS_GAME_PAZZER_HXX
#define PAZZERS_GAME_PAZZER_HXX

namespace pazzers
{
    class Game;
    class Pazzer;
}

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <pazzers/system.hxx>
#include <pazzers/flow/key-listener.hxx>
#include <pazzers/game/virtual-controller.hxx>
#include <pazzers/geometry/xy.hxx>
#include <pazzers/resources/image.hxx>

namespace pazzers
{
    /**
     * \brief Describes a pazzer.
     * */
    class PazzerDescriptor
    {
    public:
        /**
         * \brief The pazzer's name.
         * */
        const std::string name;

        /**
         * \brief The path to the image that represents the pazzer.
         * */
        const std::string image_path;

        /**
         * \brief Create a new pazzer descriptor.
         * \param name The pazzer's name.
         * \param image_path The path to the image that represents the pazzer.
         * */
        PazzerDescriptor(const std::string& name, const std::string& image_path);

        /**
         * \brief Returns a vector of all available pazzer descriptors.
         * \return The vector of all available pazzer descriptors.
         * */
        static const std::vector<const PazzerDescriptor*>& get_all();
    };

    /**
     * \brief A class that represent a single player in a game.
     * */
    class Pazzer
    {
        friend Game;

    public:
        /**
         * \brief The unique id of the player in the game.
         * */
        const int id;

        /**
         * \brief The pazzer descriptor.
         * */
        const PazzerDescriptor& descriptor;

        /**
         * \brief Create a new pazzer.
         * \param descriptor The pazzer's descriptor.
         * */
        Pazzer(const PazzerDescriptor& descriptor, game::VirtualController* controller);

        /**
         * \brief Release the used resources.
         * */
        ~Pazzer();

        /**
         * \brief Update the pazzer.
         * \param delta The amount of time that is going to be updated, in seconds.
         * */
        void update(float delta);

        /**
         * \brief Draw the pazzer taking in condition the field's origin.
         * \param field_origin The field's origin.
         * */
        void draw(const XY& field_origin);

        /**
         * \brief Tells if the player is still in game or not.
         * \return True if it's in game, false otherwise.
         * */
        bool is_out_of_game() const noexcept;

    private:
        /**
         * \brief The image that represent the pazzer.
         * */
        const resources::Image& image;

        /**
         * \brief The virtual controller that controls the pazzers.
         */
        game::VirtualController* controller;

        /**
         * \brief Tells the direction of the pazzer.
         * */
        geometry::Direction direction;

        /**
         * \brief Tells if the pazzers is moving.
         * */
        bool in_movement;

        /**
         * \brief The pazzers current x position on the field.
         * */
        float field_x;

        /**
         * \brief The pazzers current y position on the field.
         * */
        float field_y;

        /**
         * \brief Stores the amount of distance covered since last clip change.
         * */
        float clipping_accumulator;

        /**
         * \brief The player's life points.
         * */
        int life_points;

        /**
         * \brief Applies the movement using the direction.
         * \param delta The amount of time that is going to be updated, in seconds.
         * */
        void apply_movement(float delta);

        /**
         * \brief Return the index of the clip that have to be used for the current step
         *     of the movement.
         * \return The second index of the clip in the "pazzer_views[][]" array.
         * */
        int get_clip_index();
    };
}

#endif
