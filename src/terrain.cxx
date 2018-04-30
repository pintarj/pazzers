#include <pazzers/terrain.hxx>
#include <SDL/SDL_image.h>

namespace pazzers
{
    static SDL_Surface* LoadImageSDL(const char* filename)
    {
        SDL_Surface* loadedImage = NULL;
        SDL_Surface* optimizedImage = NULL;

        loadedImage = IMG_Load(filename);
        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);
        SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0xFF, 0x00, 0xFF));
        return optimizedImage;
    }

    static void ApplySurfaceSDL(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
        SDL_Rect offset;

        offset.x = x;
        offset.y = y - 40;
        SDL_BlitSurface(source, clip, destination, &offset);
    }

    Terrain::Terrain()
    {
        wall=LoadImageSDL("res/arena/wall.bmp");
        bomb=LoadImageSDL("res/arena/bomb.bmp");
        expl=LoadImageSDL("res/arena/expl.bmp");
        bonus=LoadImageSDL("res/arena/bonus.bmp");
        for (int i=0;i<7;i++) {
            for (int j=0;j<4;j++) {
                expl_clip[i][j].x=i*40;
                expl_clip[i][j].y=j*40;
                expl_clip[i][j].w=40;
                expl_clip[i][j].h=40;
            }
        }
        for (int i=0;i<4;i++) {
            bomb_clip[i].x=i*40;
            bomb_clip[i].y=0;
            bomb_clip[i].w=40;
            bomb_clip[i].h=40;
        }
        for (int i=0;i<3;i++) {
            wall_clip[i].x=i*40;
            wall_clip[i].y=0;
            wall_clip[i].w=40;
            wall_clip[i].h=40;
        }
        for (int i=0;i<7;i++) {
            for (int j=0;j<2;j++) {
                bonus_clip[i][j].x=i*40;
                bonus_clip[i][j].y=j*40;
                bonus_clip[i][j].w=40;
                bonus_clip[i][j].h=40;
            }
        }

        for (int i=0;i<19;i++) {
            for (int j=0;j<17;j++) {
                if ((!(j%2==1 && i%2==1)) && (rand()%100<pro_block)){
                    area[i][j].type=WALL;
                }
            }
        }

        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 11; ++j)
                numbs[i][j] = new geometry::Rectangle(j * 10, i * 20, 10, 20);

        for (int i = 0; i < 5; ++i)
            skyfall_clip[i] = new geometry::Rectangle(i * 40, 0, 40, 40);

        area[0][0].type=FREE;
        area[0][1].type=FREE;
        area[1][0].type=FREE;
        area[0][15].type=FREE;
        area[1][16].type=FREE;
        area[0][16].type=FREE;
        area[18][0].type=FREE;
        area[18][1].type=FREE;
        area[17][0].type=FREE;
        area[18][15].type=FREE;
        area[18][16].type=FREE;
        area[17][16].type=FREE;
    }

    Terrain::~Terrain()
    {
        SDL_FreeSurface(bomb);
        SDL_FreeSurface(expl);
        SDL_FreeSurface(wall);
        SDL_FreeSurface(bonus);

        for (int y = 0; y < 2; y++)
            for (int x = 0; x < 11; x++)
                delete numbs[y][x];

        for (int i = 0; i < 5; ++i)
            delete skyfall_clip[i];
    }

    int Terrain::which_one(int phase) {
        if (phase<70) return 0;
        else if (phase<140) return 2;
        else if (phase<210) return 1;
        else if (phase<280) return 2;
        else if (phase<350) return 0;
        else return 3;
    }

    int Terrain::which_one2(int phase) {
        if (phase<2000) return ((phase/150)%2);
        else if (phase<3000) return ((phase/125)%2+1);
        else return ((phase/50)%2+2);
    }

    int Terrain::which_one3(int phase) {
        if(phase<200)
            return 1;
        else
            return 2;
    }

    int Terrain::chain(int x, int y, int pwr) {
        Uint8 i, j, _pwr, result=0;

        for (i=0;i<4;i++){
            switch (i%2) {
                case 0: j=y; break;
                case 1: j=x; break;
            }
            _pwr=pwr;
            while ( (_pwr-->0)  && (!(i==2 && j==16)) && (!(i==1 && j==18)) && (!((i%2==0 && x%2==1)||(i%2==1 && y%2==1))) && ((!(i==3 && j==0)) && (!(i==0 && j==0))) ) {
                switch (i) {
                    case 0 : {
                        j--;
                        if (area[x][j].type==BOMB && area[x][j].time>=SDL_GetTicks()-4000) {
                            area[x][j].time=SDL_GetTicks()-4000;
                            result=1;
                            _pwr=0;
                        } else if (area[x][j].type==FREE) {
                            area[x][j].type=EXPL;
                            area[x][j].time=area[x][y].time;
                            area[x][j].atk=area[x][y].atk;
                            if ( _pwr==0 )
                                area[x][j].img=5;
                            else
                                area[x][j].img=3;
                        } else if ((area[x][j].type==EXPL) && (area[x][j].img==5 && _pwr>0)) {
                            area[x][j].img=3;
                        } else if (area[x][j].type==WALL ) {
                            area[x][j].type=DESTROY;
                            area[x][j].time=SDL_GetTicks();
                            _pwr=0;
                        }	else if (area[x][j].type==DESTROY) {
                            _pwr=0;
                        }
                        break;
                    }
                    case 1 : {
                        j++;
                        if (area[j][y].type==BOMB && area[j][y].time>=SDL_GetTicks()-4000) {
                            area[j][y].time=SDL_GetTicks()-4000;
                            result=1;
                            _pwr=0;
                        } else if (area[j][y].type==FREE) {
                            area[j][y].type=EXPL;
                            area[j][y].time=area[x][y].time;
                            area[j][y].atk=area[x][y].atk;
                            if ( _pwr==0 )
                                area[j][y].img=2;
                            else
                                area[j][y].img=1;
                        } else if ((area[j][y].type==EXPL) && (area[j][y].img==2 && _pwr>0)) {
                            area[j][y].img=1;
                        } else if (area[j][y].type==WALL ) {
                            area[j][y].type=DESTROY;
                            area[j][y].time=SDL_GetTicks();
                            _pwr=0;
                        }	else if (area[j][y].type==DESTROY) {
                            _pwr=0;
                        }
                        break;
                    }
                    case 2 : {
                        j++;
                        if (area[x][j].type==BOMB && area[x][j].time>=SDL_GetTicks()-4000) {
                            area[x][j].time=SDL_GetTicks()-4000;
                            result=1;
                            _pwr=0;
                        } else if (area[x][j].type==FREE) {
                            area[x][j].type=EXPL;
                            area[x][j].time=area[x][y].time;
                            area[x][j].atk=area[x][y].atk;
                            if ( _pwr==0 )
                                area[x][j].img=4;
                            else
                                area[x][j].img=3;
                        } else if ((area[x][j].type==EXPL) && (area[x][j].img==4 && _pwr>0)) {
                            area[x][j].img=3;
                        } else if (area[x][j].type==WALL ) {
                            area[x][j].type=DESTROY;
                            area[x][j].time=SDL_GetTicks();
                            _pwr=0;
                        }	else if (area[x][j].type==DESTROY) {
                            _pwr=0;
                        }
                        break;
                    }
                    case 3 : {
                        j--;
                        if (area[j][y].type==BOMB && area[j][y].time>=SDL_GetTicks()-4000) {
                            area[j][y].time=SDL_GetTicks()-4000;
                            result=1;
                            _pwr=0;
                        } else if (area[j][y].type==FREE) {
                            area[j][y].type=EXPL;
                            area[j][y].time=area[x][y].time;
                            area[j][y].atk=area[x][y].atk;
                            if ( _pwr==0 )
                                area[j][y].img=6;
                            else
                                area[j][y].img=1;
                        } else if ((area[j][y].type==EXPL) && (area[j][y].img==6 && _pwr>0)) {
                            area[j][y].img=1;
                        } else if (area[j][y].type==WALL ) {
                            area[j][y].type=DESTROY;
                            area[j][y].time=SDL_GetTicks();
                            _pwr=0;
                        }	else if (area[j][y].type==DESTROY) {
                            _pwr=0;
                        }
                        break;
                    }
                }
            }
        }
        return result;
    }

    void Terrain::cicle(Pazzer** ptr) {
        Uint8 i, j, bns;

        for (i=0;i<19;i++) {
            for (j=0;j<17;j++) {
                if (area[i][j].type==BOMB) {
                    if (SDL_GetTicks()-area[i][j].time>=4000) {
                        area[i][j].type=EXPL;
                        if (area[i][j].atk>0) ptr[area[i][j].owner]->mun++;
                        while ( chain(i, j, area[i][j].power) );
                        ApplySurfaceSDL(FX1+i*40, FY1+j*40, expl, window->surface, &expl_clip[area[i][j].img][which_one(SDL_GetTicks()-area[i][j].time-4000)]);
                    } else {
                        ApplySurfaceSDL(FX1+i*40, FY1+j*40, bomb, window->surface, &bomb_clip[which_one2(SDL_GetTicks()-area[i][j].time)]);
                    }
                } else if (area[i][j].type==EXPL) {
                    if (SDL_GetTicks()-area[i][j].time>=4420) {
                        area[i][j].type=FREE;
                    } else {
                        ApplySurfaceSDL(FX1+i*40, FY1+j*40, expl, window->surface, &expl_clip[area[i][j].img][which_one(SDL_GetTicks()-area[i][j].time-4000)]);
                    }
                } else if (area[i][j].type==WALL) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, wall, window->surface, &wall_clip[0]);
                } else if (area[i][j].type==DESTROY) {
                    if (SDL_GetTicks()-area[i][j].time>=350) {
                        bns=rand()%100;
                        if (bns>0 && bns<11) area[i][j].type=BS_BOMB;
                        else if (bns>10 && bns<21) area[i][j].type=BS_FIRE;
                        else if (bns>20 && bns<26) area[i][j].type=BS_ROLL;
                        else if ((bns>25 && bns<28) && pro_pac==0)  { area[i][j].type=BS_PAC; pro_pac=1; }
                        else if (bns>27 && bns<31) area[i][j].type=BS_ATK;
                        else if (bns>30 && bns<34) area[i][j].type=BS_DEF;
                        else if (bns>33 && bns<35) area[i][j].type=BS_SKY;
                        else
                            area[i][j].type=FREE;
                    } else {
                        ApplySurfaceSDL(FX1+i*40, FY1+j*40, wall, window->surface, &wall_clip[which_one3(SDL_GetTicks()-area[i][j].time)]);
                    }
                } else if (area[i][j].type==BS_BOMB) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[0][1]:&bonus_clip[0][0]);
                } else if (area[i][j].type==BS_FIRE) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[1][1]:&bonus_clip[1][0]);
                } else if (area[i][j].type==BS_ROLL) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[2][1]:&bonus_clip[2][0]);
                } else if (area[i][j].type==BS_PAC) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[3][1]:&bonus_clip[3][0]);
                } else if (area[i][j].type==BS_SKY) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[4][1]:&bonus_clip[4][0]);
                } else if (area[i][j].type==BS_DEF) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[5][1]:&bonus_clip[5][0]);
                } else if (area[i][j].type==BS_ATK) {
                    ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, window->surface, SDL_GetTicks()%500>250?&bonus_clip[6][1]:&bonus_clip[6][0]);
                } else if (area[i][j].type==SKY) {
                    if (SDL_GetTicks()-area[i][j].time>2000) {
                        area[i][j].time-=2000;
                        area[i][j].type=BOMB;
                    } else {
                        window->apply(*skyfall, *skyfall_clip[(SDL_GetTicks()-area[i][j].time)/500], FX1 + i * 40, FY1 + j * 40 - 40);
                        window->apply(*skyfall, *skyfall_clip[4], FX1 + i * 40, FY1 + j * 40 - (2000 - (SDL_GetTicks() - area[i][j].time)) - 40);
                    }
                }
            }
        }
    }
}
