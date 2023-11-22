#include "Triangle.h"

#include <glad.h>
#include <GLFW/glfw3.h>
#include "CRC8.h"


static char texturePath[260] = MAIN_SOURCE_DIR "textures/sviborg.jpg";

void Triangle::ImGUIFlex()
{
    void* address = this;

    for (int i = 0; i < std::size(vertices) / stride; ++i) {
        static char labelVertexPos[256];
        sprintf(labelVertexPos, "Pos @0x%x #%d", (int)CRC8(address, sizeof(address)), i);
        ImGui::SliderFloat3(labelVertexPos, vertices + i * Triangle::stride, -1.0f, 1.0f);
    }

    for (int i = 0; i < std::size(vertices) / stride; ++i) {
        static char labelVertexColor[256];
        sprintf(labelVertexColor, "Color @0x%x #%d", (int)CRC8(address, sizeof(address)), i);
        ImGui::ColorEdit3(labelVertexColor, vertices + i * Triangle::stride + 3);
    }

    for (int i = 0; i < std::size(indices); ++i) {
        static char labelIndex[256];
        sprintf(labelIndex, "Index @0x%x #%d", (int)CRC8(address, sizeof(address)), i);
        ImGui::InputInt(labelIndex, indices + i);
    }
}

void Triangle::InitShaders()
{
    shader.InitWithFiles(MAIN_SOURCE_DIR "shaders/vertex.vert", MAIN_SOURCE_DIR "shaders/fragment.frag");
}

void Triangle::InitTextures()
{
    texture.Init(texturePath);
}

void Triangle::Init()
{
    InitShaders();
    InitTextures();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Triangle::Draw()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Triangle::stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Triangle::stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Triangle::stride * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader.Use();
    shader.SetFloat("time", glfwGetTime());

    texture.Bind();
    glBindVertexArray(VAO);
    
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    glBindVertexArray(0);
}