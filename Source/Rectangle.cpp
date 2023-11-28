#include "Rectangle.h"

float rectangleVertexData_[8] = {
	1.f, 0.f,  // top right
	1.f, 1.f,  // bottom right
	0.f, 1.f,  // bottom left
	0.f, 0.f,  // top left 
};

unsigned int rectangleIndices_[6] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

std::vector<zn::VertexAttribute> rectangleVertexAttributes{
	zn::VertexAttribute{ 2, GL_FLOAT, 2 * sizeof(GLfloat), nullptr }
};

zn::RectangleMesh::RectangleMesh(const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight) :
	RectangleMesh(
		topLeft,
		topRight.x - topLeft.x,
		bottomRight.y - topRight.y
	)
{}

zn::RectangleMesh::RectangleMesh(const glm::vec2& topLeft, float width, float height) :
	Mesh(
		rectangleVertexData_,
		sizeof(rectangleVertexData_),
		rectangleIndices_,
		sizeof(rectangleIndices_),
		rectangleVertexAttributes
	)
{
	position_ = initialPosition_ = glm::vec3(topLeft, 0.f);
	scale_ = glm::vec3(width, height, 0.f);
}

zn::RectangleMesh::RectangleMesh() :
	RectangleMesh(glm::vec2(-0.5f, -0.5f), 1.0f, 1.0f)
{}

glm::vec2 zn::RectangleMesh::TopLeft() const noexcept
{
	return glm::vec2(position_.x, position_.y);
}

glm::vec2 zn::RectangleMesh::TopRight() const noexcept
{
	return glm::vec2(position_.x + scale_.x, position_.y);
}

glm::vec2 zn::RectangleMesh::BottomLeft() const noexcept
{
	return glm::vec2(position_.x, position_.y + scale_.y);
}

glm::vec2 zn::RectangleMesh::BottomRight() const noexcept
{
	return glm::vec2(position_.x + scale_.x, position_.y + scale_.y);
}

float zn::RectangleMesh::Top() const noexcept
{
	return position_.y;
}

float zn::RectangleMesh::Bottom() const noexcept
{
	return position_.y + scale_.y;
}

float zn::RectangleMesh::Left() const noexcept
{
	return position_.x;
}

float zn::RectangleMesh::Right() const noexcept
{
	return position_.x + scale_.x;
}

bool zn::RectangleMesh::CheckCollision(const Mesh* other) const noexcept
{
	const bool bCollisionX = Left() <= other->Right()
								    && other->Left() <= Right();

	const bool bCollisionY = Top() <= other->Bottom()
								   && other->Top() <= Bottom();

	return bCollisionX && bCollisionY;
}
