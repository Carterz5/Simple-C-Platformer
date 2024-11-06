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



// store level data as array 2D
// add sound
// 
//
//


int main(void){
    Game game;
    init_game(&game);
    GLFWwindow* glfwwindow = init_opengl(&game);

    Nuklear_window* nkwindow = NK_init(glfwwindow);


    Renderer player_renderer;
    Renderer batch_renderer;
    Create_Batch_Renderer(&batch_renderer,"../shaders/Batch.glsl", 1000);
    Create_Player_Renderer(&player_renderer,"../shaders/Batch.glsl");


    Player P1;
    init_player(&P1, 2.5f, 10.0f, 10.0f, 32.0f, 15.0f, 2.0f, 2.0f, 1.0f);
    VB_AddToDynamic(&player_renderer.vb, sizeof(Quad), &P1.quad);

    Quad testlevel_data[16][12];
    float testlevel_array[192] = {0.0f};

    for (int i = 0; i < 16; i++){
        testlevel_array[i] = 2.0f;
    }
    
    

    testlevel_array[16] = 2.0f;
    testlevel_array[32] = 2.0f;
    testlevel_array[48] = 2.0f;
    testlevel_array[64] = 2.0f;

    testlevel_array[31] = 2.0f;
    testlevel_array[47] = 2.0f;
    testlevel_array[63] = 2.0f;
    testlevel_array[79] = 2.0f;


    testlevel_array[87] = 2.0f;
    testlevel_array[88] = 2.0f;
    testlevel_array[89] = 2.0f;
    
    generate_level_data(testlevel_data, testlevel_array);
    // Quad tiles[16];

    // R_CreateQuad(&tiles[0], 0.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[1], 64.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[2], 128.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[3], 192.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[4], 256.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[5], 320.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[6], 384.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[7], 448.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[8], 512.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[9], 576.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[10], 640.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[11], 704.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[12], 768.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[13], 832.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[14], 896.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);
    // R_CreateQuad(&tiles[15], 960.0f, 0.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 2.0f);

    VB_AddToBatch(&batch_renderer.vb, sizeof(testlevel_data), testlevel_data);

    load_textures(&batch_renderer.shader, &player_renderer.shader);
   
    // glfwSetWindowUserPointer(glfwwindow, &game.inputs);

    VA_Unbind();
    SH_Unbind();
    VB_Unbind();
    IB_Unbind();
    GLCall(glfwSwapInterval(1));

    const int FPS_SAMPLES = 100;
    double fpsSum = 0.0;
    int fpsCount = 0;

    const double PHYSICS_TIME_STEP = 1.0 / 60.0;
    double previousTime = glfwGetTime();
    double accumulator = 0.0;
    /* Loop until the user closes the window */
    glClearColor(nkwindow->bg.r, nkwindow->bg.g, nkwindow->bg.b, nkwindow->bg.a);
    while (!glfwWindowShouldClose(glfwwindow)){
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


        // Run fixed timestep updates
        while (accumulator >= PHYSICS_TIME_STEP) {
            //updatePhysics(PHYSICS_TIME_STEP);  // Update physics at fixed rate

            process_inputs(&P1, &game.inputs);
            process_physics(&P1);
            process_collisions(&P1, testlevel_data);
            update_player_coords(&P1);
            accumulator -= PHYSICS_TIME_STEP;
        }

     
    
        R_Draw(&batch_renderer.va, &batch_renderer.ib, &batch_renderer.shader);
        Draw_Player(&player_renderer, &P1);

        if(game.inputs.F12Toggle == true){
            NK_Draw(glfwwindow, nkwindow, &P1);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(glfwwindow);
        R_Clear();
        glfwPollEvents();




    }

    SH_Destruct(&batch_renderer.shader);
    IB_Destruct(&batch_renderer.ib);
    VB_Destruct(&batch_renderer.vb);
    VA_Destruct(&batch_renderer.va);
    // TX_Destruct(&Wtexture);
    // TX_Destruct(&Ctexture);
    // TX_Destruct(&blocktex);

    NK_Destruct(nkwindow);


    glfwTerminate();
    return 0;
}


