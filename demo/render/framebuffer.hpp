#ifndef BENGINE_FRAMEBUFFER
#define BENGINE_FRAMEBUFFER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class FrameBuffer {
    public:

        void create_buffers(int32_t width, int32_t height);

        void delete_buffers();

        void bind();

        void unbind();

        uint32_t get_texture();

    protected:

        uint32_t m_FBO = 0;
        uint32_t m_TexId = 0;
        uint32_t m_DepthId = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

};

#endif