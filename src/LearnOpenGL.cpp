#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "GUI.h"
#include "Shader.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


struct Triangle
{
    
    // Regular ass triangle
    float vertices[18] = {
        // positions         // colors
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
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

    Shader shader;

    void ImGUIFlex()
    {
        for (int i = 0; i < 3; ++i) {
            static char labelVertexPos[256];
            sprintf(labelVertexPos, "Pos @0x%x #%d", (uint8_t)vertices, i);
            ImGui::SliderFloat3(labelVertexPos, vertices + i * 6, -1.0f, 1.0f);
        }

        for (int i = 0; i < 3; ++i) {
            static char labelVertexColor[256];
            sprintf(labelVertexColor, "Color @0x%x #%d", (uint8_t)vertices, i);
            ImGui::ColorEdit3(labelVertexColor, vertices + i * 6 + 3);
        }
    }

    void Init()
    {
        shader.InitWithFiles("../../../../shaders/vertex.vert", "../../../../shaders/fragment.frag");

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    void Draw()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        shader.Use();
        shader.SetFloat("time", glfwGetTime());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;

    // GL 3.3 + GLSL 150
    const char* glsl_version = "#version 330 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          
 
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Learn OpenGL", nullptr, nullptr);
    if (window == nullptr) 
    {
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "%s", "Failed to initialize GLAD\n");
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable vsync

    ImGUI::Init(glsl_version, window);

    Triangle triangle;
    triangle.Init();

    //Triangle triangle2;
    //triangle2.Init();
   
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        ImGUI::ClearFrame();
        ImGUI::NewFrame();


        triangle.Draw();
        //triangle2.Draw();

        // imgui window
        {
            ImGui::Begin("Config");
            ImGui::ColorEdit3("clear color", (float*)&ImGUI::clear_color); // Edit 3 floats representing a color

            triangle.ImGUIFlex();
            //triangle2.ImGUIFlex();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGUI::io->Framerate, ImGUI::io->Framerate);
            ImGui::End();
        }

        ImGUI::RenderFrame();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}