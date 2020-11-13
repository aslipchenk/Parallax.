#pragma once

//our frameworks
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>


//system libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#define GRAVITY 0.6f

#define STATUS_STATE_LIVES 0
#define STATUS_STATE_GAME 1
#define STATUS_STATE_GAMEOVER 2
#define STATUS_STATE_WIN 3

#define NUM_STARS 100
#define NUM_LEDGES 120

typedef struct
{
    float x, y;
    float dx, dy;
    short lives;
    char *name;
    int onLedge, isDead;

    int animFrame, facingLeft, slowingDown;
} Man;


typedef struct 
{
    int x, y, baseX, baseY, mode;
    float phase;
} Star;

typedef struct 
{
    int x, y, w, h;
} Ledge;

typedef struct 
{
    int x, y;    
}  Back;


typedef struct 
{
    int x, y, baseX;    
}  House;

typedef struct 
{
    int x, y, baseX;    
}  Mount;


typedef struct 
{
    int x, y, baseX;    
}  Village;

typedef struct 
{
    int x, y, baseX;    
}  Bridges;

// typedef struct 
// {
//     int x, y, w, h;
// } Floor;

typedef struct 
{

    //PATRALLAX
    House house;
    Mount mount;
    Village village;
    Bridges bridges;

    float scrollMount;
    float scrollX;
    float scrollVillage;
    float scrollBridge;

    bool multiplayer;

    Man secondPlayer;
    Man man;

    Star stars[NUM_STARS];

    // Floor floor[100]; 
    Ledge ledges[NUM_LEDGES];

    SDL_Texture *bridge;
    SDL_Texture *home;
    SDL_Texture *mountain;
    SDL_Texture *houses;
    SDL_Texture *star;
    SDL_Texture *manFrames[13];
    SDL_Texture *secFrames[13];
    SDL_Texture *brick;
    SDL_Texture *back[7];
    SDL_Texture *fire;
    SDL_Texture *label, *label2;
    int labelW, labelH, label2W, label2H;

    //MENU 
    SDL_Texture *menu;
    int menu_status;

    //SDL_Joystick *joystick;

    TTF_Font *font;

    int time, deathCountdown;
    int statusState;

    int musicChannel;
    Mix_Chunk *bgMusic, *jumpSound, *landSound, *dieSound, *menuMusic;

    SDL_Renderer *renderer;

} GameState;

//Prototypes (func)
void init_status_lives(GameState *game);
void draw_status_lives(GameState *game);
void shutdown_status_lives(GameState *game);

void init_game_over(GameState *game);
void draw_game_over(GameState *game);
void shutdown_game_over(GameState *game);

void init_game_win(GameState *game);
void draw_game_win(GameState *game);
void shutdown_game_win(GameState *game);
