#include "Mesh.h"

namespace zn {

Mesh::Mesh(GLfloat* vertices, unsigned int verticesSize, unsigned int* indices, unsigned int indicesSize, std::vector<VertexAttribute> vertexAttributes) :
	vertices_(vertices),
	verticesSize_(verticesSize),
	indices_(indices),
	indicesSize_(indicesSize),
	numberOfIndices_(CalculateNumberOfIndices()),
	vertexAttributes_(vertexAttributes),
	vbo_(0), vao_(0), ebo_(0),
	numberOfVertices_(CalculateNumberOfVertices()),
	bHoldsData_(false)
{}

unsigned int Mesh::CalculateNumberOfVertices()
{
	unsigned int sizeOfOneVertex = 0;

	for (const VertexAttribute& attrib : vertexAttributes_)
	{
		sizeOfOneVertex += attrib.size;
	}

	return verticesSize_ / sizeOfOneVertex;
}

unsigned int Mesh::CalculateNumberOfIndices()
{
	return indicesSize_ / sizeof(unsigned int);
}

Mesh::~Mesh()
{
	if (bHoldsData_)
	{
		glDeleteVertexArrays(1, &vao_);
		glDeleteBuffers(1, &vbo_);
		if (indices_) {
			glDeleteBuffers(1, &ebo_);
		}
	}
}

void Mesh::SendVertexDataToGPU() noexcept
{
	// create a vertex buffer object & a vertex array object
	glGenBuffers(1, &vbo_);
	glGenVertexArrays(1, &vao_);

	glBindVertexArray(vao_);

		// send vertex data to buffer on the GPU
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, verticesSize_, vertices_, GL_STATIC_DRAW);

		// optionally send index data to buffer on the GPU
		if (indices_)
		{
			glGenBuffers(1, &ebo_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize_, indices_, GL_STATIC_DRAW);
		}

		for (unsigned int index = 0; index < vertexAttributes_.size(); index++)
		{
			// tell GPU there is another vertex attribute
			glEnableVertexAttribArray(index);

			// .. and how it is laid out in the data we just sent
			const VertexAttribute& attrib = vertexAttributes_[index];
			glVertexAttribPointer(index, 
				attrib.size, 
				attrib.type,
				GL_FALSE, 
				attrib.stride,
				attrib.offset
			);
		}

		if (indices_)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	glBindVertexArray(0);

	bHoldsData_ = true;
}

void Mesh::Draw() const noexcept
{
	glBindVertexArray(vao_);

	if (indices_) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glDrawElements(GL_TRIANGLES, numberOfIndices_, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_);
	}
	glBindVertexArray(0);
}

bool Mesh::CheckCollision(const Mesh* other) const noexcept
{
	return false;
}

}
