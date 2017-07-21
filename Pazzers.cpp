#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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


typedef struct _XY {
  int x;
  int y;	
} XY;


struct _terrain {
  int type;	
  int time;	
  int power;
  int img;
  int owner;
  int atk;
} area[19][17];


int FPS=30;
int pro_pac=0;
int pro_block=75;
const int WIDTH=1014;
const int HEIGHT=759;
const int DEPTH=32;
const int button[4][5]={
  SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_KP0,
  SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_TAB,
  SDLK_i, SDLK_k, SDLK_j, SDLK_l, SDLK_SPACE,
  SDLK_KP_DIVIDE, SDLK_KP8, SDLK_KP7, SDLK_KP9, SDLK_KP0
};
const int initxy[4][2]={214, 69, 934, 710, 934, 69, 214, 710};
XY pacXY={-1, -1};
SDL_Joystick *stick=NULL;
SDL_Surface *screen=NULL;
SDL_Surface *field=NULL;
SDL_Surface *number=NULL;
SDL_Surface *skyfall=NULL;
SDL_Surface *status_img=NULL;
SDL_Surface *pacman=NULL;
SDL_Surface *life_img=NULL;
SDL_Surface *text=NULL;
SDL_Rect numbs[11][2];
TTF_Font *font1, *font2;
SDL_Event event;
SDL_Color black={0, 0, 0};
SDL_Color white ={255, 255, 255};


int sqr(int n, int x) {
  int i, j=n;
  
  if (!x) return 1;
  for (i=1;i<x;i++) n*=j;
  return n;	
}


void NegativeSDL(SDL_Surface *surface) {
  Uint32 *pixels = (Uint32 *) surface->pixels;
  int i;    
  
  for (i = 0; i< (surface->w * surface->h); i++) {
	if (pixels[i] != SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF))     
      pixels[i] = SDL_MapRGB(surface->format, 
        0xff-(pixels[i]>>16),
        0xff-pixels[i]%256,
        0xff-(pixels[i]>>8)%256
      );
  }
}


SDL_Surface *LoadImageSDL(const char *filename) {
  SDL_Surface* loadedImage=NULL;
  SDL_Surface* optimizedImage=NULL;

  loadedImage=IMG_Load(filename);
  optimizedImage=SDL_DisplayFormat(loadedImage);
  SDL_FreeSurface(loadedImage);
  SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0x00, 0xFF ) );
  return optimizedImage;
}


void ApplySurfaceSDL(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL) {
  SDL_Rect offset;
  
  offset.x=x;
  offset.y=y-40;
  SDL_BlitSurface( source, clip, destination, &offset );
}


void InitSDL(const char *title) {
  SDL_Init(SDL_INIT_EVERYTHING);
  screen=SDL_SetVideoMode(0, 0, DEPTH, SDL_HWSURFACE | SDL_FULLSCREEN );
  SDL_WM_SetCaption(title, NULL);
  srand(time(NULL));
  TTF_Init();
}


void show_num(int *x, int *y, const char *str, int t) {
  int i=0;	
  
  t=1250-(SDL_GetTicks()-t);
  if (t<0) t=0;
  else if (t>1000) t=1000;
  SDL_SetAlpha(number, SDL_SRCALPHA, t/4);
  if (str[i]=='-') {
	ApplySurfaceSDL(*x+(i*10), *y, number, screen, &numbs[10][1]);
	i++;  
    while (str[i]) {
	    ApplySurfaceSDL(*x+(i*10), *y, number, screen, &numbs[str[i]-'0'][1]);	
      i++;  
    }
  } else { 
    if (str[i]=='+') {
	  ApplySurfaceSDL(*x+(i*10), *y, number, screen, &numbs[10][0]);
	  i++; 
    }  
    while (str[i]) {
	    ApplySurfaceSDL(*x+(i*10), *y, number, screen, &numbs[str[i]-'0'][0]);	
      i++;  
    }
  }
  *y=*y-1;	
}


