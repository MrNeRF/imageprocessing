#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
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
        unsigned int GetShaderProgramID(void){return m_shaderProgramID;};
        void SetInt(const std::string &name, int value) const {glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);};
        void use(void) {glUseProgram(m_shaderProgramID);};

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