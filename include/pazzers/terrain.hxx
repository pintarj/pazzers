
#ifndef PAZZERS_TERRAIN_HXX
#define PAZZERS_TERRAIN_HXX

#include <SDL/SDL.h>
#include <pazzers/pazzer.hxx>

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
     * \brief A class that represent a game terrain for a single game.
     * */
    class Terrain
    {
    private:
        SDL_Rect expl_clip[7][4];
        SDL_Rect bomb_clip[4];
        SDL_Rect wall_clip[3];
        SDL_Rect bonus_clip[7][2];
        SDL_Surface *bonus;
        SDL_Surface *wall;
        SDL_Surface *bomb;
        SDL_Surface *expl;

    public:
        /**
         * \brief Create the terrain.
         * */
        Terrain();

        /**
         * \brief Release the used resources.
         * */
        ~Terrain();

        void cicle(Pazzer** ptr);
        int chain(int x, int y, int pwr);
        int which_one(int phase);
        int which_one2(int phase);
        int which_one3(int phase);
    };
}

#endif
