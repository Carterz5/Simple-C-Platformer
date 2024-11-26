#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "graphics.h"


enum Scene {
    MAIN_MENU = 0,
    LEVEL_ONE = 1,
    LEVEL_TWO = 2,
    LEVEL_THREE = 3,
    LEVEL_FOUR = 4,
    LEVEL_TEST = 10,


};


typedef struct Player {
    float Xpos, Ypos;
    float size;
    int Xtile, Ytile;
    float Xvelocity, Yvelocity;
    float Xstart, Ystart;
    float acceleration;
    float jump_height;
    float friction;
    float gravity;
    float maxspeed;
    float maxfall;
    int jumps;
    Quad quad;

} Player;

typedef struct Inputs {
    int UpState;
    int DownState;
    int LeftState;
    int RightState;
    int SpaceState;
    int EnterState;
    int F12State;
    int LastF12State;
    int F12Toggle;
    int F1State;
    int F2State;
} Inputs;

typedef struct Game {
    int scene;
    Inputs inputs;



} Game;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_inputs(Player* player, Inputs* inputs);
void process_physics(Player* player);
void update_player_coords(Player* player);
Player* init_player(float acceleration, float maxspeed, float maxfall, float size, float jump_height, float gravity, float friction, float textureID);
Game* init_game();
bool check_collision(Player* player, Quad* box);
void respawn_player(Player* player);
int process_collisions(Player* player, Quad tiles[16][12]);
void generate_level_data(Quad stage_data[16][12], float stage_array[192]);
void load_level_data(Quad level_data[4][16][12]);



#endif