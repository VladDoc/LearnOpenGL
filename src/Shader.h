#pragma once
#include <string>

#include <unordered_map>

class Shader
{
public:
    static constexpr auto NullID = 0xFFFFFFFF;

    // the program ID
    uint32_t ID = NullID;

    Shader& InitWithFiles(const char* vertexPath, const char* fragmentPath, bool reload = false);
    Shader& InitWithStrings(const char* vertexSrc, const char* fragmentSrc);

    // will blow up if called on an unitialized file
    void Reload() {
        this->InitWithFiles(nullptr, nullptr, false);
    }

    // use/activate the shader
    void Use();
    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
};

struct ShaderPaths
{
    std::string vertexFile, fragmentFile;
};

struct ShaderInfoStorage
{
    using ID = uint32_t;
    using FileMap = std::unordered_map<ID, ShaderPaths>;
    static FileMap shaderfiles;
};
