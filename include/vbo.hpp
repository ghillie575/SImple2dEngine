#pragma once
#include <glad/glad.h>
#include <vector>
namespace engine
{
    struct VertexAttributePointer
    {
        unsigned int index;
        int size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        const void *pointer;
    };
    class VBO
    {
    public:
        unsigned int ID;
        VBO(const std::vector<float> &data) : data(data), ID(0) {}
        void create();
        void bind();
        void unbind();
        void free();
        void addVertexAttributePointer(unsigned int index, int size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    private:
        std::vector<float> data;
    };
} // namespace engine