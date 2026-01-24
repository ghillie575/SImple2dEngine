#pragma once
#include <vector>
namespace engine
{
    class EBO
    {
    public:
        EBO(const std::vector<unsigned int> &indices) : indices(indices), eboID(0) {};
        ~EBO();
        void create();
        void bind() const;
        void unbind() const;
        void free();
        std::vector<unsigned int> indices;

    private:
        unsigned int eboID;
    };
} // namespace engine
