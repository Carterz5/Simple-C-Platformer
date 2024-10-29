#include "graphics.h"








GLFWwindow* init_opengl(){

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



void Create_Batch_Renderer(Renderer* renderer, char* shaderpath, unsigned int max_vertices){
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





}

void Create_Player_Renderer(Renderer* renderer, char* shaderpath){

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

}


void Draw_Player(Renderer* renderer, Player* player){

    Quad Pquad;
    R_CreateQuad(&Pquad, 0.0f, 0.0f, 32.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    VB_AddToDynamic(&renderer->vb, sizeof(Quad), &Pquad);

    if(player->Xpos > 992.0f){
        player->Xpos = 992.0f;
    }
    if(player->Xpos < 0.0f){
        player->Xpos = 0.0f;
    }

    if(player->Ypos > 736.0f){
        player->Ypos = 736.0f;
    }
    if(player->Ypos < 0.0f){
        player->Ypos = 0.0f;
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