class Men {
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


class Terrain {
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
	void cicle(Men *ptr);
	int chain(int x, int y, int pwr);
	Terrain();
	int which_one(int phase);
	int which_one2(int phase);
	int which_one3(int phase);
	void kill() {SDL_FreeSurface(bomb); SDL_FreeSurface(expl); SDL_FreeSurface(wall); SDL_FreeSurface(bonus); }
};


Terrain::Terrain() {   
  Uint8 i, j;	
	  
  wall=LoadImageSDL("arena/wall.bmp");
  bomb=LoadImageSDL("arena/bomb.bmp");
  expl=LoadImageSDL("arena/expl.bmp");
  bonus=LoadImageSDL("arena/bonus.bmp");
  for (i=0;i<7;i++) {
	for (j=0;j<4;j++) {
	  expl_clip[i][j].x=i*40;
	  expl_clip[i][j].y=j*40;
	  expl_clip[i][j].w=40;
	  expl_clip[i][j].h=40;	
	}   
  }
  for (i=0;i<4;i++) {
    bomb_clip[i].x=i*40;
	bomb_clip[i].y=0;
    bomb_clip[i].w=40;
	bomb_clip[i].h=40;	  
  } 
  for (i=0;i<3;i++) {
    wall_clip[i].x=i*40;
	wall_clip[i].y=0;
    wall_clip[i].w=40;
	wall_clip[i].h=40;	  
  } 
  for (i=0;i<7;i++) {
	for (j=0;j<2;j++) {
      bonus_clip[i][j].x=i*40;
	  bonus_clip[i][j].y=j*40;
      bonus_clip[i][j].w=40;
      bonus_clip[i][j].h=40; 
	}  
  }
  
  for (i=0;i<19;i++) {
	for (j=0;j<17;j++) {
	  if ((!(j%2==1 && i%2==1)) && (rand()%100<pro_block)){
		area[i][j].type=WALL; 
	  }		
	}   
  }
  
  for (i=0;i<11;i++) {
	for (j=0;j<2;j++) {
      numbs[i][j].x=i*10;
	  numbs[i][j].y=j*20;
      numbs[i][j].w=10;
      numbs[i][j].h=20; 
	}   
  }
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


void Terrain::cicle(Men *ptr) {
  Uint8 i, j, bns;	
  
  for (i=0;i<19;i++) {
	for (j=0;j<17;j++) {
	  if (area[i][j].type==BOMB) {
		if (SDL_GetTicks()-area[i][j].time>=4000) {
		  area[i][j].type=EXPL;
		  if (area[i][j].atk>0) ptr[area[i][j].owner].mun++;
		  while ( chain(i, j, area[i][j].power) );	
		  ApplySurfaceSDL(FX1+i*40, FY1+j*40, expl, screen, &expl_clip[area[i][j].img][which_one(SDL_GetTicks()-area[i][j].time-4000)]);
		} else {  
		  ApplySurfaceSDL(FX1+i*40, FY1+j*40, bomb, screen, &bomb_clip[which_one2(SDL_GetTicks()-area[i][j].time)]);
		}  
	  } else if (area[i][j].type==EXPL) {
		if (SDL_GetTicks()-area[i][j].time>=4420) {
		  area[i][j].type=FREE;  	
		} else {
		  ApplySurfaceSDL(FX1+i*40, FY1+j*40, expl, screen, &expl_clip[area[i][j].img][which_one(SDL_GetTicks()-area[i][j].time-4000)]);   	
		}  
	  } else if (area[i][j].type==WALL) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, wall, screen, &wall_clip[0]);  
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
		  ApplySurfaceSDL(FX1+i*40, FY1+j*40, wall, screen, &wall_clip[which_one3(SDL_GetTicks()-area[i][j].time)]);   	
		}    
	  } else if (area[i][j].type==BS_BOMB) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[0][1]:&bonus_clip[0][0]); 
	  } else if (area[i][j].type==BS_FIRE) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[1][1]:&bonus_clip[1][0]); 
	  } else if (area[i][j].type==BS_ROLL) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[2][1]:&bonus_clip[2][0]); 
	  } else if (area[i][j].type==BS_PAC) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[3][1]:&bonus_clip[3][0]);
	  } else if (area[i][j].type==BS_SKY) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[4][1]:&bonus_clip[4][0]);  
	  } else if (area[i][j].type==BS_DEF) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[5][1]:&bonus_clip[5][0]); 
	  } else if (area[i][j].type==BS_ATK) {
		ApplySurfaceSDL(FX1+i*40, FY1+j*40, bonus, screen, SDL_GetTicks()%500>250?&bonus_clip[6][1]:&bonus_clip[6][0]); 
	  } else if (area[i][j].type==SKY) {
		if (SDL_GetTicks()-area[i][j].time>2000) {
		  area[i][j].time-=2000;
		  area[i][j].type=BOMB;	
		} else {  
		  ApplySurfaceSDL(FX1+i*40, FY1+j*40, skyfall, screen, &bonus_clip[(SDL_GetTicks()-area[i][j].time)/500][0] );
		  ApplySurfaceSDL(FX1+i*40, FY1+j*40-(2000-(SDL_GetTicks()-area[i][j].time)), skyfall, screen, &bonus_clip[4][0] );
		}  
	  }
 	}
  } 	
}


