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
    case GLFW_KEY_O:
        callback_keys.OState = action;
        break;
    case GLFW_KEY_ENTER:
        callback_keys.EnterState = action;
        break;
    case GLFW_KEY_F12:
        callback_keys.F12State = action;
        break;
    case GLFW_KEY_F1:
        callback_keys.F1State = action;
        break;
    case GLFW_KEY_F2:
        callback_keys.F2State = action;
        break;
    default:
        break;
    }

    

}


void process_inputs(Player* player, Inputs* inputs, Sound sound_data[10]){


    static unsigned int jump_cooldown = 0;


    if(callback_keys.F12State == GLFW_RELEASE && callback_keys.LastF12State == GLFW_PRESS && callback_keys.F12Toggle == false){
        callback_keys.F12Toggle = true;
    } else if (callback_keys.F12State == GLFW_RELEASE && callback_keys.LastF12State == GLFW_PRESS && callback_keys.F12Toggle == true){
        callback_keys.F12Toggle = false;
    }

    if(callback_keys.OState == GLFW_RELEASE && callback_keys.LastOState == GLFW_PRESS && callback_keys.OToggle == false){
        callback_keys.OToggle = true;
    } else if (callback_keys.OState == GLFW_RELEASE && callback_keys.LastOState == GLFW_PRESS && callback_keys.OToggle == true){
        callback_keys.OToggle = false;
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
        play_sound(&sound_data[SOUND_TEST]);
    }
    
    if(jump_cooldown > 0){
        jump_cooldown--;
    }


    callback_keys.LastF12State = callback_keys.F12State;
    callback_keys.LastOState = callback_keys.OState;

    *inputs = callback_keys;
}

Player* init_player(float acceleration, float maxspeed, float maxfall, float size, float jump_height, float gravity, float friction, float textureID){
    Player* player = malloc(sizeof(Player));
    
    player->acceleration = acceleration;
    player->Xpos = 512.0f;
    player->Ypos = 500.0f;
    player->Xstart = 512.0f;
    player->Ystart = 500.0f;
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




    R_CreateQuad(&player->quad, 0.0f, 0.0f, player->size, player->size, 0.0f, 1.0f, 0.0f, 1.0f, textureID);

    return player;

}

Game* init_game(){
    Game* game = malloc(sizeof(Game));
    game->scene = MAIN_MENU;
    game->inputs.DownState = 0;
    game->inputs.UpState = 0;
    game->inputs.RightState = 0;
    game->inputs.LeftState = 0;
    game->inputs.SpaceState = 0;
    game->inputs.F12State = 0;
    game->inputs.LastF12State = 0;
    game->inputs.F12Toggle = false;
    game->inputs.F1State = 0;
    game->inputs.F2State = 0;
    return game;
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

int process_collisions(Player* player, Quad tiles[16][12]){

    float playerLeft = player->Xpos;
    float playerRight = player->Xpos + player->size;
    float playerBottom = player->Ypos;
    float playerTop = player->Ypos + player->size;



    for (int i = 0; i < 5; i++) {
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
        //self
        case 4:

            current_tile = tiles[player->Xtile][player->Ytile];

            
            break;
        
        default:
            break;
        }

        float quadLeft = current_tile.v0.Position[0];
        float quadRight = current_tile.v1.Position[0];
        float quadBottom = current_tile.v0.Position[1];
        float quadTop = current_tile.v2.Position[1];

        // if(check_collision(player, &current_tile) == true && (int)current_tile.v0.TexID >= TEXTURE_SPIKEUP && (int)current_tile.v0.TexID <= TEXTURE_SPIKERIGHT){

        //     respawn_player(player);

        // }

        if(check_collision(player, &current_tile) == true){

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
                player->jumps = 1;
            } else if (overlapLeft < overlapTop && overlapLeft < overlapBottom && overlapLeft < overlapRight && overlapLeft > 1.0f) {
                // Collision on left side of player
                player->Xpos = quadRight;                 
                player->Xvelocity = 0;                     // Stop horizontal movement
            } else if (overlapRight < overlapTop && overlapRight < overlapBottom && overlapRight < overlapLeft && overlapRight > 1.0f) {
                // Collision on right side of player
                player->Xpos = quadLeft - player->size;    
                player->Xvelocity = 0;                     // Stop horizontal movement
            }

            if((int)current_tile.v0.TexID >= TEXTURE_SPIKEUP && (int)current_tile.v0.TexID <= TEXTURE_SPIKERIGHT){
                respawn_player(player);
            } else if ((int)current_tile.v0.TexID == TEXTURE_FLAG) {
                return 1;
            }
            

        }


    }
    
    return 0;

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

