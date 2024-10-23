#pragma once
#include <vector>

#include <glad.h>

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class PostProcessor
{
public:
    std::vector<Shader> shaders;
    unsigned int width, height;
    PostProcessor(unsigned int width, unsigned int height);

    void Init();

    void AddShader(Shader shader);

    // prepares the postprocessor's framebuffer operations before rendering the game
    void BeginRender();
    // should be called after rendering the game, so it stores all the rendered data into a texture object
    void EndRender();
    // renders the PostProcessor texture quad (as a screen-encompassing large sprite)
    void Render();
private:
    // render state
    Texture         textures[2]{};
    unsigned int    MSFBO[2]{};
    unsigned int    FBO[2]{}; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    unsigned int    RBO[2]{}; // RBO is used for multisampled color buffer
    unsigned int    VAO{};
    unsigned int    VBO{};

    bool swapped_slot = 0;

    // initialize quad for rendering postprocessing texture
    void initRenderData();
};