void no_shadow_on_my_head(int *tab, int size, Men *inf) {
  Uint8 i, j;
  int *min, to;	
  
  for (i=0;i<size;i++) tab[i]=inf[i].xy[1].y;
  for (i=0;i<size;i++) {
	min=&tab[i];  
	for (j=i;j<size;j++) {
	  if (tab[j]<*min) min=&tab[j];	
	}  
	to=*min;
	*min=tab[i]; 
	tab[i]=to;
  }
  for (i=0;i<size;i++) for (j=0;j<size;j++) if (inf[i].xy[1].y==tab[j]) { tab[j]=inf[i].id; break;}	
}


const char *bmp_name (int i) {
  switch(i) {
	case 0: return "men/men1.bmp";  
	case 1: return "men/men2.bmp";  
	case 2: return "men/men3.bmp";  
	case 3: return "men/men4.bmp";  
  }	
}


void Men::status() {
  char str[10];	
	
  ApplySurfaceSDL(17, 50+id*190, status_img, screen, NULL);
  ApplySurfaceSDL(67, 70+id*190, obj, screen, dead==-2?&obj_clip[5][0]:&obj_clip[0][0]);
  life_clip.w=life;	
  ApplySurfaceSDL(54, 130+id*190, life_img, screen, &life_clip);
  
  sprintf(str, "%d", life);
  text=TTF_RenderText_Solid(font1, (const char*)str , black );
  ApplySurfaceSDL(27, id*190+130, text, screen, NULL);
  SDL_FreeSurface(text);
  
  sprintf(str, "%d", atk);
  text=TTF_RenderText_Solid(font1, (const char*)str , black );
  ApplySurfaceSDL(atk>9?106:109, id*190+156, text, screen, NULL);
  SDL_FreeSurface(text);
  
  sprintf(str, "%d", def);
  text=TTF_RenderText_Solid(font1, (const char*)str , black );
  ApplySurfaceSDL(def>9?106:109, id*190+183, text, screen, NULL);
  SDL_FreeSurface(text);
  
  sprintf(str, "%d", mun_max);
  text=TTF_RenderText_Solid(font2, (const char*)str , black );
  ApplySurfaceSDL(mun_max>9?45:47, id*190+153, text, screen, NULL);
  SDL_FreeSurface(text);
  
  sprintf(str, "%d", power);
  text=TTF_RenderText_Solid(font2, (const char*)str , black );
  ApplySurfaceSDL(power>9?45:47, id*190+170, text, screen, NULL);
  SDL_FreeSurface(text);
   
  sprintf(str, "%d", speed);
  text=TTF_RenderText_Solid(font2, (const char*)str , black );
  ApplySurfaceSDL(speed>9?45:47, id*190+187, text, screen, NULL);
  SDL_FreeSurface(text);
  
}


void Men::make_fun(int phase) {
  phase-=dead;	
  if (phase<1500){
    ApplySurfaceSDL(xy[0].x, xy[0].y, obj, screen, &obj_clip[4][(phase/250)%2]);
  }	else if (phase<1750) {
    ApplySurfaceSDL(xy[0].x, xy[0].y, obj, screen, &obj_clip[4][2]);
  }	else if (phase<1900) {
    ApplySurfaceSDL(xy[0].x, xy[0].y, obj, screen, &obj_clip[5][2]);
  } else if (xy[0].y>-50) {
	ApplySurfaceSDL(xy[0].x, xy[0].y, obj, screen, &obj_clip[5][(phase/100)%2]);
	xy[0].y-=count/2;
	if (phase>2000) count+=1;  
  } else dead=-2; 
}


