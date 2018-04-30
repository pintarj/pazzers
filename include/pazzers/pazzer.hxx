
#ifndef PAZZERS_MEN_HXX
#define PAZZERS_MEN_HXX

namespace pazzers
{
    class Pazzer;
}

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <pazzers/garbage.hxx>
#include <pazzers/xy.hxx>

#define UP 1
#define DOWN 0
#define LEFT 3
#define RIGHT 2
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

    class Pazzer
    {
    private:
        SDL_Surface *obj, *sht;
        SDL_Rect obj_clip[6][3];
        SDL_Rect life_clip;
        Uint8 dir;
        Uint8 count;
        int up, down, left, right, drop;
        XY joy;
        bool mov;
        int power, speed, time, pac;
        int cheat[10];
        struct _message {
            XY xy;
            int time;
            char text[10];
        } message;

    public:
        /**
         * \brief The pazzer descriptor.
         * */
        const PazzerDescriptor& descriptor;

        /**
         * \brief Create a new pazzer.
         * \param descriptor The pazzer's descriptor.
         * */
        Pazzer(const PazzerDescriptor& descriptor);

        /**
         * \brief Release the used resources.
         * */
        ~Pazzer();

        XY xy[4];
        bool alter;
        int id, mun, life, dead, mun_max, atk, def, dmg;
        void make_fun(int phase);
        void status();
        void show();
        int alive(Uint8 i);
        void handle(int msg, int type);
        void move();
        void check();
    };
}

#endif
