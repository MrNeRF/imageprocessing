#include "Shader.h"
#include <GL/glew.h>

void Shader::InitShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::vector<unsigned int> shaderIDs;

    // Vertex Shader Shader
    std::string  vertexCode     = readShaderProgramCode(vertexShaderPath);
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    createShader(vertexShaderID, vertexCode, ShaderType::VERTEX);
    shaderIDs.push_back(vertexShaderID);

    // Fragment Shader
    std::string  fragmentCode     = readShaderProgramCode(fragmentShaderPath);
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    createShader(fragmentShaderID, fragmentCode, ShaderType::FRAGMENT);
    shaderIDs.push_back(fragmentShaderID);

    // Shader Program
    createShaderProgram(shaderIDs);
}

std::string Shader::readShaderProgramCode(const std::string& shaderPath)
{
    std::string   shaderCode;
    std::ifstream shaderFileStream;
    shaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFileStream.open(shaderPath.c_str());
        std::stringstream shaderStringStream;

        shaderStringStream << shaderFileStream.rdbuf();

        shaderFileStream.close();

        shaderCode = shaderStringStream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cerr << "ERROR: Shader file not successfully read: " << e.what() << '\n';
    }

    return shaderCode;
}

void Shader::createShader(unsigned int shaderID, const std::string& shaderProgramCode, const Shader::ShaderType& shaderType)
{
    const GLchar* code = shaderProgramCode.c_str();
    glShaderSource(shaderID, 1, &code, NULL);
    glCompileShader(shaderID);
    checkCompileErrors(shaderID, shaderType);
}

void Shader::createShaderProgram(const std::vector<unsigned int>& IDs)
{
    shaderProgramID = glCreateProgram();
    for (const auto& id : IDs)
    {
        glAttachShader(shaderProgramID, id);
    }
    checkCompileErrors(shaderProgramID, ShaderType::PROGRAM);
    for (const auto& id : IDs)
    {
        glDeleteShader(id);
    }
}

void Shader::checkCompileErrors(unsigned int shaderID, const Shader::ShaderType& shaderType)
{
    int  success;
    char infoLog[1024];

    // Lambda to print compile Error
    auto printCompileError = [this](const Shader::ShaderType& type, char* infoLog) {
        std::cout << shaderName << "\n"
                  << "SHADER_LINKING_ERROR of type: "
                  << shaderTypeMapping[type] << "\n"
                  << infoLog
                  << "\n -- ----------------------------------- --\n";
    };

    switch (shaderType)
    {
        case ShaderType::FRAGMENT:
        case ShaderType::VERTEX:
            glGetShaderiv(shaderID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
                printCompileError(shaderType, infoLog);
            }
            break;
        case ShaderType::PROGRAM:
            glGetProgramiv(shaderID, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
                printCompileError(shaderType, infoLog);
            }
            break;
        default:
            std::cout << shaderTypeMapping[shaderType] << " successfully compiled!"
                      << "\n"
                      << "\n -- ----------------------------------- --\n";
    };
}

