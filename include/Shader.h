#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

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

    private:
        void initShaders(void);
        void checkCompileErrors(unsigned int shaderID, Shader::ShaderType shaderType);

    private:
        unsigned int m_shaderProgramID;
        std::map<ShaderType, std::string> m_shaderTypeMapping;
        std::string m_vertexShaderPath;
        std::string m_fragmentShaderPath;
};

#endif