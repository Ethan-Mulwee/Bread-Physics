#include "b_shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

unsigned int Shader::get_compiled_shader(unsigned int shaderType, const std::string &shaderSource) {
    unsigned int shader_id = glCreateShader(shaderType);

    const char* c_source = shaderSource.c_str();
    glShaderSource(shader_id, 1, &c_source, nullptr);
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

    std::cout << "compile result: " << result << "\n";

    return shader_id;
}

void Shader::load(const std::string &vertShader, const std::string &fragShader) {
    std::ifstream is_vs(vertShader);
    const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

    std::ifstream is_fs(fragShader);
    const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

    m_ProgramId = glCreateProgram();

    std::cout << "m_ProgramId: " << m_ProgramId << "\n";
    unsigned int vs = get_compiled_shader(GL_VERTEX_SHADER, f_vs);
    unsigned int fs = get_compiled_shader(GL_FRAGMENT_SHADER, f_fs);
    std::cout << "vs: " << vs << "\n";
    std::cout << "fs: " << fs << "\n";

    glAttachShader(m_ProgramId, vs);
    glAttachShader(m_ProgramId, fs);

    glLinkProgram(m_ProgramId);
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

void Shader::set_mat4(const bMath::matrix4 &mat4, const std::string &name)
{
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glUniformMatrix4fv(myLoc, 1, GL_FALSE, &mat4.data[0][0]);
}

void Shader::set_int(const int v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glUniform1i(myLoc, v);
}

void Shader::set_float3(bMath::float3 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(get_program_id(), name.c_str());
    glProgramUniform3fv(get_program_id(), myLoc, 1, v.data);
}
