#include "Shader.h"
#include <GL/glew.h>


Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : m_vertexShaderPath(vertexShaderPath), m_fragmentShaderPath(fragmentShaderPath)
{
    m_shaderTypeMapping[ShaderType::Vertex]     = "VERTEX";
    m_shaderTypeMapping[ShaderType::Fragment]   = "FRAGMENT";
    m_shaderTypeMapping[ShaderType::Program]    = "PROGRAM";

    initShaders();
}

void Shader::initShaders(void)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(m_vertexShaderPath.c_str());
        fShaderFile.open(m_fragmentShaderPath.c_str());
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch(const std::ifstream::failure& e)
    {
        std::cerr << "ERROR: Shader file not successfully read: " << e.what() << '\n';
    }

    unsigned int vertexShaderID, fragmentShaderID;

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *vertexShaderCode = vertexCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShaderID);
    checkCompileErrors(vertexShaderID, ShaderType::Vertex);

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar *fragmentShaderCode = fragmentCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShaderID);
    checkCompileErrors(fragmentShaderID, ShaderType::Fragment);

    m_shaderProgramID = glCreateProgram();
    glAttachShader(m_shaderProgramID, vertexShaderID);
    glAttachShader(m_shaderProgramID, fragmentShaderID);
    glLinkProgram(m_shaderProgramID);
    checkCompileErrors(m_shaderProgramID, ShaderType::Program);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

}

void Shader::checkCompileErrors(unsigned int shaderID, Shader::ShaderType shaderType)
{
    int success;
    char infoLog[1024];
    if(shaderType == ShaderType::Program)
    {
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
            std::cout   << "ERROR::PROGRAM_LINKING_ERROR of type: "  
                        << m_shaderTypeMapping[shaderType] << "\n"
                        << infoLog 
                        << "\n -- ----------------------------------- --\n";
        }

    }

    else
    {
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
            std::cout   << "ERROR::SHADER_COMPILATION_ERROR of type: "  
                        << m_shaderTypeMapping[shaderType] << "\n"
                        << infoLog 
                        << "\n -- ----------------------------------- --\n";
        }
    }

}

void Shader::activate(void)
{
    glUseProgram(m_shaderProgramID);
}

void Shader::SetBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const Eigen::Vector3f& v)
{
    glUniform3fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, v.data());
}

void Shader::SetVec3(const std::string& name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const Eigen::Vector4f& v)
{
    glUniform4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, v.data());
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z, w);
}

void Shader::SetMat3(const std::string& name, const Eigen::Matrix3f& mat)
{
    glUniformMatrix3fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data());
}

void Shader::SetMat4(const std::string& name, const Eigen::Matrix4f& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, mat.data());
}

