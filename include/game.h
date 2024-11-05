#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "graphics.h"





typedef struct Player {
    float Xpos, Ypos;
    float size;
    int Xtile, Ytile;
    float Xvelocity, Yvelocity;
    float acceleration;
    float jump_height;
    float friction;
    float gravity;
    Quad quad;

} Player;

typedef struct Inputs {
    int UpState;
    int DownState;
    int LeftState;
    int RightState;
    int SpaceState;
    int F12State;
    int F12Toggle;
} Inputs;

typedef struct Game {
    int scene;
    Inputs inputs;



} Game;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_inputs(Player* player, Inputs* inputs);
void process_physics(Player* player);
void update_player_coords(Player* player);
void init_player(Player* player, float acceleration, float size, float jump_height, float gravity, float friction, float textureID);
void init_game(Game* game);
bool check_collision(Player* player, Quad* box);
void process_collisions(Player* player, Quad tiles[]);

#endif