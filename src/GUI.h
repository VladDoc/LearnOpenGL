#pragma once

#include "imgui.h"

struct GLFWwindow;


struct ImGUI {
    static void Init(const char* glsl_version, GLFWwindow* window);

    static void ClearFrame();
    static void NewFrame();
    static void RenderFrame();

    static void ReleaseResources();

    static bool show_demo_window;
    static bool show_another_window;
    static ImVec4 clear_color;
    static ImGuiIO* io;
};