void switch_scene(int scene, Game* game, Player* P1, Renderer* batch_renderer, Renderer* player_renderer, Sound sound_data[10], Quad level_data[4][16][12]){

    switch (scene){
    case LEVEL_TEST:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[0]), level_data[0]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 512.0f;
        P1->Ystart = 468.0f;
        respawn_player(P1);
        play_sound(&sound_data[SOUND_MENU]);    
        game->scene = LEVEL_TEST;
        break;
    case LEVEL_ONE:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[1]), level_data[1]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 1.0f;
        P1->Ystart = 65.0f;
        respawn_player(P1);
        game->scene = LEVEL_ONE;
        stop_sound(&sound_data[SOUND_MENU]);
        break;
    case LEVEL_TWO:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[2]), level_data[2]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 1.0f;
        P1->Ystart = 65.0f;
        respawn_player(P1);
        game->scene = LEVEL_TWO;

        break;
    
    default:
        break;
    }



}

void respawn_player(Player* player){
    player->Xvelocity = 0.0f;
    player->Yvelocity = 0.0f;
    player->Xpos = player->Xstart;
    player->Ypos = player->Ystart;


}


// 16w x 12h = 192 tiles

void generate_level_data(Quad stage_data[16][12], float stage_array[192]){


    unsigned int quadcount = 0;

    for (int i = 0; i < 12; i++){

        for (int j = 0; j < 16; j++){
            if(stage_array[quadcount] == 0){
                R_CreateQuad(&stage_data[j][i], -64.0f, -64.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            } else{
                R_CreateQuad(&stage_data[j][i], j * 64.0f, i * 64.0f, 64.0f, 64.0f, 0.0f, 0.0f, 0.0f, 1.0f, stage_array[quadcount]);
            }
            
            quadcount++;
        }
        


    }
    







}



void load_level_data(Quad level_data[4][16][12]){

    
    // Test level
    float testlevel_array[192] = {0.0f};

    for (int i = 0; i < 16; i++){
        testlevel_array[i] = (float)TEXTURE_GRASS;
    }

    testlevel_array[8] = (float)TEXTURE_NONE;
    testlevel_array[10] = (float)TEXTURE_SPIKEUP;


    testlevel_array[16] = (float)TEXTURE_GRASS;
    testlevel_array[32] = (float)TEXTURE_GRASS;
    testlevel_array[48] = (float)TEXTURE_GRASS;
    testlevel_array[64] = (float)TEXTURE_GRASS;

    testlevel_array[30] = (float)TEXTURE_FLAG;

    testlevel_array[31] = (float)TEXTURE_GRASS;
    testlevel_array[47] = (float)TEXTURE_GRASS;
    testlevel_array[63] = (float)TEXTURE_GRASS;
    testlevel_array[79] = (float)TEXTURE_GRASS;


    testlevel_array[87] = (float)TEXTURE_GRASS;
    testlevel_array[88] = (float)TEXTURE_GRASS;
    testlevel_array[89] = (float)TEXTURE_GRASS;

    generate_level_data(level_data[0],testlevel_array);


    //Level one

    float level1_array[192] = {0.0f};

    for (int i = 0; i < 9; i++){
        level1_array[i] = (float)TEXTURE_GRASS;
    }
    for (int i = 1; i < 11; i++){
        level1_array[(i*16)+3] = (float)TEXTURE_GRASS;
    }
    for (int i = 3; i < 12; i++){
        level1_array[(i*16)+6] = (float)TEXTURE_GRASS;
    }
    for (int i = 3; i < 12; i++){
        level1_array[(i*16)+10] = (float)TEXTURE_GRASS;
    }
    for (int i = 3; i < 12; i++){
        level1_array[(i*16)+11] = (float)TEXTURE_GRASS;
    }
    
    level1_array[4] = (float)TEXTURE_SPIKEUP;
    level1_array[5] = (float)TEXTURE_SPIKEUP;

    // right stairs
    level1_array[18] = (float)TEXTURE_GRASS;
    level1_array[50] = (float)TEXTURE_GRASS;
    level1_array[82] = (float)TEXTURE_GRASS;
    level1_array[114] = (float)TEXTURE_GRASS;
    level1_array[146] = (float)TEXTURE_GRASS;
  


    //left stairs
    level1_array[32] = (float)TEXTURE_GRASS;
    level1_array[64] = (float)TEXTURE_GRASS;
    level1_array[96] = (float)TEXTURE_GRASS;
    level1_array[128] = (float)TEXTURE_GRASS;
    level1_array[160] = (float)TEXTURE_GRASS;


    // deathfall landing

    level1_array[22] = (float)TEXTURE_GRASS;
    level1_array[23] = (float)TEXTURE_GRASS;
    level1_array[24] = (float)TEXTURE_GRASS;
    
    // final jump landing
    level1_array[28] = (float)TEXTURE_GRASS;
    level1_array[29] = (float)TEXTURE_GRASS;
    level1_array[30] = (float)TEXTURE_GRASS;
    level1_array[31] = (float)TEXTURE_GRASS;

    //win flag
    level1_array[47] = (float)TEXTURE_FLAG;

    generate_level_data(level_data[1],level1_array);


    // level two

    float level2_array[192] = {0.0f};

        for (int i = 0; i < 16; i++){
        level2_array[i] = (float)TEXTURE_GRASS;
    }

    generate_level_data(level_data[2], level2_array);

}
