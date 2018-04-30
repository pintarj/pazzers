
#ifndef PAZZERS_GAME_PAZZER_HXX
#define PAZZERS_GAME_PAZZER_HXX

namespace pazzers
{
    class Pazzer;
}

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <pazzers/garbage.hxx>
#include <pazzers/flow/key-listener.hxx>
#include <pazzers/game/virtual-controller.hxx>
#include <pazzers/geometry/xy.hxx>
#include <pazzers/resources/image.hxx>

#define FX1 214
#define FX2 974
#define FY1 79
#define FY2 760
#define FREE 0
#define BOMB 1
#define EXPL 2
#define WALL 3
#define DESTROY 4
#define BS_BOMB 5
#define BS_FIRE 6
#define BS_ROLL 7
#define BS_PAC 8
#define BS_ATK 9
#define BS_DEF 10
#define BS_SKY 11
#define SKY 12

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

        XY xy[4];
        int mun, life, dead, mun_max, atk, def, dmg;
        void make_fun(int phase);
        void status();
        void show();
        int alive(Uint8 i);
        void move();
        void check();

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

        Uint8 count;
        int power, speed, time, pac;
        struct _message {
            XY xy;
            int time;
            char text[10];
        } message;
    };
}

#endif
