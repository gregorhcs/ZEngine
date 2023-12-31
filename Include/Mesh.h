#pragma once

#include "ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace zn {

struct VertexAttribute {
    GLuint size;
    GLenum type;
    GLsizei stride;
    const void* offset;
};

class Mesh
{
    friend class ResourceManager;

public:

    glm::vec3 position_ = glm::vec3(0.f);
    glm::vec3 rotation_ = glm::vec3(0.f);
    glm::vec3 scale_ = glm::vec3(1.f);

    glm::vec3 initialPosition_ = glm::vec3(0.f);

    bool bHasCollision = false;

    Mesh() {};
    ~Mesh();

    // create vbo & vao, send data_ to GPU, configure vertex attributes
    void SendVertexDataToGPU() noexcept;

    // bind vao and issue draw call
    void Draw() const noexcept;

    virtual void Reset();

    virtual glm::vec2 TopLeft() const noexcept;
    virtual glm::vec2 TopRight() const noexcept;
    virtual glm::vec2 BottomLeft() const noexcept;
    virtual glm::vec2 BottomRight() const noexcept;

    virtual float Top() const noexcept;
    virtual float Bottom() const noexcept;
    virtual float Left() const noexcept;
    virtual float Right() const noexcept;

    virtual bool CheckCollision(const Mesh* other) const noexcept;

protected:

    Mesh(GLfloat* vertices, unsigned int verticesSize, unsigned int* indices, unsigned int indicesSize, std::vector<VertexAttribute> vertexAttributes);

    // calculate number of vertices for this mesh from dataSize_ and vertexAttributes_
    unsigned int CalculateNumberOfVertices();

    // calculate number of vertices for this mesh from indices_
    unsigned int CalculateNumberOfIndices();

    std::vector<VertexAttribute> vertexAttributes_;

    GLfloat* vertices_;
    unsigned int verticesSize_;
    unsigned int numberOfVertices_;

    unsigned int* indices_;
    unsigned int indicesSize_;
    unsigned int numberOfIndices_;

    GLuint vbo_, vao_, ebo_;

    bool bHoldsData_;

};

}
