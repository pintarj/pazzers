#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pazzers/pazzer.hxx>
#include <pazzers/terrain.hxx>

int main(int argc, char* argv[])
{
    pazzers::InitSDL("Pazzers");
    pazzers::Game(argc>1 ? atoi(argv[1]) : 4);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
  
