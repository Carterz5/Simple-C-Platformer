#include "menus.h"
#include "renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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





Nuklear_window* NK_init(GLFWwindow* glfwwindow){
    Nuklear_window* nkwindow = malloc(sizeof(Nuklear_window));

    glfwGetWindowSize(glfwwindow, &nkwindow->width, &nkwindow->height);


    nkwindow->ctx = nk_glfw3_init(&nkwindow->glfw, glfwwindow, NK_GLFW3_INSTALL_CALLBACKS);
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&nkwindow->glfw, &atlas);
    nk_glfw3_font_stash_end(&nkwindow->glfw);
    }
    nkwindow->bg.r = 0.10f, nkwindow->bg.g = 0.18f, nkwindow->bg.b = 0.24f, nkwindow->bg.a = 1.0f;

    return nkwindow;
}

void NK_Destruct(Nuklear_window* nkwindow){

    nk_glfw3_shutdown(&nkwindow->glfw);

    free(nkwindow);
}


void NK_Draw(GLFWwindow* glfwwindow, Nuklear_window* nkwindow){

    nk_glfw3_new_frame(&nkwindow->glfw);

    if (nk_begin(nkwindow->ctx, "Test", nk_rect(50, 50, 230, 300),
        NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
        NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {

        nk_layout_row_dynamic(nkwindow->ctx, 20, 1);
        nk_label(nkwindow->ctx, "Lamps", NK_TEXT_LEFT);

    }
    nk_end(nkwindow->ctx);
    R_Clear();
    glClearColor(nkwindow->bg.r, nkwindow->bg.g, nkwindow->bg.b, nkwindow->bg.a);



    glfwGetWindowSize(glfwwindow, &nkwindow->width, &nkwindow->height);
    glViewport(0, 0, nkwindow->width, nkwindow->height);
    /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
        * with blending, scissor, face culling, depth test and viewport and
        * defaults everything back into a default state.
        * Make sure to either a.) save and restore or b.) reset your own state after
        * rendering the UI. */
    nk_glfw3_render(&nkwindow->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

}