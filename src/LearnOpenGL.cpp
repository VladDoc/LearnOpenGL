#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "GUI.h"
#include "Shader.h"

#include "Triangle.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, Triangle& tri)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
        tri.InitShaders();
}


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
    GLFWwindow* window = glfwCreateWindow(1000, 750, "Learn OpenGL", nullptr, nullptr);
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
    glfwSwapInterval(0); // Enable vsync

    ImGUI::Init(glsl_version, window);

    Triangle triangle;
    triangle.Init();

    //Triangle triangle2;
    //triangle2.Init();
   
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window, triangle);

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