void Men::initialize(const char *name, int id_) {
  Uint8 i,j;
  	
  obj=LoadImageSDL(name);
  sht=LoadImageSDL(name);
  NegativeSDL(sht);	
  dir=DOWN;
  mun=2;
  pac=0;
  mun_max=mun;  
  power=4;
  atk=26;
  def=1;
  life=100;
  id=id_;
  count=0;
  time=SDL_GetTicks()-4000;
  xy[0].x=initxy[id][0];
  xy[0].y=initxy[id][1];
  joy.x=0;
  joy.y=0;
  message.time=-1;
  dead=-1;
  dmg=0;
  speed=120/FPS;
  mov=false;
  alter=false;
  up=button[id][0];
  down=button[id][1];
  left=button[id][2];
  right=button[id][3];
  drop=button[id][4];
  for (i=0;i<10;i++) cheat[i]=0;
  for (i=0;i<6;i++) {
	for (j=0;j<3;j++) {
	  obj_clip[i][j].x=j*40;
	  obj_clip[i][j].y=i*50;
	  obj_clip[i][j].w=40;
	  obj_clip[i][j].h=50;	
	}  
  }
  life_clip.x=2;
  life_clip.y=0;
  life_clip.h=19;	
} 


void Men::handle(int msg, int type) {
  int i;	
	
  if (alter==true && event.type==SDL_JOYAXISMOTION) {
	if (event.jaxis.axis==0) joy.x=event.jaxis.value;
    if (event.jaxis.axis==1) joy.y=event.jaxis.value; 
    if (abs(joy.x)>8000 || abs(joy.y)>8000) {
	  mov=true;
	  if (abs(joy.y)<abs(joy.x)) {
	    if(joy.x>0) 
	      dir=RIGHT;
	    else 
	      dir=LEFT;	
	  } else {
	    if(joy.y>0) 
	      dir=DOWN;
	    else 
	      dir=UP;	
	  }
	  if (cheat[0]!=dir) { 
        for (i=9;i>0;i--) cheat[i]=cheat[i-1];
        cheat[i]=dir; 
      }
    } else mov=false;      
  } else if (alter==true && event.type==SDL_JOYBUTTONDOWN) {         
	if ((event.jbutton.button) && (mun>0)) {
	  if (area[xy[3].x][xy[3].y].type==0 && mun>0) {
		area[xy[3].x][xy[3].y].owner=id;
		area[xy[3].x][xy[3].y].type=1;
		area[xy[3].x][xy[3].y].time=SDL_GetTicks();
		area[xy[3].x][xy[3].y].power=power;
		area[xy[3].x][xy[3].y].img=0;
		area[xy[3].x][xy[3].y].atk=atk;
		mun--;
	  } 
	}   
  } else if (alter==false){	
    switch (type) {
	  case SDL_KEYDOWN: { 
		if (msg==SDLK_ESCAPE) exit(0);  
		if (msg==SDLK_t) SDL_SaveBMP(screen,"shot.bmp");  
	    if (msg==up) { dir=UP; mov=true; } 
        if (msg==down) { dir=DOWN; mov=true; } 
        if (msg==left) { dir=LEFT; mov=true; } 
        if (msg==right) { dir=RIGHT; mov=true; }
        if (msg==drop) {
		  if (area[xy[3].x][xy[3].y].type==0 && mun>0) {
			area[xy[3].x][xy[3].y].owner=id;
			area[xy[3].x][xy[3].y].type=1;
			area[xy[3].x][xy[3].y].time=SDL_GetTicks();
			area[xy[3].x][xy[3].y].power=power;
			area[xy[3].x][xy[3].y].img=0;
		    area[xy[3].x][xy[3].y].atk=atk;
			mun--;
		  } 
		}
        for (i=9;i>0;i--) cheat[i]=cheat[i-1];
        cheat[i]=dir;
        break; 
      }   
	  case SDL_KEYUP: {  
        if (((dir==UP) && (msg==up)) ||
            ((dir==DOWN) && (msg==down)) || 
            ((dir==LEFT) && (msg==left)) || 
            ((dir==RIGHT) && (msg==right))  
        ) mov=false; 
	    break;  
	  }   
    }
  }
}


int Men::alive(Uint8 i) {
  if (mov==false) return 0;
  if (i<4) { return 0; } else
  if (i<12) { return 1; } else
  if (i<16) { return 0; } else
  if (i<23) { return 2; } 
}


