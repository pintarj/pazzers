
#ifndef PAZZERS_MEN_HXX
#define PAZZERS_MEN_HXX

namespace pazzers
{
    class Men;
}

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
    class Pazzer {
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
            XY xy[4];
            bool alter;
            int id, mun, life, dead, mun_max, atk, def, dmg;
            void make_fun(int phase);
            void status();
            void initialize(const char *name, int id_);
            void show();
            void kill() { SDL_FreeSurface(obj); SDL_FreeSurface(sht); }
            int alive(Uint8 i);
            void handle(int msg, int type);
            void move();
            void check();
    };
}

#endif
