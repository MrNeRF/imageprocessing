#ifndef SHADER_H
#define SHADER_H

#include <Eigen/Dense>
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#define GLM_FORCE_CXX14
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

public:
    explicit Shader(const std::string& name)
        : shaderName(name){};

    void InitShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void UseShader(void) { glUseProgram(shaderProgramID); }
    unsigned int GetShaderProgramID(void) { return shaderProgramID; };

    template<typename T>
    void SetValue(const std::string& name, T value);
    template<typename T, int N = 1>
    void SetVector(const std::string& name, const T& v);
    template<typename T>
    void SetTransformationMatrix(const std::string& name, const T& mat);
    void SetTransformationMatrix(const std::string& name, glm::mat4& tran)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &tran[0][0]);
    }

public:
    const std::string shaderName;

private:
    std::string readShaderProgramCode(const std::string& shaderPath);

    void createShader(unsigned int shaderID, const std::string& shaderProgramCode, const Shader::ShaderType& shaderType);
    void createShaderProgram(const std::vector<unsigned int>& IDs);
    void checkCompileErrors(unsigned int shaderID, const Shader::ShaderType& shaderType);

private:
    unsigned int shaderProgramID;

    std::map<ShaderType, std::string> shaderTypeMapping = {{ShaderType::VERTEX, "VERTEX"},
                                                           {ShaderType::FRAGMENT, "FRAGMENT"},
                                                           {ShaderType::PROGRAM, "PROGRAM"}};
};

template<typename T>
void Shader::SetValue(const std::string& name, T value)
{
    if (std::is_floating_point<T>::value)
    {
        glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
    }
    else
    {
        glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
    }
}

// Vector
template<typename T, int N>
void Shader::SetVector(const std::string& name, const T& v)
{
    if (std::is_same<Eigen::Vector3f, T>::value)
    {
        glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), N, v.data());
    }
    else if (std::is_same<Eigen::Vector4f, T>::value)
    {
        glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), N, v.data());
    }
    else
    {
        assert(false && "Neither Vectorf3f nor Vector4f");
    }
}

// Matrices
template<typename T>
void Shader::SetTransformationMatrix(const std::string& name, const T& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data());
}

#endif