void Men::show() {
  if (pac)
    ApplySurfaceSDL(xy[0].x, xy[0].y, pacman, screen, &obj_clip[dir][alive(++count)]);
  else   	
    ApplySurfaceSDL(xy[0].x, xy[0].y, dmg?(SDL_GetTicks()%200>100?obj:sht):obj, screen, &obj_clip[dir][alive(++count)]);
  count%=22;
  if (message.time!=-1) {
    show_num(&message.xy.x, &message.xy.y, message.text, message.time);	
    if (SDL_GetTicks()-message.time>1000) message.time=-1; 
  }  
}


void Men::move() {
  if (mov==true) switch (dir) {
	case UP: xy[0].y+=-speed; break;	
	case DOWN: xy[0].y+=speed; break;	
	case LEFT: xy[0].x+=-speed; break;	
	case RIGHT: xy[0].x+=speed; break;	
  }
  xy[1].x=xy[0].x+2;
  xy[1].y=xy[0].y+12;
  xy[2].x=xy[0].x+38;
  xy[2].y=xy[0].y+49;
  xy[3].x=(((xy[1].x+xy[2].x)/2)-FX1)/40;	
  xy[3].y=(((xy[1].y+xy[2].y)/2)-FY1)/40;
  check();
} 


void Men::check() {
  int i, j;	
	
  if (xy[1].x<FX1) { xy[0].x+=FX1-xy[1].x; xy[1].x=xy[0].x+2; }
  if (xy[1].y<FY1) { xy[0].y+=FY1-xy[1].y; xy[1].y=xy[0].y+12; }
  if (xy[2].x>FX2) { xy[0].x+=FX2-xy[2].x; xy[2].x=xy[0].x+38; }
  if (xy[2].y>FY2) { xy[0].y+=FY2-xy[2].y; xy[2].y=xy[0].y+49; }
  switch (dir) {
	case UP: {
	  int my_x=(((xy[1].x-215)/80)*80+255); 	 
	  if ( (((xy[1].y-40)/40)%2==0) && ((( (xy[1].x>my_x) && (xy[1].x<my_x+39) )) || (( (xy[2].x>my_x) || (xy[2].x>my_x+39) )))) {
		xy[0].y+=speed;
		if (xy[1].x-my_x<-5) xy[0].x+=-4;
		if (my_x+39-xy[2].x<-5) xy[0].x+=4;
	  }
	  if (((area[xy[3].x][xy[3].y-1].type==WALL) || (area[xy[3].x][xy[3].y-1].type==BOMB)) && (xy[2].y<(FY1+40*(xy[3].y+1)))) {
	    xy[0].y-=-speed;  
      }  
	  break; 
	}
	case DOWN: {
	  int my_x=(((xy[1].x-215)/80)*80+255); 	 
	  if ( (((xy[2].y-40)/40)%2==0) && ((( (xy[1].x>my_x) && (xy[1].x<my_x+39) )) || (( (xy[2].x>my_x) || (xy[2].x>my_x+39) )))) {
		xy[0].y+=-speed;
		if (xy[1].x-my_x<-5) xy[0].x+=-4;
		if (my_x+39-xy[2].x<-5) xy[0].x+=4;
	  }
	  if (((area[xy[3].x][xy[3].y+1].type==WALL) || (area[xy[3].x][xy[3].y+1].type==BOMB)) && (xy[2].y>(FY1+40*(xy[3].y+1)))) {
	    xy[0].y-=speed;  
      } 
	  break; 
	}
	case RIGHT: {
	  int my_y=(40+80*(xy[1].y/80));	 
	  if ( (((xy[2].x-175)/40)%2==0) && ((((xy[1].y<my_y+39) && (xy[1].y>my_y))) || (((xy[2].y<my_y+39) && (xy[2].y>my_y))))) {
		xy[0].x+=-speed;
		if (xy[1].y-my_y<-5) xy[0].y+=-4;
		if (my_y+39-xy[2].y<-5) xy[0].y+=4;
	  }
	  if (xy[3].x!=18)
	    if (((area[xy[3].x+1][xy[3].y].type==WALL) || (area[xy[3].x+1][xy[3].y].type==BOMB)) && (xy[2].x>(FX1+40*(xy[3].x+1)))) {
	      xy[0].x-=speed;  
      } 
	  break; 
	}
	case LEFT:  {
	  int my_y=(40+80*(xy[1].y/80));	 
	  if ( (((xy[1].x-175)/40)%2==0) && ((((xy[1].y<my_y+39) && (xy[1].y>my_y))) || (((xy[2].y<my_y+39) && (xy[2].y>my_y))))) {
		xy[0].x+=speed;
		if (xy[1].y-my_y<-5) xy[0].y+=-4;
		if (my_y+39-xy[2].y<-5) xy[0].y+=4;
	  }
	  if (((area[xy[3].x-1][xy[3].y].type==WALL) || (area[xy[3].x-1][xy[3].y].type==BOMB)) && (xy[2].x<(FX1+40*(xy[3].x+1)))) {
	    xy[0].x-=-speed;  
      }  
	  break; 
	}  
  }
  if (pac) {
    pacXY.x=xy[3].x;
	pacXY.y=xy[3].y;
    if (SDL_GetTicks()-time>10000) {
      pacXY.x=-1;
	  pacXY.y=-1;
	  pac=0;
	  pro_pac=0;
	  speed-=3;
	}  
  } else {  
    if (area[xy[3].x][xy[3].y].type==EXPL && SDL_GetTicks()-time>4500) {
      if ( area[xy[3].x][xy[3].y].atk<=0 && abs(area[xy[3].x][xy[3].y].atk)!=id ) {
	    life/=2;
        message.time=SDL_GetTicks(); 
        sprintf(message.text, "-%d",life);
        message.xy.x=xy[0].x;
        message.xy.y=xy[0].y-20;
        dmg=1;
        time=area[xy[3].x][xy[3].y].time;
      } else if (area[xy[3].x][xy[3].y].atk>0) {
		life-=area[xy[3].x][xy[3].y].atk-def;
        message.time=SDL_GetTicks(); 
        sprintf(message.text, "-%d", area[xy[3].x][xy[3].y].atk-def);
        message.xy.x=xy[0].x;
        message.xy.y=xy[0].y-20;
        dmg=1;
        time=area[xy[3].x][xy[3].y].time;
      }  
    }
  }    
  if (SDL_GetTicks()-time>4500 && dmg==1) dmg=0;
  if (life<0) life=0;
  else if (area[xy[3].x][xy[3].y].type==BS_BOMB) {
	area[xy[3].x][xy[3].y].type=FREE;
	mun++;
	mun_max++; 
    message.time=SDL_GetTicks(); 
    sprintf(message.text, "%d", mun_max);
    message.xy.x=xy[0].x;
    message.xy.y=xy[0].y-20;  
  }
  else if (area[xy[3].x][xy[3].y].type==BS_FIRE) {
	area[xy[3].x][xy[3].y].type=FREE;
	power++;   
    message.time=SDL_GetTicks(); 
    sprintf(message.text, "%d", power);
    message.xy.x=xy[0].x;
    message.xy.y=xy[0].y-20;   
  }
  else if (area[xy[3].x][xy[3].y].type==BS_PAC) {
	area[xy[3].x][xy[3].y].type=FREE;
	time=SDL_GetTicks();
	if (!pac) {
	  pac=1; 
	  speed+=3;
	}    
  }
  else if (area[xy[3].x][xy[3].y].type==BS_ROLL) {
	area[xy[3].x][xy[3].y].type=FREE;
	if (speed<9) speed++; 
    message.time=SDL_GetTicks(); 
    sprintf(message.text, "%d", speed);
    message.xy.x=xy[0].x;
    message.xy.y=xy[0].y-20;     
  }
  else if (area[xy[3].x][xy[3].y].type==BS_ATK) {
	area[xy[3].x][xy[3].y].type=FREE;
	atk+=9; 
    message.time=SDL_GetTicks(); 
    sprintf(message.text, "%d", atk);
    message.xy.x=xy[0].x;
    message.xy.y=xy[0].y-20;     
  }
  else if (area[xy[3].x][xy[3].y].type==BS_DEF) {
	area[xy[3].x][xy[3].y].type=FREE;
	def+=4; 
    message.time=SDL_GetTicks(); 
    sprintf(message.text, "%d", def);
    message.xy.x=xy[0].x;
    message.xy.y=xy[0].y-20;     
  }
  else if (area[xy[3].x][xy[3].y].type==BS_SKY) {
	area[xy[3].x][xy[3].y].type=FREE;
	for (i=0;i<19;i++) {
	  for (j=0;j<17;j++) {
	    if ((!(j%2==1 && i%2==1)) && (rand()%100<7) && area[i][j].type==FREE){
		  area[i][j].type=SKY;
		  area[i][j].time=SDL_GetTicks();
		  area[i][j].power=20;
		  area[i][j].atk=-id;
	    }		
	  }   
    }
  }
  else if (pacXY.x==xy[3].x && pacXY.y==xy[3].y && !pac) {
	life=0;   
  }
  if (life<1) {
    dead=SDL_GetTicks();
    count=0; 
  }
  for (i=0; (cheat[i]==i%2) && (i<10) ; i++);
  if (i==10) {
	area[xy[3].x][xy[3].y].type=BS_SKY;
	cheat[0]=8;
  }	  
  for (i=0; (cheat[i]==(i%2)+2) && (i<10) ; i++);
  if (i==10) {
	life=75;
	cheat[0]=8;
  }	  
}


