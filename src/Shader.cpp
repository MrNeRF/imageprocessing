#include "Shader.h"
#include "Logger.h"
#include <GL/glew.h>

void Shader::InitShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::vector<unsigned int> shaderIDs;

    // Vertex Shader Shader
    std::string  vertexCode = readShaderProgramCode(vertexShaderPath);
    unsigned int vertexShaderID;
    CHECK_GL_ERROR_WITH_OUTPUT_(vertexShaderID, glCreateShader(GL_VERTEX_SHADER))
    createShader(vertexShaderID, vertexCode, ShaderType::VERTEX);
    shaderIDs.push_back(vertexShaderID);
    auto& rLogger = Logger::GetInstance().GetLogger();
    rLogger.info("Vertex Shader {} with ID {} created", vertexShaderPath, vertexShaderID);

    // Fragment Shader
    std::string  fragmentCode = readShaderProgramCode(fragmentShaderPath);
    unsigned int fragmentShaderID;
    CHECK_GL_ERROR_WITH_OUTPUT_(fragmentShaderID, glCreateShader(GL_FRAGMENT_SHADER))
    createShader(fragmentShaderID, fragmentCode, ShaderType::FRAGMENT);
    shaderIDs.push_back(fragmentShaderID);
    rLogger.info("Fragment Shader {} with ID {} created", fragmentShaderPath, fragmentShaderID);

    // Shader Program
    createShaderProgram(shaderIDs);
    rLogger.info("Shader Program created");
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
        auto& rLogger = Logger::GetInstance().GetLogger();
        rLogger.error("ERROR: Shader file not successfully read: {}", e.what());
    }

    return shaderCode;
}

void Shader::createShader(unsigned int shaderID, const std::string& shaderProgramCode, const Shader::ShaderType& shaderType)
{
    const GLchar* code = (const GLchar*)shaderProgramCode.c_str();
    CHECK_GL_ERROR_(glShaderSource(shaderID, 1, &code, NULL));
    CHECK_GL_ERROR_(glCompileShader(shaderID));
    checkCompileErrors(shaderID, shaderType);
}

void Shader::createShaderProgram(const std::vector<unsigned int>& IDs)
{
    CHECK_GL_ERROR_WITH_OUTPUT_(shaderProgramID, glCreateProgram())
    for (const auto& id : IDs)
    {
        CHECK_GL_ERROR_(glAttachShader(shaderProgramID, id));
    }
    CHECK_GL_ERROR_(glLinkProgram(shaderProgramID));
    checkCompileErrors(shaderProgramID, ShaderType::PROGRAM);
    for (const auto& id : IDs)
    {
        CHECK_GL_ERROR_(glDeleteShader(id));
    }
}

void Shader::checkCompileErrors(unsigned int shaderID, const Shader::ShaderType& shaderType)
{
    int  success;
    char infoLog[GL_INFO_LOG_LENGTH];

    // Lambda to print compile Error
    auto printCompileError = [this](const Shader::ShaderType& type, char* infoLog) {
        auto& rLogger = Logger::GetInstance().GetLogger();
        rLogger.error("{}: Shader Link Error of Type: {}, {}", shaderName, shaderTypeMapping[type], infoLog);
    };

    switch (shaderType)
    {
        case ShaderType::FRAGMENT:
        case ShaderType::VERTEX:
            CHECK_GL_ERROR_(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success))
            if (!success)
            {
                glGetShaderInfoLog(shaderID, GL_INFO_LOG_LENGTH, NULL, &infoLog[0]);
                printCompileError(shaderType, infoLog);
            }
            break;
        case ShaderType::PROGRAM:
            glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderID, GL_INFO_LOG_LENGTH, NULL, &infoLog[0]);
                printCompileError(shaderType, infoLog);
            }
            break;
        default:
            auto& rLogger = Logger::GetInstance().GetLogger();
            rLogger.info("{} successfully compile!", shaderTypeMapping[shaderType]);
    };
}
