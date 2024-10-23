#pragma once
#include "GUI.h"
#include "Shader.h"
#include "Texture.h"

struct Mesh : ImGUIable
{
    // floats per vertex
    inline static const size_t stride = 8;
    // Regular rectangle
    float vertices[32] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    int indices[6] = {
        3, 1, 2,
        1, 0, 3
    };
    /*

    // Full screen triangle
    float vertices[18] = {
        // positions         // colors
        -1.0f,  3.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -2.0f, -2.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         3.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };
    */
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;

    Shader shader;

    Texture texture1;
    Texture texture2;

    float blendingFactor = 0.5;
    bool applyColor = true;
    bool rotate = false;

    void ImGUIFlex() override;

    void InitShaders();
    
    void InitTextures();

    void Init();

    void Draw();
};
