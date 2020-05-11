#ifndef SHADER_H
#define SHADER_H

#include <Eigen/Dense>
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include "Logger.h"
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
    void UseShader(void) { CHECK_GL_ERROR_(glUseProgram(shaderProgramID)); }
    unsigned int GetShaderProgramID(void) { return shaderProgramID; };

    template<typename T>
    void SetValue(const std::string& name, T value);
    void SetQuat(const std::string& name, const Eigen::Quaternionf& v)
    {
        CHECK_GL_ERROR_(glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), v.x(), v.y(), v.z(), v.w()));
    }
    template<typename T, int N = 1>
    void SetVector(const std::string& name, const T& v);
    template<typename T>
    void SetTransformationMatrix(const std::string& name, const T& mat);
    void SetTransformationMatrix(const std::string& name, glm::mat4& tran)
    {
        CHECK_GL_ERROR_(glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &tran[0][0]))
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
        CHECK_GL_ERROR_(glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value));
    }
    else
    {
        CHECK_GL_ERROR_(glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value));
    }
}

// Vector
template<typename T, int N>
void Shader::SetVector(const std::string& name, const T& v)
{
    if (std::is_same<Eigen::Vector3f, T>::value)
    {
        CHECK_GL_ERROR_(glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), N, v.data()));
    }
    else if (std::is_same<Eigen::Vector4f, T>::value)
    {
        CHECK_GL_ERROR_(glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), N, v.data()));
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
    CHECK_GL_ERROR_(glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data()));
}

#endif
