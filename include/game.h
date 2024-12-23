#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "graphics.h"
#include "audio.h"


enum Scene {
    SCENE_MAIN_MENU = 10,
    SCENE_LEVEL_ONE = 1,
    SCENE_LEVEL_TWO = 2,
    SCENE_LEVEL_THREE = 3,
    SCENE_LEVEL_FOUR = 4,
    SCENE_LEVEL_TEST = 0,
    SCENE_WIN = 11,


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
    int GState, LastGState, GToggle;
    int SpaceState;
    int EnterState;
    int EscapeState;
    int OState, LastOState, OToggle;
    int F12State, LastF12State, F12Toggle;
    int F1State;
    int F2State;
    int F3State;
    int F4State;
    int F5State;
    int F6State;
} Inputs;

typedef struct Game {
    int scene;
    Inputs inputs;



} Game;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_inputs(Player* player, Inputs* inputs, Sound sound_data[3], Game* game, Renderer* player_renderer);
void process_physics(Player* player);
void update_player_coords(Player* player);
Player* init_player(float acceleration, float maxspeed, float maxfall, float size, float jump_height, float gravity, float friction, float textureID);
Game* init_game();
bool check_collision(Player* player, Quad* box);
void respawn_player(Player* player);
int process_collisions(Player* player, Quad tiles[16][12], Sound sound_data[3]);
void generate_level_data(Quad stage_data[16][12], float stage_array[192]);
void load_level_data(Quad level_data[4][16][12]);
void switch_scene(int scene, Game* game, Player* P1, Renderer* batch_renderer, Renderer* player_renderer, Sound sound_data[3], Quad level_data[4][16][12]);



#endif