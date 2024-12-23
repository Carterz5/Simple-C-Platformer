#include "graphics.h"
#include "game.h"







GLFWwindow* init_opengl(Game* game){

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        printf("Error initializing glfw");
        return NULL;
    }



    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, "Platformer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Error creating glfw window");
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK){
        printf("Error!\n");
    }
    GLCall(const unsigned char *glversion = glGetString(GL_VERSION));
    printf("GL version is |%s|\n", glversion);

    glfwSetKeyCallback(window, key_callback);
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    return window;
}



Renderer* Create_Batch_Renderer(char* shaderpath, unsigned int max_vertices){
    Renderer* renderer = malloc(sizeof(Renderer));
    
    unsigned int ibcount = (max_vertices/4)*6;

    unsigned int* IBarray = NULL;
    IBarray = IB_Populate_Dynamic(ibcount, IBarray);

    VA_Construct(&renderer->va);
    VB_Construct_Batch(sizeof(Vertex) * max_vertices, &renderer->vb);
    VBL_Construct(&renderer->vbl);
   
    IB_Construct(IBarray, ibcount, &renderer->ib);

    SH_Construct(&renderer->shader, shaderpath);

    VBL_Pushfloat(3, &renderer->vbl);
    VBL_Pushfloat(4, &renderer->vbl);
    VBL_Pushfloat(2, &renderer->vbl);
    VBL_Pushfloat(1, &renderer->vbl);
    VA_AddBuffer(&renderer->vb, &renderer->vbl, &renderer->va);

    mat4 temp;

    glm_mat4_identity(renderer->model);
    glm_mat4_identity(renderer->view);

    glm_ortho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f, renderer->proj);

    glm_mat4_mul(renderer->proj, renderer->view, temp);
    glm_mat4_mul(temp, renderer->model, renderer->mvp);

    SH_SetUniformMat4f(&renderer->shader, "u_MVP", renderer->mvp);

    return renderer;



}

Renderer* Create_Player_Renderer(char* shaderpath){
    Renderer* renderer = malloc(sizeof(Renderer));

    unsigned int* IBarray = NULL;
    IBarray = IB_Populate_Dynamic(6, IBarray);

    VA_Construct(&renderer->va);
    VB_Construct_Dynamic(sizeof(Vertex) * 4, &renderer->vb);
    VBL_Construct(&renderer->vbl);
   
    IB_Construct(IBarray, 6, &renderer->ib);

    SH_Construct(&renderer->shader, shaderpath);

    VBL_Pushfloat(3, &renderer->vbl);
    VBL_Pushfloat(4, &renderer->vbl);
    VBL_Pushfloat(2, &renderer->vbl);
    VBL_Pushfloat(1, &renderer->vbl);
    VA_AddBuffer(&renderer->vb, &renderer->vbl, &renderer->va);

    mat4 temp;

    glm_mat4_identity(renderer->model);
    glm_mat4_identity(renderer->view);

    glm_ortho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f, renderer->proj);

    glm_mat4_mul(renderer->proj, renderer->view, temp);
    glm_mat4_mul(temp, renderer->model, renderer->mvp);

    SH_SetUniformMat4f(&renderer->shader, "u_MVP", renderer->mvp);

    return renderer;

}

void load_backgrounds(Quad backgrounds[5]){

    R_CreateQuad(&backgrounds[0], 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_MAINMENU);
    R_CreateQuad(&backgrounds[1], 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_LEVELONE);
    R_CreateQuad(&backgrounds[2], 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_LEVELTWO);
    R_CreateQuad(&backgrounds[3], 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_LEVELTHREE);
    R_CreateQuad(&backgrounds[4], 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_WIN);
    R_CreateQuad(&backgrounds[5], 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)TEXTURE_LEVELONE);




}

void Draw_Background(Renderer* renderer, Game* game, Quad backgrounds[5]){

    switch (game->scene){
    case SCENE_MAIN_MENU:
        VB_AddToDynamic(&renderer->vb, sizeof(Quad), &backgrounds[0]);
        break;
    case SCENE_LEVEL_ONE:
        VB_AddToDynamic(&renderer->vb, sizeof(Quad), &backgrounds[1]);
        break;
    case SCENE_LEVEL_TWO:
        VB_AddToDynamic(&renderer->vb, sizeof(Quad), &backgrounds[2]);
        break;
    case SCENE_LEVEL_THREE:
        VB_AddToDynamic(&renderer->vb, sizeof(Quad), &backgrounds[3]);
        break;
    case SCENE_WIN:
        VB_AddToDynamic(&renderer->vb, sizeof(Quad), &backgrounds[4]);
        break;
    case SCENE_LEVEL_TEST:
        VB_AddToDynamic(&renderer->vb, sizeof(Quad), &backgrounds[5]);
        break;
    
    default:
        break;
    }

    R_Draw(&renderer->va, &renderer->ib, &renderer->shader);

}

