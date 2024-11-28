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
    case GLFW_KEY_G:
        callback_keys.GState = action;
        break;
    case GLFW_KEY_ENTER:
        callback_keys.EnterState = action;
        break;
    case GLFW_KEY_ESCAPE:
        callback_keys.EscapeState = action;
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
    case GLFW_KEY_F3:
        callback_keys.F3State = action;
        break;
    case GLFW_KEY_F4:
        callback_keys.F4State = action;
        break;
    case GLFW_KEY_F5:
        callback_keys.F5State = action;
        break;
    case GLFW_KEY_F6:
        callback_keys.F6State = action;
        break;
    default:
        break;
    }

    

}


void process_inputs(Player* player, Inputs* inputs, Sound sound_data[10], Game* game, Renderer* player_renderer){


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

    if(callback_keys.GState == GLFW_RELEASE && callback_keys.LastGState == GLFW_PRESS && callback_keys.GToggle == false && (game->scene == SCENE_LEVEL_THREE || game->scene == SCENE_LEVEL_TEST)){
        callback_keys.GToggle = true;
        player->gravity = -0.5f;
        player->quad.v0.TexCoords[0] = 0.0f;
        player->quad.v0.TexCoords[1] = 1.0f;
        player->quad.v1.TexCoords[0] = 1.0f;
        player->quad.v1.TexCoords[1] = 1.0f;
        player->quad.v2.TexCoords[0] = 1.0f;
        player->quad.v2.TexCoords[1] = 0.0f;
        player->quad.v3.TexCoords[0] = 0.0f;
        player->quad.v3.TexCoords[1] = 0.0f;
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &player->quad);
    } else if (callback_keys.GState == GLFW_RELEASE && callback_keys.LastGState == GLFW_PRESS && callback_keys.GToggle == true && (game->scene == SCENE_LEVEL_THREE || game->scene == SCENE_LEVEL_TEST)){
        callback_keys.GToggle = false;
        player->gravity = 0.5f;
        player->quad.v0.TexCoords[0] = 0.0f;
        player->quad.v0.TexCoords[1] = 0.0f;
        player->quad.v1.TexCoords[0] = 1.0f;
        player->quad.v1.TexCoords[1] = 0.0f;
        player->quad.v2.TexCoords[0] = 1.0f;
        player->quad.v2.TexCoords[1] = 1.0f;
        player->quad.v3.TexCoords[0] = 0.0f;
        player->quad.v3.TexCoords[1] = 1.0f;
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &player->quad);
    }



    if (callback_keys.RightState > GLFW_RELEASE){
       player->Xvelocity += player->acceleration;
    }
    if (callback_keys.LeftState > GLFW_RELEASE){
       player->Xvelocity -= player->acceleration;
    }
    if ((callback_keys.UpState == GLFW_PRESS || callback_keys.SpaceState == GLFW_PRESS) && jump_cooldown == 0 && player->jumps > 0){
        switch (callback_keys.GToggle){
        case true:
            player->Yvelocity = -player->jump_height;
            break;
        case false:
            player->Yvelocity = player->jump_height;
            break;
        
        default:
            break;
        }
        player->jumps--;
        jump_cooldown = 10;
        play_sound(&sound_data[SOUND_JUMP]);
    }
    
    if(jump_cooldown > 0){
        jump_cooldown--;
    }

   

    callback_keys.LastF12State = callback_keys.F12State;
    callback_keys.LastOState = callback_keys.OState;
    callback_keys.LastGState = callback_keys.GState;

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
    game->scene = SCENE_MAIN_MENU;
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
    } else if (player->Yvelocity > player->maxfall){
        player->Yvelocity = player->maxfall;
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

int process_collisions(Player* player, Quad tiles[16][12], Sound sound_data[10]){

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
                player->Yvelocity = 0;
                if (callback_keys.GToggle == true) {
                    player->jumps = 1;
                }
    
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
                play_sound(&sound_data[SOUND_DEATH]);
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
    case SCENE_LEVEL_TEST:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[0]), level_data[0]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 512.0f;
        P1->Ystart = 468.0f;
        P1->friction = 0.95f;
        P1->Xvelocity = 0.0f;
        P1->Yvelocity = 0.0f;
        respawn_player(P1);
            
        game->scene = SCENE_LEVEL_TEST;
        break;
    case SCENE_LEVEL_ONE:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[1]), level_data[1]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 1.0f;
        P1->Ystart = 641.0f;
        P1->friction = 0.95f;
        P1->Xvelocity = 0.0f;
        P1->Yvelocity = 0.0f;
        respawn_player(P1);
        play_sound(&sound_data[SOUND_MUSIC]);
        game->scene = SCENE_LEVEL_ONE;
        break;
    case SCENE_LEVEL_TWO:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[2]), level_data[2]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 1.0f;
        P1->Ystart = 65.0f;
        P1->friction = 0.1f;
        P1->Xvelocity = 0.0f;
        P1->Yvelocity = 0.0f;
        respawn_player(P1);
        game->scene = SCENE_LEVEL_TWO;

        break;
    case SCENE_LEVEL_THREE:
        VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[3]), level_data[3]);
        VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);
        P1->Xstart = 1.0f;
        P1->Ystart = 65.0f;
        P1->friction = 0.95f;
        P1->Xvelocity = 0.0f;
        P1->Yvelocity = 0.0f;
        respawn_player(P1);
        game->scene = SCENE_LEVEL_THREE;

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


    //------Level one-------

    float level1_array[192] = {0.0f};


    //top left start
    for (int i = 0; i < 7; i++) {
        level1_array[113+i] = (float)TEXTURE_GRASS;
    }
    level1_array[112] = (float)TEXTURE_DIRT;
    level1_array[120] = (float)TEXTURE_DIRT;
    level1_array[121] = (float)TEXTURE_DIRT;
    level1_array[128] = (float)TEXTURE_GRASS;
    level1_array[146] = (float)TEXTURE_GRASS;
    level1_array[147] = (float)TEXTURE_DIRT;
    level1_array[163] = (float)TEXTURE_GRASS;


    level1_array[136] = (float)TEXTURE_DIRT;
    level1_array[152] = (float)TEXTURE_DIRT;
    level1_array[168] = (float)TEXTURE_GRASS;

    //top right jump
    level1_array[153] = (float)TEXTURE_GRASS;
    level1_array[137] = (float)TEXTURE_DIRT;
    level1_array[121] = (float)TEXTURE_DIRT;

    level1_array[122] = (float)TEXTURE_SPIKEUP;
    level1_array[123] = (float)TEXTURE_SPIKEUP;
    level1_array[124] = (float)TEXTURE_SPIKEUP;

    level1_array[125] = (float)TEXTURE_DIRT;
    level1_array[141] = (float)TEXTURE_DIRT;
    level1_array[157] = (float)TEXTURE_GRASS;

    //tunnel
    for (int i = 0; i < 14; i++) {
        level1_array[82+i] = (float)TEXTURE_GRASS;
    }

    //death drop

    level1_array[2] = (float)TEXTURE_DIRT;
    level1_array[18] = (float)TEXTURE_GRASS;
    level1_array[19] = (float)TEXTURE_GRASS;
    level1_array[20] = (float)TEXTURE_GRASS;
    level1_array[21] = (float)TEXTURE_GRASS;
    level1_array[22] = (float)TEXTURE_GRASS;
    level1_array[23] = (float)TEXTURE_GRASS;
    level1_array[50] = (float)TEXTURE_DIRT;
    level1_array[66] = (float)TEXTURE_DIRT;



    
    // final jump
    level1_array[73] = (float)TEXTURE_DIRT;
    level1_array[74] = (float)TEXTURE_DIRT;


    level1_array[57] = (float)TEXTURE_SPIKEDOWN;
    level1_array[58] = (float)TEXTURE_SPIKEDOWN;


    level1_array[28] = (float)TEXTURE_GRASS;
    level1_array[29] = (float)TEXTURE_GRASS;
    level1_array[30] = (float)TEXTURE_GRASS;
    level1_array[31] = (float)TEXTURE_GRASS;

    //win flag
    level1_array[47] = (float)TEXTURE_FLAG;

    generate_level_data(level_data[1],level1_array);


    //------level two-----------


    //bottom floor
    float level2_array[192] = {0.0f};

        for (int i = 0; i < 3; i++){
        level2_array[i] = (float)TEXTURE_SNOW;
    }

    level2_array[5] = (float)TEXTURE_SNOW;
    level2_array[8] = (float)TEXTURE_SNOW;
    level2_array[11] = (float)TEXTURE_SNOW;
    level2_array[14] = (float)TEXTURE_SNOW;
    level2_array[15] = (float)TEXTURE_ICE;

    //right stairs
    level2_array[31] = (float)TEXTURE_SNOW;
    level2_array[63] = (float)TEXTURE_SNOW;
    level2_array[45] = (float)TEXTURE_SNOW;
    level2_array[77] = (float)TEXTURE_SNOW;


    //middle tunnel
    level2_array[76] = (float)TEXTURE_SNOW;
    level2_array[75] = (float)TEXTURE_SNOW;
    level2_array[74] = (float)TEXTURE_SNOW;
    level2_array[73] = (float)TEXTURE_SNOW;
    level2_array[72] = (float)TEXTURE_SNOW;
    level2_array[102] = (float)TEXTURE_SPIKEDOWN;
    level2_array[103] = (float)TEXTURE_SPIKEDOWN;

    level2_array[119] = (float)TEXTURE_ICE;
    level2_array[120] = (float)TEXTURE_ICE;
    level2_array[121] = (float)TEXTURE_ICE;
    level2_array[122] = (float)TEXTURE_ICE;
    level2_array[123] = (float)TEXTURE_SNOW;
    level2_array[124] = (float)TEXTURE_SNOW;
    level2_array[125] = (float)TEXTURE_SNOW;
    level2_array[126] = (float)TEXTURE_SNOW;

    level2_array[69] = (float)TEXTURE_SNOW;
    level2_array[68] = (float)TEXTURE_SNOW;
    level2_array[67] = (float)TEXTURE_SNOW;
    level2_array[66] = (float)TEXTURE_SNOW;
    level2_array[65] = (float)TEXTURE_SNOW;
    level2_array[64] = (float)TEXTURE_SNOW;

    //left spikewall
    level2_array[112] = (float)TEXTURE_SPIKERIGHT;
    level2_array[128] = (float)TEXTURE_ICE;
    level2_array[144] = (float)TEXTURE_ICE;
    level2_array[160] = (float)TEXTURE_ICE;
    level2_array[176] = (float)TEXTURE_ICE;

    level2_array[80] = (float)TEXTURE_ICE;
    level2_array[81] = (float)TEXTURE_ICE;
    level2_array[82] = (float)TEXTURE_SNOW;
    level2_array[96] = (float)TEXTURE_ICE;
    level2_array[97] = (float)TEXTURE_SNOW;


    //top middle
    level2_array[115] = (float)TEXTURE_SNOW;
    level2_array[116] = (float)TEXTURE_SNOW;
    level2_array[117] = (float)TEXTURE_SNOW;
    level2_array[118] = (float)TEXTURE_SNOW;

    level2_array[135] = (float)TEXTURE_SNOW;
    level2_array[136] = (float)TEXTURE_SNOW;
    level2_array[137] = (float)TEXTURE_ICE;
    level2_array[153] = (float)TEXTURE_SNOW;


    //final drop
    level2_array[191] = (float)TEXTURE_SPIKELEFT;
    level2_array[175] = (float)TEXTURE_SPIKELEFT;

    level2_array[154] = (float)TEXTURE_SNOW;
    level2_array[155] = (float)TEXTURE_SNOW;
    level2_array[156] = (float)TEXTURE_SNOW;
    level2_array[157] = (float)TEXTURE_SNOW;

    level2_array[159] = (float)TEXTURE_ICE;
    level2_array[143] = (float)TEXTURE_ICE;
    level2_array[127] = (float)TEXTURE_ICE;




    level2_array[122] = (float)TEXTURE_SNOW;
    level2_array[138] = (float)TEXTURE_FLAG;

    generate_level_data(level_data[2], level2_array);



    //------level THREE-----------

    float level3_array[192] = {0.0f};

    //start
    level3_array[0] = (float)TEXTURE_SPACEHAZARD;
    level3_array[1] = (float)TEXTURE_SPACEHAZARD;
    level3_array[2] = (float)TEXTURE_SPACEHAZARD;
    level3_array[3] = (float)TEXTURE_SPACEHAZARD;
    level3_array[4] = (float)TEXTURE_SPACE;

    //spike up tunnel
    for (size_t i = 3; i < 11; i++) {
        level3_array[i*16] = (float)TEXTURE_SPIKERIGHT;
    }
    for (size_t i = 1; i < 8; i++) {
        level3_array[i*16+4] = (float)TEXTURE_SPIKELEFT;
    }
    level3_array[32] = (float)TEXTURE_SPIKEDOWN;
    level3_array[33] = (float)TEXTURE_SPIKEDOWN;
    level3_array[34] = (float)TEXTURE_SPIKEDOWN;

    level3_array[176] = (float)TEXTURE_SPACE;
    level3_array[177] = (float)TEXTURE_SPACEHAZARDDOWN;
    level3_array[178] = (float)TEXTURE_SPACEHAZARDDOWN;
    level3_array[179] = (float)TEXTURE_SPACEHAZARDDOWN;
    level3_array[180] = (float)TEXTURE_SPACEHAZARDDOWN;


    level3_array[132] = (float)TEXTURE_SPACEHAZARD;
    level3_array[131] = (float)TEXTURE_SPIKEDOWN;
    level3_array[130] = (float)TEXTURE_SPIKEDOWN;

    //spike right up tunnel
    for (int i = 181; i < 192; i++){
        level3_array[i] = (float)TEXTURE_SPIKEDOWN;
    }
    for (int i = 117; i < 126; i++){
        level3_array[i] = (float)TEXTURE_SPIKEUP;
    }
    
    level3_array[61] = (float)TEXTURE_SPACEHAZARD;
    level3_array[62] = (float)TEXTURE_SPACEHAZARD;
    level3_array[63] = (float)TEXTURE_SPACEHAZARD;

    //upsidedown jump
    for (int i = 101; i < 110; i++){
        level3_array[i] = (float)TEXTURE_SPACEHAZARDDOWN;
    }

    level3_array[103] = (float)TEXTURE_SPIKEDOWN;
    level3_array[106] = (float)TEXTURE_SPIKEDOWN;
    

    level3_array[60] = (float)TEXTURE_SPIKEUP;
    level3_array[59] = (float)TEXTURE_SPIKEUP;
    level3_array[58] = (float)TEXTURE_SPIKEUP;
    level3_array[57] = (float)TEXTURE_SPIKEUP;
    level3_array[56] = (float)TEXTURE_SPIKEUP;
    level3_array[55] = (float)TEXTURE_SPIKEUP;

    //final area

    level3_array[5] = (float)TEXTURE_SPACEHAZARD;
    level3_array[6] = (float)TEXTURE_SPACEHAZARD;
    level3_array[7] = (float)TEXTURE_SPACEHAZARD;
    level3_array[8] = (float)TEXTURE_SPACEHAZARD;
    level3_array[9] = (float)TEXTURE_SPACEHAZARD;
    level3_array[10] = (float)TEXTURE_SPACEHAZARD;
    level3_array[11] = (float)TEXTURE_SPACEHAZARD;
    level3_array[12] = (float)TEXTURE_SPACEHAZARD;
    level3_array[13] = (float)TEXTURE_SPACEHAZARD;
    level3_array[14] = (float)TEXTURE_SPACEHAZARD;
    level3_array[15] = (float)TEXTURE_SPACEHAZARD;

    level3_array[31] = (float)TEXTURE_FLAG;



    generate_level_data(level_data[3], level3_array);

}
