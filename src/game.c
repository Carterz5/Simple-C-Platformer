#include "game.h"
#include <stdio.h>


int UpState = 0;
int DownState = 0;
int LeftState = 0;
int RightState = 0;
int SpaceState = 0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    switch (key){
    case GLFW_KEY_UP:
        UpState = action;
        break;
    case GLFW_KEY_DOWN:
        DownState = action;
        break;
    case GLFW_KEY_LEFT:
        LeftState = action;
        break;
    case GLFW_KEY_RIGHT:
        RightState = action;
        break;
    case GLFW_KEY_SPACE:
        SpaceState = action;
        break;
    default:
        break;
    }

    

}


void process_inputs(Player* player){

    if (RightState > GLFW_RELEASE){
       player->Xvelocity += player->acceleration;
    }
    if (LeftState > GLFW_RELEASE){
       player->Xvelocity -= player->acceleration;
    }
    if (UpState == GLFW_PRESS || SpaceState == GLFW_PRESS){
        player->Yvelocity = 10.0f;
    }
    

    
}

void init_player(Player* player, float acceleration){
    player->acceleration = acceleration;
    player->Xpos = 512.0f;
    player->Ypos = 500.0f;
    player->Xvelocity = 0.0f;
    player->Yvelocity = 0.0f;
    player->Xtile = 0;
    player->Ytile = 0;


}

void process_physics(Player* player){
    float friction = 2.0f;
    float gravity = 2.0f;
    
    if(player->Xvelocity > 20.0f){
        player->Xvelocity = 20.0f;
    } else if (player->Xvelocity < -20.0f){
        player->Xvelocity = -20.0f;
    }

    if(player->Yvelocity > 20.0f){
        player->Yvelocity = 20.0f;
    } else if (player->Yvelocity < -20.0f){
        player->Yvelocity = -20.0f;
    }



    if(player->Xvelocity <= 2.0f && player->Xvelocity >= -2.0f){
        player->Xvelocity = 0.0f;
    }
    if(player->Xvelocity > 0.0f){
        player->Xvelocity -= friction;
    } else if (player->Xvelocity < 0.0f){
        player->Xvelocity += friction;
    }

    player->Yvelocity -= gravity;

    player->Xpos += player->Xvelocity;
    player->Ypos += player->Yvelocity;



}