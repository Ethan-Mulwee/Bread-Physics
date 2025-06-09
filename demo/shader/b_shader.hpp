#ifndef BENGINE_SHADER
#define BENGINE_SHADER

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "bMath.hpp"

#include <string>

class Shader {
    private:
        unsigned int m_ProgramId;

        unsigned int get_compiled_shader(unsigned int shaderType, const std::string &shaderSource);

    public:
        void load(const std::string &vertShader, const std::string &fragShader);

        void use();

        void unload();

        unsigned int get_program_id() {return m_ProgramId;}

        void set_mat4(const bMath::matrix4 &mat4, const std::string &name);
        void set_int(int v, const std::string &name);
        void set_float(float v, const std::string &name);
        void set_float3(bMath::float3, const std::string &name);
        void set_float4(bMath::float4, const std::string &name);
};

#endif