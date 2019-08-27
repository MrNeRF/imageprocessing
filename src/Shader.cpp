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



