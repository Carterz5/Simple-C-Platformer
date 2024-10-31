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

    static unsigned int jump_cooldown = 0;

    if (RightState > GLFW_RELEASE){
       player->Xvelocity += player->acceleration;
    }
    if (LeftState > GLFW_RELEASE){
       player->Xvelocity -= player->acceleration;
    }
    if ((UpState == GLFW_PRESS || SpaceState == GLFW_PRESS) && jump_cooldown == 0){
        player->Yvelocity = player->jump_height;
        jump_cooldown = 20;
    }
    
    if(jump_cooldown > 0){
        jump_cooldown--;
    }
    
}

void init_player(Player* player, float acceleration, float size, float jump_height, float gravity, float friction){
    player->acceleration = acceleration;
    player->Xpos = 512.0f;
    player->Ypos = 500.0f;
    player->size = size;
    player->Xvelocity = 0.0f;
    player->Yvelocity = 0.0f;
    player->Xtile = 0;
    player->Ytile = 0;
    player->jump_height = jump_height;
    player->gravity = gravity;
    player->friction = friction;

}

void process_physics(Player* player){
    
    if(player->Xvelocity > 15.0f){
        player->Xvelocity = 15.0f;
    } else if (player->Xvelocity < -15.0f){
        player->Xvelocity = -15.0f;
    }

    // if(player->Yvelocity > 20.0f){
    //     player->Yvelocity = 20.0f;
    // } else if (player->Yvelocity < -20.0f){
    //     player->Yvelocity = -20.0f;
    // }
    if(player->Yvelocity < -20.0f){
        player->Yvelocity = -20.0f;
    }


    if(player->Xvelocity <= 2.0f && player->Xvelocity >= -2.0f){
        player->Xvelocity = 0.0f;
    }
    if(player->Xvelocity > 0.0f){
        player->Xvelocity -= player->friction;
    } else if (player->Xvelocity < 0.0f){
        player->Xvelocity += player->friction;
    }

    player->Yvelocity -= player->gravity;

    player->Xpos += player->Xvelocity;
    player->Ypos += player->Yvelocity;



}

void process_collisions(Player* player, Quad tiles[]){

    float playerLeft = player->Xpos;
    float playerRight = player->Xpos + player->size;
    float playerBottom = player->Ypos;
    float playerTop = player->Ypos + player->size;



    for (int i = 0; i < 3; i++) {

        float quadLeft = tiles[i].v0.Position[0];
        float quadRight = tiles[i].v1.Position[0];
        float quadBottom = tiles[i].v0.Position[1];
        float quadTop = tiles[i].v2.Position[1];





        
        if(check_collision(player, &tiles[i]) == true){

            // Calculate overlap distances in each direction
            float overlapTop = playerTop - quadBottom;
            float overlapBottom = quadTop - playerBottom;
            float overlapLeft = quadRight - playerLeft;
            float overlapRight = playerRight - quadLeft;

            // Determine the smallest overlap to find the collision side
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // Collision on top side of player
                player->Ypos = quadBottom - player->size;  
                player->Yvelocity = 0;                     // Stop vertical movement
            } else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // Collision on bottom side of player
                player->Ypos = quadTop;                    
                player->Yvelocity = 0;                     // Stop vertical movement
            } else if (overlapLeft < overlapTop && overlapLeft < overlapBottom && overlapLeft < overlapRight) {
                // Collision on left side of player
                player->Xpos = quadRight;                  
                player->Xvelocity = 0;                     // Stop horizontal movement
            } else if (overlapRight < overlapTop && overlapRight < overlapBottom && overlapRight < overlapLeft) {
                // Collision on right side of player
                player->Xpos = quadLeft - player->size;    
                player->Xvelocity = 0;                     // Stop horizontal movement
            }




        }


    }
    


}

bool check_collision(Player* player, Quad* box){

    float playerLeft = player->Xpos;
    float playerRight = player->Xpos + player->size;
    float playerBottom = player->Ypos;
    float playerTop = player->Ypos + player->size;


    float quadLeft = box->v0.Position[0];
    float quadRight = box->v1.Position[0];
    float quadBottom = box->v0.Position[1];
    float quadTop = box->v2.Position[1];
    

    if (playerRight > quadLeft && playerLeft < quadRight && playerTop > quadBottom && playerBottom < quadTop) {
        return true; 
    }
    return false;

}