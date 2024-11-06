#include "game.h"

#include <stdio.h>

 Inputs callback_keys;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    
    switch (key){
    case GLFW_KEY_UP:
        callback_keys.UpState = action;
        break;
    case GLFW_KEY_DOWN:
        callback_keys.DownState = action;
        break;
    case GLFW_KEY_LEFT:
        callback_keys.LeftState = action;
        break;
    case GLFW_KEY_RIGHT:
        callback_keys.RightState = action;
        break;
    case GLFW_KEY_SPACE:
        callback_keys.SpaceState = action;
        break;
    case GLFW_KEY_F12:
        callback_keys.F12State = action;
        break;
    default:
        break;
    }

    

}


void process_inputs(Player* player, Inputs* inputs){


    static unsigned int jump_cooldown = 0;


    if(callback_keys.F12State == GLFW_PRESS && callback_keys.F12Toggle == false){
        printf("toggle on\n");
        callback_keys.F12Toggle = true;
    } else if (callback_keys.F12State == GLFW_PRESS && callback_keys.F12Toggle == true){
        callback_keys.F12Toggle = false;
        printf("toggle off\n");
    }



    if (callback_keys.RightState > GLFW_RELEASE){
       player->Xvelocity += player->acceleration;
    }
    if (callback_keys.LeftState > GLFW_RELEASE){
       player->Xvelocity -= player->acceleration;
    }
    if ((callback_keys.UpState == GLFW_PRESS || callback_keys.SpaceState == GLFW_PRESS) && jump_cooldown == 0 && player->jumps > 0){
        player->Yvelocity = player->jump_height;
        player->jumps--;
        jump_cooldown = 10;
    }
    
    if(jump_cooldown > 0){
        jump_cooldown--;
    }

    *inputs = callback_keys;
}

void init_player(Player* player, float acceleration, float maxspeed, float maxfall, float size, float jump_height, float gravity, float friction, float textureID){
    player->acceleration = acceleration;
    player->Xpos = 512.0f;
    player->Ypos = 500.0f;
    player->size = size;
    player->Xvelocity = 0.0f;
    player->Yvelocity = 0.0f;
    player->Xtile = 0;
    player->Ytile = 0;
    player->jumps = 1;
    player->jump_height = jump_height;
    player->gravity = gravity;
    player->friction = friction;
    player->maxspeed = maxspeed;
    player->maxfall = maxfall;




    R_CreateQuad(&player->quad, 0.0f, 0.0f, player->size, 0.0f, 1.0f, 0.0f, 1.0f, textureID);



}

void init_game(Game* game){
    game->scene = 0;
    game->inputs.DownState = 0;
    game->inputs.UpState = 0;
    game->inputs.RightState = 0;
    game->inputs.LeftState = 0;
    game->inputs.SpaceState = 0;
    game->inputs.F12State = 0;
    game->inputs.F12Toggle = false;

}

void process_physics(Player* player){
    
    if(player->Xvelocity > player->maxspeed){
        player->Xvelocity = player->maxspeed;
    } else if (player->Xvelocity < -player->maxspeed){
        player->Xvelocity = -player->maxspeed;
    }


    if(player->Yvelocity < -player->maxfall){
        player->Yvelocity = -player->maxfall;
    }


    if(player->Xvelocity <= 1.0f && player->Xvelocity >= -1.0f){
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

void update_player_coords(Player* player){

    player->Xtile = (int)((player->Xpos + 16.0f) / 64);
    player->Ytile = (int)((player->Ypos + 16.0f) / 64);


}

void process_collisions(Player* player, Quad tiles[16][12]){

    float playerLeft = player->Xpos;
    float playerRight = player->Xpos + player->size;
    float playerBottom = player->Ypos;
    float playerTop = player->Ypos + player->size;



    for (int i = 0; i < 4; i++) {
        Quad current_tile;
        switch (i){
        //bottom
        case 0:
            if (player->Ytile == 0){
                continue;
            } else {
                current_tile = tiles[player->Xtile][player->Ytile-1];
            }
            break;
        //right
        case 1:
            if (player->Xtile == 15){
                continue;
            } else {
                current_tile = tiles[player->Xtile+1][player->Ytile];
            }
            break;
        //top
        case 2:
            if (player->Ytile == 11){
                continue;
            } else {
                current_tile = tiles[player->Xtile][player->Ytile+1];
            }
            
            break;
        //left
        case 3:
            if (player->Xtile == 0){
                continue;
            } else {
                current_tile = tiles[player->Xtile-1][player->Ytile];
            }
            
            break;
        
        default:
            break;
        }

        float quadLeft = current_tile.v0.Position[0];
        float quadRight = current_tile.v1.Position[0];
        float quadBottom = current_tile.v0.Position[1];
        float quadTop = current_tile.v2.Position[1];





        
        if(check_collision(player, &current_tile) == true){

            // Calculate overlap distances in each direction
            float overlapTop = playerTop - quadBottom;
            float overlapBottom = quadTop - playerBottom;
            float overlapLeft = quadRight - playerLeft;
            float overlapRight = playerRight - quadLeft;

            // Determine the smallest overlap to find the collision side
            if (overlapTop < overlapBottom && overlapTop < overlapLeft && overlapTop < overlapRight) {
                // Collision on top side of player
                player->Ypos = quadBottom - player->size - 1.0f;  
                player->Yvelocity = 0;                     // Stop vertical movement
            } else if (overlapBottom < overlapTop && overlapBottom < overlapLeft && overlapBottom < overlapRight) {
                // Collision on bottom side of player
                player->Ypos = quadTop + 1.0f;                    
                player->Yvelocity = 0;                     // Stop vertical movement
                player->jumps = 1;
            } else if (overlapLeft < overlapTop && overlapLeft < overlapBottom && overlapLeft < overlapRight && overlapLeft > 1.0f) {
                // Collision on left side of player
                player->Xpos = quadRight + 1.0f;                  
                player->Xvelocity = 0;                     // Stop horizontal movement
            } else if (overlapRight < overlapTop && overlapRight < overlapBottom && overlapRight < overlapLeft && overlapRight > 1.0f) {
                // Collision on right side of player
                player->Xpos = quadLeft - player->size - 1.0f;    
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


// 16w x 12h = 192 tiles

void generate_level_data(Quad stage_data[16][12], float stage_array[192]){


    unsigned int quadcount = 0;

    for (int i = 0; i < 12; i++){

        for (int j = 0; j < 16; j++){
            if(stage_array[quadcount] == 0){
                R_CreateQuad(&stage_data[j][i], -64.0f, -64.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            } else{
                R_CreateQuad(&stage_data[j][i], j * 64.0f, i * 64.0f, 64.0f, 0.0f, 0.0f, 0.0f, 1.0f, stage_array[quadcount]);
            }
            
            quadcount++;
        }
        


    }
    







}