#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "audio.h"

#include "cglm/cglm.h"
#include "cglm/mat4.h"


typedef struct Player Player;
typedef struct Game Game;

enum TextureType {
    TEXTURE_NONE = 0,
    TEXTURE_PLAYER = 1,
    TEXTURE_SPIKEUP = 2,
    TEXTURE_SPIKEDOWN = 3,
    TEXTURE_SPIKELEFT = 4,
    TEXTURE_SPIKERIGHT = 5,
    TEXTURE_GRASS = 10,
    TEXTURE_DIRT = 11,
    TEXTURE_ICE = 12,
    TEXTURE_SNOW = 13,
    TEXTURE_SPACE = 14,
    TEXTURE_SPACEHAZARD = 15,
    TEXTURE_SPACEHAZARDDOWN = 16,
    TEXTURE_FLAG = 19,
    TEXTURE_MAINMENU = 20,
    TEXTURE_LEVELONE = 21,
    TEXTURE_LEVELTWO = 22,
    TEXTURE_LEVELTHREE = 23,
    TEXTURE_WIN = 24


};

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
void Draw_Player(Renderer* renderer, Player* player, Sound sound_data[10]);
void load_textures(Shader* batchshader, Shader* playershader, Shader* backgroundshader);
void Draw_Background(Renderer* renderer, Game* game, Quad backgrounds[5]);
void load_backgrounds(Quad backgrounds[5]);


#endif