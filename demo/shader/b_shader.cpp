#include "b_shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstring>

unsigned int Shader::getCompiledShader(unsigned int shaderType, const char* shaderSource) {
    unsigned int shader_id = glCreateShader(shaderType);

    glShaderSource(shader_id, 1, &shaderSource, nullptr);
    glCompileShader(shader_id);

    GLint result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

        GLchar* strInfoLog = new GLchar[length + 1];
        glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

        fprintf(stderr, "Compile error in shader: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    return shader_id;
}

void Shader::load(const std::string &vertShader, const std::string &fragShader) {
    
    std::ifstream is_vs(vertShader);
    if (!is_vs.is_open()) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    }

    const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

    std::ifstream is_fs(fragShader);
        if (!is_fs.is_open()) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    }

    const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

    m_ProgramId = glCreateProgram();

    unsigned int vs = getCompiledShader(GL_VERTEX_SHADER, f_vs.c_str());
    unsigned int fs = getCompiledShader(GL_FRAGMENT_SHADER, f_fs.c_str());

    glAttachShader(m_ProgramId, vs);
    glAttachShader(m_ProgramId, fs);

    GLint attached = 0;

    glLinkProgram(m_ProgramId);

    GLint linkResult;
    glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE) {
        std::cout << "shader linking failed \n";

        int length;
        glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &length);

        GLchar* strInfoLog = new GLchar[length + 1];
        glGetShaderInfoLog(m_ProgramId, length, &length, strInfoLog);

        fprintf(stderr, "Shader linking error: %s\n", strInfoLog);
        delete[] strInfoLog;
    }


    glValidateProgram(m_ProgramId);

    GLint validationResult;
    glGetProgramiv(m_ProgramId, GL_VALIDATE_STATUS, &validationResult);
    if (validationResult == GL_FALSE) {
        std::cout << "shader validation failed \n";
    }


    glDeleteShader(vs);
    glDeleteShader(fs);

}

void Shader::use()
{
    glUseProgram(m_ProgramId);
}

void Shader::unload()
{
    glDeleteProgram(m_ProgramId);
}

void Shader::setMatrix4(const bMath::matrix4 &mat4, const std::string &name)
{
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glUniformMatrix4fv(myLoc, 1, GL_TRUE, &mat4.data[0][0]);
}

void Shader::setInt(const int v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glUniform1i(myLoc, v);
}

void Shader::setFloat3(bMath::float3 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glProgramUniform3fv(get_program_id(), myLoc, 1, v.data);
}

void Shader::setFloat4(bMath::float4 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glProgramUniform4fv(get_program_id(), myLoc, 1, v.data);
}
