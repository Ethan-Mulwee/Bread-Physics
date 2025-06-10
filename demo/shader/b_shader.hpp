#ifndef BENGINE_SHADER
#define BENGINE_SHADER

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "bMath.hpp"

#include <string>

class Shader {
    private:
        unsigned int m_ProgramId;

        unsigned int getCompiledShader(unsigned int shaderType, const char* shaderSource);

    public:
        void load(const std::string &vertShader, const std::string &fragShader);

        void use();

        void unload();

        unsigned int get_program_id() {return m_ProgramId;}

        void setMatrix4(const bMath::matrix4 &mat4, const std::string &name);
        void setInt(int v, const std::string &name);
        void set_float(float v, const std::string &name);
        void setFloat3(bMath::float3 &v, const std::string &name);
        void set_float4(bMath::float4 &v, const std::string &name);
};

#endif