void Draw_Player(Renderer* renderer, Player* player, Sound sound_data[3]){

    if(player->Xpos > 992.0f){
        player->Xpos = 992.0f;
    }
    if(player->Xpos < 0.0f){
        player->Xpos = 0.0f;
    }

    if(player->Ypos > 736.0f){
        player->Ypos = 736.0f;
    }
    if(player->Ypos < -128.0f){
        respawn_player(player);
        play_sound(&sound_data[SOUND_DEATH]);
    }

    vec3 modeltranslation = {player->Xpos, player->Ypos, 0.0f};
    glm_translate(renderer->model, modeltranslation);

    
    mat4 temp;

    glm_mat4_mul(renderer->proj, renderer->view, temp);
    glm_mat4_mul(temp, renderer->model, renderer->mvp);


    SH_SetUniformMat4f(&renderer->shader, "u_MVP", renderer->mvp);

    R_Draw(&renderer->va, &renderer->ib, &renderer->shader);

    glm_mat4_identity(renderer->model);

}



void load_textures(Shader* batchshader, Shader* playershader, Shader* backgroundshader){

    Texture player;
    Texture grassblock, dirtblock;
    Texture iceblock, snowblock;
    Texture spaceblock, spacehazardblock, spacehazardblockdown;
    Texture spikeup, spikedown, spikeleft, spikeright;
    Texture MainMenu, Winscreen;
    Texture tree_background, ice_background, space_background;
    Texture Flag;
    TX_Construct("../assets/textures/grass.png", &grassblock);
    TX_Construct("../assets/textures/dirt.png", &dirtblock);
    TX_Construct("../assets/textures/ice_block.png", &iceblock);
    TX_Construct("../assets/textures/snow_block.png", &snowblock);
    TX_Construct("../assets/textures/spaceblock.png", &spaceblock);
    TX_Construct("../assets/textures/spaceblockhazard.png", &spacehazardblock);
    TX_Construct("../assets/textures/spaceblockhazarddown.png", &spacehazardblockdown);
    TX_Construct("../assets/textures/spring boy.png", &player);
    TX_Construct("../assets/textures/Spikes.png", &spikeup);
    TX_Construct("../assets/textures/Spikes_down.png", &spikedown);
    TX_Construct("../assets/textures/Spikes_left.png", &spikeleft);
    TX_Construct("../assets/textures/Spikes_right.png", &spikeright);
    TX_Construct("../assets/textures/StartScreen.png", &MainMenu);
    TX_Construct("../assets/textures/winscreen.png", &Winscreen);
    TX_Construct("../assets/textures/tree_background.png", &tree_background);
    TX_Construct("../assets/textures/icebackground.png", &ice_background);
    TX_Construct("../assets/textures/spacebackground.png", &space_background);
    TX_Construct("../assets/textures/flag.png", &Flag);
    TX_Bind(1, &player);
    TX_Bind(2, &spikeup);
    TX_Bind(3, &spikedown);
    TX_Bind(4, &spikeleft);
    TX_Bind(5, &spikeright);
    TX_Bind(10, &grassblock);
    TX_Bind(11, &dirtblock);
    TX_Bind(12, &iceblock);
    TX_Bind(13, &snowblock);
    TX_Bind(14, &spaceblock);
    TX_Bind(15, &spacehazardblock);
    TX_Bind(16, &spacehazardblockdown);
    TX_Bind(19, &Flag);
    TX_Bind(20, &MainMenu);
    TX_Bind(21, &tree_background);
    TX_Bind(22, &ice_background);
    TX_Bind(23, &space_background);
    TX_Bind(24, &Winscreen);
    int samplers[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    //int samplers[4] = {0, 1, 2, 3};
    SH_SetUniform1iv(batchshader, "u_Textures", 32, samplers);
    SH_SetUniform1iv(playershader, "u_Textures", 32, samplers);
    SH_SetUniform1iv(backgroundshader, "u_Textures", 32, samplers);



}