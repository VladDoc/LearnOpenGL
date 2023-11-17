#pragma once
#include <string>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    void InitWithFiles(const char* vertexPath, const char* fragmentPath);
    void InitWithStrings(const char* vertexSrc, const char* fragmentSrc);

    // use/activate the shader
    void Use();
    // utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
};

