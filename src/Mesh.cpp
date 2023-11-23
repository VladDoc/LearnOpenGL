#include "Mesh.h"

#include <glad.h>
#include <GLFW/glfw3.h>
#include "CRC8.h"


static char texture1Path[260] = MAIN_SOURCE_DIR "textures/container.jpg";
static char texture2Path[260] = MAIN_SOURCE_DIR "textures/awesomeface.png";

void Mesh::ImGUIFlex()
{
    void* address = this;

    for (int i = 0; i < std::size(vertices) / stride; ++i) {
        static char labelVertexPos[256];
        sprintf(labelVertexPos, "Pos @0x%x #%d", (int)CRC8(address, sizeof(address)), i);
        ImGui::SliderFloat3(labelVertexPos, vertices + i * Mesh::stride, -1.0f, 1.0f);
    }

    for (int i = 0; i < std::size(vertices) / stride; ++i) {
        static char labelVertexColor[256];
        sprintf(labelVertexColor, "Color @0x%x #%d", (int)CRC8(address, sizeof(address)), i);
        ImGui::ColorEdit3(labelVertexColor, vertices + i * Mesh::stride + 3);
    }

    //for (int i = 0; i < std::size(indices); ++i) {
    //    static char labelIndex[256];
    //    sprintf(labelIndex, "Index @0x%x #%d", (int)CRC8(address, sizeof(address)), i);
    //    ImGui::InputInt(labelIndex, indices + i);
    //}

    ImGui::Separator();
    ImGui::Checkbox("Apply Color", &applyColor);
    ImGui::SameLine();
    ImGui::PushItemWidth(150);
    ImGui::SliderFloat("Blending Factor", &blendingFactor, -1, 1);
    ImGui::PopItemWidth();

    ImGui::InputText("Texture1", texture1Path, sizeof(texture1Path));
    ImGui::InputText("Texture2", texture2Path, sizeof(texture2Path));
    if (ImGui::Button("Reload Textures and Shaders")) {
        this->Init();
    }
}

void Mesh::InitShaders()
{
    shader.InitWithFiles(MAIN_SOURCE_DIR "shaders/vertex.vert", MAIN_SOURCE_DIR "shaders/fragment.frag");
}

void Mesh::InitTextures()
{
    texture1.textureIndex = GL_TEXTURE0;
    texture1.Init(texture1Path);

    texture2.textureIndex = GL_TEXTURE1;
    texture2.format = GL_RGBA;
    texture2.Init(texture2Path);
}

void Mesh::Init()
{
    InitShaders();
    InitTextures();

    shader.Use();
    shader.SetInt("texture0", 0);
    shader.SetInt("texture1", 1);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Mesh::Draw()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Mesh::stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Mesh::stride * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Mesh::stride * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader.Use();
    shader.SetFloat("time", glfwGetTime());
    shader.SetFloat("blendingFactor", blendingFactor);
    shader.SetBool("applyColor", applyColor);

    texture1.Bind();
    texture2.Bind();

    glBindVertexArray(VAO);
    
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    glBindVertexArray(0);
}