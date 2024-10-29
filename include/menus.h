#ifndef MENUS_H
#define MENUS_H
#include "graphics.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

typedef struct Nuklear_window {
    struct nk_glfw glfw;
    int width, height;
    struct nk_context* ctx;
    struct nk_colorf bg;

} Nuklear_window;

Nuklear_window* NK_init(GLFWwindow* glfwwindow);
void NK_Destruct(Nuklear_window* nkwindow);
void NK_Draw(GLFWwindow* glfwwindow, Nuklear_window* nkwindow, Player* player);


#endif