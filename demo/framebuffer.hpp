#ifndef BFRAMEBUF
#define BFRAMEBUF

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

        uint32_t fBO = 0;
        uint32_t texId = 0;
        uint32_t depthId = 0;
        uint32_t width = 0;
        uint32_t height = 0;

};

#endif