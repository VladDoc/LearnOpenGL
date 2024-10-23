
#include "PostProc.h"

#include <iostream>
#include <GLFW/glfw3.h>

static Shader nullshader;

PostProcessor::PostProcessor(unsigned int width, unsigned int height)
    : width(width), height(height)
{
    nullshader.InitWithFiles(
        MAIN_SOURCE_DIR "shaders/nullvertex.vert",
        MAIN_SOURCE_DIR "shaders/nullfragment.frag");

    //shaders.push_back(shader);
    //shaders.push_back(shader);
    //shaders.push_back(shader);
    //shaders.push_back(shader);
    //shaders.push_back(shader);
    //shaders.push_back(shader);

    // if even size add passthrough shader
    //if(!(shaders.size() & 1)) {
        //shaders.push_back(nullshader);
    //}
    Init();
}

void PostProcessor::AddShader(Shader shader)
{
    shaders.push_back(shader);
}

void PostProcessor::Init()
{
    for(int i = 0; i < 2; ++i) {

        textures[i].wrapS     = GL_REPEAT;
        textures[i].wrapT     = GL_REPEAT;
        textures[i].minFilter = GL_LINEAR;
        textures[i].magFilter = GL_LINEAR;
        textures[i].format    = GL_RGBA;

        // initialize renderbuffer/framebuffer object
        glGenFramebuffers(1, &MSFBO[i]);
        glGenFramebuffers(1, &FBO[i]);
        glGenRenderbuffers(1, &RBO[i]);
        // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
        glBindFramebuffer(GL_FRAMEBUFFER, MSFBO[i]);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO[i]);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, width, height); // allocate storage for render buffer object
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, RBO[i]); // attach MS render buffer object to framebuffer
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
        // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
        glBindFramebuffer(GL_FRAMEBUFFER, FBO[i]);

        textures[i].Generate(width, height);
        textures[i].Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i].texture, 0); // attach texture to framebuffer as its color attachment
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // initialize render data and uniforms
    initRenderData();
}

void PostProcessor::BeginRender()
{
    for(int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, MSFBO[i]);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, MSFBO[0]);
    glViewport(0, 0, width, height);
}
void PostProcessor::EndRender()
{
    // now resolve multisampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO[swapped_slot]);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO[swapped_slot]);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessor::Render()
{
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    swapped_slot = 0;
    for(auto& shader : shaders) {
        lolkek:
        glBindFramebuffer(GL_FRAMEBUFFER, MSFBO[swapped_slot]);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO[swapped_slot]);
        glClearColor(0, 0, 0, 1);
        shader.Use();
        // set uniforms/options
        shader.SetFloat("time", glfwGetTime());
        // render textured quad
        textures[swapped_slot].Bind();
        //glBindTexture(GL_TEXTURE_2D, textures[swapped_slot].texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        EndRender();
        swapped_slot = !swapped_slot;
        if(!swapped_slot) goto lolkek;
    }
    swapped_slot = !swapped_slot;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}

void PostProcessor::initRenderData()
{
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
