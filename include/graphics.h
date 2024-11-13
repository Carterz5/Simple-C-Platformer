#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

#include "cglm/cglm.h"
#include "cglm/mat4.h"


typedef struct Player Player;
typedef struct Game Game;

typedef struct Renderer{
    VertexArray va;
    VertexBuffer vb;
    VertexBufferLayout vbl;
    IndexBuffer ib;
    Shader shader;
    mat4 mvp, proj, view, model;
} Renderer;




GLFWwindow* init_opengl(Game* game);
Renderer* Create_Batch_Renderer(char* shaderpath, unsigned int max_vertices);
Renderer* Create_Player_Renderer(char* shaderpath);
void Draw_Player(Renderer* renderer, Player* player);
void load_textures(Shader* batchshader, Shader* playershader);


#endif