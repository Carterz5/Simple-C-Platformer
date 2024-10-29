#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>





typedef struct Player {
    float Xpos, Ypos;
    int Xtile, Ytile;
    float Xvelocity, Yvelocity;
    float acceleration;

} Player;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void process_inputs(Player* player);
void process_physics(Player* player);
void init_player(Player* player, float speed);


#endif