void Game(int players) {
  Terrain nature;	
  bool quit=false;	
  Men men[players];
  int clock, turn[3];
  int joysticks=SDL_NumJoysticks();
  int joy_id[players];
  SDL_Joystick *stick[joysticks];
  Uint8 i, j=players;
  
  
  // Initialize..
  status_img=LoadImageSDL("arena/status.bmp");
  life_img=LoadImageSDL("arena/life.bmp");  	
  number=LoadImageSDL("arena/number.bmp");  		
  field=LoadImageSDL("arena/Field.bmp");
  pacman=LoadImageSDL("arena/pac.bmp");
  skyfall=LoadImageSDL("arena/skyfall.bmp");
  font1=TTF_OpenFont( "arena/Ubuntu-R.ttf", 16);
  font2=TTF_OpenFont( "arena/Ubuntu-R.ttf", 14);
  for (i=0;i<players;i++) men[i].initialize(bmp_name(i),i);
  while (j-->0 && joysticks-->0) {
	men[j].alter=true;                       
	stick[j]=SDL_JoystickOpen(j-players+1);
	joy_id[j-players+1]=j;
  }
  //-----------
  
  
  // Game loop..
  while (quit==false) {
	clock=SDL_GetTicks();  	  
    while (SDL_PollEvent(&event)) {
	  if (event.type==SDL_KEYDOWN || event.type==SDL_KEYUP)	{
		for (i=0;i<players;i++) {  
          if (men[i].dead==-1) {
			men[i].handle(event.key.keysym.sym, event.type);
		  }	
        }  
      } 
	  if (event.type==SDL_JOYAXISMOTION) {
		if (men[joy_id[event.jaxis.which]].dead==-1)    
		  men[joy_id[event.jaxis.which]].handle(0, 0); 
	  }	
	  if (event.type==SDL_JOYBUTTONDOWN) {
		if (men[joy_id[event.button.which]].dead==-1)    
		  men[joy_id[event.button.which]].handle(0, 0); 
	  }	
      if (event.type==SDL_QUIT) quit=true; 
    }
    SDL_FillRect(screen, &screen->clip_rect, 0x000000);
    for (i=0;i<players;i++) men[i].status();
    ApplySurfaceSDL(174, 39, field, screen, NULL);
    nature.cicle(men);
    no_shadow_on_my_head(turn, players, men);
    for (i=0;i<players;i++) {
	  if (men[turn[i]].dead==-1) {
		men[turn[i]].move();
	    men[turn[i]].show();
	  } else if (men[turn[i]].dead!=-2) {
		men[turn[i]].make_fun(SDL_GetTicks());   
	  } 	
	}
    SDL_Flip(screen);
    if ((clock=SDL_GetTicks()-clock)<1000/FPS) {
      SDL_Delay(1000/FPS-clock);	 
    }  
  }
  //----------
  	
  
  // Killing..
  SDL_FreeSurface(field);
  SDL_FreeSurface(status_img);
  SDL_FreeSurface(life_img);
  SDL_FreeSurface(pacman);
  SDL_FreeSurface(number);
  SDL_FreeSurface(skyfall);
  nature.kill();
  TTF_CloseFont(font1);
  TTF_CloseFont(font2);
  for (j=players;j-->0 && joysticks-->0;) {
	SDL_JoystickClose(stick[j]); 
  }	
  for (i=0;i<players;i++) {  
    men[i].kill();
  }
  //---------
      
}


int main(int argc, char *argv[]) {
  InitSDL("Pazzers");
  Game( argc>1?atoi(argv[1]):4 );
  TTF_Quit();
  SDL_Quit();	
  return 0;	
}
  
