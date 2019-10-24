#ifndef SHADER_H
#define SHADER_H

#include <Eigen/Dense>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Shader
{
public:
    enum class ShaderType
    {
        Program,
        Vertex,
        Fragment
    };

public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader(void) = default;
    unsigned int GetShaderProgramID(void) { return m_shaderProgramID; };

    void initShaders(void);
    void checkCompileErrors(unsigned int shaderID, Shader::ShaderType shaderType);
    void activate(void);
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec3(const std::string& name, const Eigen::Vector3f& v);
    void SetVec3(const std::string& name, float x, float y, float z);
    void SetVec4(const std::string& name, const Eigen::Vector4f& v);
    void SetVec4(const std::string& name, float x, float y, float z, float w);
    void SetMat3(const std::string& name, const Eigen::Matrix3f& mat);
    void SetMat4(const std::string& name, const Eigen::Matrix4f& mat);

private:
    unsigned int                      m_shaderProgramID;
    std::map<ShaderType, std::string> m_shaderTypeMapping;
    std::string                       m_vertexShaderPath;
    std::string                       m_fragmentShaderPath;
};

#endif
