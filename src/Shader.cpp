#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "glad.h"

#include "DebugPrint.h"

ShaderInfoStorage::FileMap ShaderInfoStorage::shaderfiles;

Shader& Shader::InitWithFiles(const char* vertexPath, const char* fragmentPath, bool reload)
{
    ShaderPaths files;
    if(ID != NullID) {
        auto it = ShaderInfoStorage::shaderfiles.find(ID);
        if(it != ShaderInfoStorage::shaderfiles.end() && reload)
            files = std::move(it->second);
        else
            files = {vertexPath, fragmentPath};

        ShaderInfoStorage::shaderfiles.erase(ID);
    } else {
        files = {vertexPath, fragmentPath};
    }
    vertexPath      = files.vertexFile.c_str();
    fragmentPath    = files.fragmentFile.c_str();

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    DebugLog() {
        std::cout << "Compiling: "  << std::endl;
        std::cout << "\t" << vertexPath     << std::endl;
        std::cout << "\t" << fragmentPath   << std::endl;
        std::cout << "\t" << "Old ID: " <<
            (ID == NullID ? "(NullID)" : std::to_string(ID)) << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    InitWithStrings(vShaderCode, fShaderCode);

    ShaderInfoStorage::shaderfiles[ID] = std::move(files);

    DebugLog() {
        std::cout << "\t" << "ID: " << ID << std::endl;
    }
    return *this;
}

Shader& Shader::InitWithStrings(const char* vShaderCode, const char* fShaderCode)
{
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // vertex Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // reloading occurs
    if(ID != Shader::NullID) glDeleteProgram(ID);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return *this;
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
