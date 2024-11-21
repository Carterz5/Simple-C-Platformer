#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include "renderer.h"
#include "shader.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"
#include "texture.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"
#include "graphics.h"
#include "game.h"
#include "menus.h"
#include "audio.h"



// ----TODO----
//win screen
//add more levels
//moving blade
//music
//sound effects
// ---ART---
//add more spike textures.
//get rid of placeholder art

//---DONE---
//kill block
//main menu
//win block
//fix bouncy bug
//sound
//options
//switch scene function
//compile for windows


int main(void){
    Game* game = init_game();

    GLFWwindow* glfwwindow = init_opengl(game);

    Nuklear_window* nkwindow = NK_init(glfwwindow);


    Renderer* player_renderer = Create_Player_Renderer("../shaders/Batch.glsl");;
    Renderer* batch_renderer = Create_Batch_Renderer("../shaders/Batch.glsl", 1000);


    Player* P1 = init_player(1.3f, 8.0f, 10.0f, 32.0f, 9.0f, 0.5f, 1.0f, (float)TEXTURE_PLAYER);

    VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &P1->quad);

    Quad main_menu;
    R_CreateQuad(&main_menu, 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_MAINMENU);
    VB_AddToDynamic(&player_renderer->vb, sizeof(Quad), &main_menu);

    
    alutInit(0,0);
    Sound sound_data[10];
    load_sounds(sound_data);

    Quad level_data[4][16][12];


    load_level_data(level_data);

    VB_AddToDynamic(&batch_renderer->vb, sizeof(level_data[0]), level_data[0]);

    load_textures(&batch_renderer->shader, &player_renderer->shader);
   
    // glfwSetWindowUserPointer(glfwwindow, &game.inputs);

    VA_Unbind();
    SH_Unbind();
    VB_Unbind();
    IB_Unbind();
    GLCall(glfwSwapInterval(1));

    const int FPS_SAMPLES = 100;
    double fpsSum = 0.0;
    int fpsCount = 0;
    int level_flag = 0;
    const double PHYSICS_TIME_STEP = 1.0 / 60.0;
    double previousTime = glfwGetTime();
    double accumulator = 0.0;
    int window_width, window_height;
    /* Loop until the user closes the window */
    glClearColor(nkwindow->bg.r, nkwindow->bg.g, nkwindow->bg.b, nkwindow->bg.a);
    while (!glfwWindowShouldClose(glfwwindow)){
        glfwGetWindowSize(glfwwindow, &window_width, &window_height);
        glViewport(0, 0, window_width, window_height);
        /* Render here */

        double currentTime = glfwGetTime();
        double frameTime = currentTime - previousTime;
        previousTime = currentTime;


        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        accumulator += frameTime;


        // Update FPS average
        double currentFPS = 1.0 / frameTime;
        fpsSum += currentFPS;
        fpsCount++;


        // Every 100 frames, output the average FPS and reset
        if (fpsCount == FPS_SAMPLES) {
            double averageFPS = fpsSum / FPS_SAMPLES;
            char title[256];
            snprintf(title, sizeof(title), "FPS: %.2f", averageFPS);
            glfwSetWindowTitle(glfwwindow, title);
            fpsSum = 0.0;
            fpsCount = 0;
        }

        if (game->scene != MAIN_MENU){

            while (accumulator >= PHYSICS_TIME_STEP) {
                process_inputs(P1, &game->inputs, sound_data);
                process_physics(P1);
                level_flag = process_collisions(P1, level_data[game->scene]);
                update_player_coords(P1);
                accumulator -= PHYSICS_TIME_STEP;
            }
            
            R_Draw(&batch_renderer->va, &batch_renderer->ib, &batch_renderer->shader);
            Draw_Player(player_renderer, P1);

        } else if (game->scene == MAIN_MENU) {
            process_inputs(P1, &game->inputs,sound_data);               
            R_Draw(&player_renderer->va, &player_renderer->ib, &player_renderer->shader);
        }
        

        if(game->inputs.OToggle == true){
            NK_Draw_Options(glfwwindow, nkwindow, sound_data);
        }
        if(game->inputs.F12Toggle == true){
            NK_Draw_Debug(glfwwindow, nkwindow, P1);
        }


        if (level_flag == 1){
            switch (game->scene) {
            case LEVEL_TEST:
                switch_scene(LEVEL_ONE, game, P1, batch_renderer, player_renderer, sound_data, level_data);
                break;
            case LEVEL_ONE:
                switch_scene(LEVEL_TWO, game, P1, batch_renderer, player_renderer, sound_data, level_data);
                break;
            case LEVEL_TWO:
                switch_scene(LEVEL_THREE, game, P1, batch_renderer, player_renderer, sound_data, level_data);
                break;
            case LEVEL_THREE:
                switch_scene(LEVEL_FOUR, game, P1, batch_renderer, player_renderer, sound_data, level_data);
                break;
            
            default:
                break;
            }


        }

        if (game->inputs.F1State > GLFW_RELEASE){
            switch_scene(LEVEL_TEST, game, P1, batch_renderer, player_renderer, sound_data, level_data);
            
        } else if (game->inputs.F2State > GLFW_RELEASE || game->inputs.EnterState > GLFW_RELEASE){
            switch_scene(LEVEL_ONE, game, P1, batch_renderer, player_renderer, sound_data, level_data);

        }
        
        

        /* Swap front and back buffers */
        glfwSwapBuffers(glfwwindow);
        R_Clear();
        glfwPollEvents();

        
        level_flag = 0;

    }

    SH_Destruct(&batch_renderer->shader);
    IB_Destruct(&batch_renderer->ib);
    VB_Destruct(&batch_renderer->vb);
    VA_Destruct(&batch_renderer->va);
    // TX_Destruct(&Wtexture);
    // TX_Destruct(&Ctexture);
    // TX_Destruct(&blocktex);

    NK_Destruct(nkwindow);
    free(game);
    free(batch_renderer);
    free(player_renderer);
    free(P1);

    glfwTerminate();
    return 0;
}


