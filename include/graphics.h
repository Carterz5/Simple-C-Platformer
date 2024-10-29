#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "game.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"


typedef struct Renderer{
    VertexArray va;
    VertexBuffer vb;
    VertexBufferLayout vbl;
    IndexBuffer ib;
    Shader shader;
    mat4 mvp, proj, view, model;
} Renderer;




GLFWwindow* init_opengl();
void Create_Batch_Renderer(Renderer* renderer, char* shaderpath, unsigned int max_vertices);
void Create_Player_Renderer(Renderer* renderer, char* shaderpath);
void Draw_Player(Renderer* renderer, Player* player);



#endif