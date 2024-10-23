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




#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024


int main(void){

    GLFWwindow* window = init_opengl();



    unsigned int BatchIndices[1500];
    IB_Populate(250, BatchIndices, 1500);


    // BATCHING
    VertexArray vabatch;
    VA_Construct(&vabatch);

    VertexBuffer vbbatch;
    VB_Construct_Batch(sizeof(Vertex) * 1000, &vbbatch);

    Quad tiles[3];




    R_CreateQuad(&tiles[0], 448.0f, 384.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 3.0f);
    R_CreateQuad(&tiles[1], 512.0f, 384.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 3.0f);
    R_CreateQuad(&tiles[2], 576.0f, 384.0f, 64.0f, 0.5f, 0.0f, 0.0f, 1.0f, 3.0f);



    VB_AddToBatch(&vbbatch, sizeof(tiles), tiles);



    VertexBufferLayout vblbatch;
    VBL_Construct(&vblbatch);
    VBL_Pushfloat(3, &vblbatch);
    VBL_Pushfloat(4, &vblbatch);
    VBL_Pushfloat(2, &vblbatch);
    VBL_Pushfloat(1, &vblbatch);
    VA_AddBuffer(&vbbatch, &vblbatch, &vabatch);
    
    IndexBuffer ibbatch;
    
    IB_Construct(BatchIndices, 1500, &ibbatch);


    Shader batchshader;
    SH_Construct(&batchshader,"../shaders/Batch.glsl");
    SH_Bind(&batchshader);


    

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
    SH_SetUniform1iv(&batchshader, "u_Textures", 4, samplers);

    mat4 bmvp;
    mat4 batchproj;
    mat4 batchview;
    mat4 batchmodel;
    mat4 batchtemp;
    vec3 bvtranslate = {0.0f, 0.0f, 0.0f};
    vec3 bmtranslate = {0.0f, 0.0f, 0.0f};
    glm_mat4_identity(batchmodel);
    glm_mat4_identity(batchview);

    glm_ortho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f, batchproj);
    glm_translate(batchview, bvtranslate);
    glm_translate(batchmodel, bmtranslate);

    glm_mat4_mul(batchproj, batchview, batchtemp);
    glm_mat4_mul(batchtemp, batchmodel, bmvp);

    SH_SetUniformMat4f(&batchshader, "u_MVP", bmvp);
    


    VA_Unbind();
    SH_Unbind();
    VB_Unbind();
    IB_Unbind();
    GLCall(glfwSwapInterval(1));
    double lasttime = glfwGetTime();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */


        //Different way to limit framerate than Cherno. glfwSwapInterval was not working on virtual machine.
        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {

        };
        lasttime += 1.0/TARGET_FPS;

        glfwPollEvents();


        SH_Bind(&batchshader);
        R_Draw(&vabatch, &ibbatch, &batchshader);




        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        
    }

    //GLCall(glDeleteProgram(shader));
    SH_Destruct(&batchshader);
    IB_Destruct(&ibbatch);
    VB_Destruct(&vbbatch);
    VA_Destruct(&vabatch);
    TX_Destruct(&Wtexture);
    TX_Destruct(&Ctexture);



    glfwTerminate();
    return 0;
}


