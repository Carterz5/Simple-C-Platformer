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


// movement prototype
// store level data as array 2D
// add sound
// add collision
//
//
//


int main(void){

    GLFWwindow* glfwwindow = init_opengl();

    Nuklear_window* nkwindow = NK_init(glfwwindow);
    Player P1;
    init_player(&P1, 5.0f);

    Renderer player_renderer;
    Renderer batch_renderer;
    Create_Batch_Renderer(&batch_renderer,"../shaders/Batch.glsl", 1000);
    Create_Player_Renderer(&player_renderer,"../shaders/Batch.glsl");

    Quad tiles[3];

    R_CreateQuad(&tiles[0], 448.0f, 384.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 3.0f);
    R_CreateQuad(&tiles[1], 512.0f, 384.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 3.0f);
    R_CreateQuad(&tiles[2], 576.0f, 384.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 3.0f);

    VB_AddToBatch(&batch_renderer.vb, sizeof(tiles), tiles);


    Texture Wtexture;
    Texture Ctexture;
    Texture blocktex;
    TX_Construct("../assets/textures/testtile.png", &blocktex);
    TX_Construct("../assets/textures/W.png", &Wtexture);
    TX_Construct("../assets/textures/C.png", &Ctexture);
    TX_Bind(1, &Wtexture);
    TX_Bind(2, &Ctexture);
    TX_Bind(3, &blocktex);
    //int samplers[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    int samplers[4] = {0, 1, 2, 3};
    SH_SetUniform1iv(&batch_renderer.shader, "u_Textures", 4, samplers);
    


    VA_Unbind();
    SH_Unbind();
    VB_Unbind();
    IB_Unbind();
    GLCall(glfwSwapInterval(1));
    double lasttime = glfwGetTime();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(glfwwindow))
    {
        /* Render here */


        //Different way to limit framerate than Cherno. glfwSwapInterval was not working on virtual machine.
        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {

        };
        lasttime += 1.0/TARGET_FPS;


        NK_Draw(glfwwindow, nkwindow, &P1);

        


        SH_Bind(&batch_renderer.shader);
        R_Draw(&batch_renderer.va, &batch_renderer.ib, &batch_renderer.shader);
        Draw_Player(&player_renderer, &P1);


        /* Swap front and back buffers */
        glfwSwapBuffers(glfwwindow);

        glfwPollEvents();
        
        process_inputs(&P1);
        process_physics(&P1);


    }

    SH_Destruct(&batch_renderer.shader);
    IB_Destruct(&batch_renderer.ib);
    VB_Destruct(&batch_renderer.vb);
    VA_Destruct(&batch_renderer.va);
    TX_Destruct(&Wtexture);
    TX_Destruct(&Ctexture);

    NK_Destruct(nkwindow);


    glfwTerminate();
    return 0;
}


