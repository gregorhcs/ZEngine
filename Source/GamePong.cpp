#include "GamePong.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RectangleMesh.h"
#include <iostream>

#define DIST_SMALLEST 5.f
#define DIST_MEDIUM 10.f
#define DIST_LARGE 20.f

#define DIST_PLAYER_HEIGHT 100.f

GamePong::GamePong() :
	Application(true)
{
	SetClearColor(glm::vec3(0.f, 0.f, 0.f));
	SetWindowAndViewportSize(1920, 1080);
	SetWindowTitle("Pong");

	RegisterTickObserver([this](Application& app, double deltaTime) {
		ProcessInput(app.GetWindow(), deltaTime);
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		CheckBallCollision();
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		Render(app.GetWindow(), deltaTime);
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		MoveBall(deltaTime);
		});

	glm::normalize(ballDirection_);

	LoadScene();
}

void GamePong::LoadScene()
{
	shaderProgram_ = resourceManager_.LoadShaderProgram(
		"Resources/Rectangle.vert",
		"Resources/Rectangle.frag"
	);

	shaderProgram_->Use();

	// players

	meshPlayerOne_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(3*DIST_MEDIUM,			height_/2 - DIST_PLAYER_HEIGHT / 2), DIST_MEDIUM, DIST_PLAYER_HEIGHT);
	meshPlayerOne_->bHasCollision = true;

	meshPlayerTwo_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(width_ - 4* DIST_MEDIUM, height_/2 - DIST_PLAYER_HEIGHT / 2), DIST_MEDIUM, DIST_PLAYER_HEIGHT);
	meshPlayerTwo_->bHasCollision = true;

	// ball

	meshBall_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(50.f, 50.f), DIST_MEDIUM, DIST_MEDIUM);
	meshBall_->bHasCollision = true;

	// top & bottom border

	meshBorderTop_ = resourceManager_.LoadMesh_Rectangle(
		glm::vec2(
			DIST_MEDIUM,
			2 * DIST_MEDIUM),
		width_ - 2* DIST_MEDIUM,
		DIST_MEDIUM);
	meshBorderTop_->bHasCollision = true;

	meshBorderBot_ = resourceManager_.LoadMesh_Rectangle(
		glm::vec2(
			DIST_MEDIUM,
			height_ - 2* DIST_MEDIUM),
		width_ - 2 * DIST_MEDIUM,
		DIST_MEDIUM);
	meshBorderBot_->bHasCollision = true;

	// middle dotted line

	for (float z = height_ /2 - DIST_MEDIUM; z > 3 * DIST_MEDIUM; z -= 2 * DIST_MEDIUM)
	{
		resourceManager_.LoadMesh_Rectangle(
			glm::vec2(
				width_ /2 - DIST_MEDIUM / 2,
				z),
			DIST_MEDIUM,
			DIST_MEDIUM);
	}

	for (float z = height_ / 2 + DIST_MEDIUM; z < height_ - 3 * DIST_MEDIUM; z += 2 * DIST_MEDIUM)
	{
		resourceManager_.LoadMesh_Rectangle(
			glm::vec2(
				width_ / 2 - DIST_MEDIUM / 2,
				z),
			DIST_MEDIUM,
			DIST_MEDIUM);
	}

	// ----

	for (zn::Mesh* mesh : resourceManager_.GetLoadedMeshes()) 
	{
		mesh->SendVertexDataToGPU();
	}
}

void GamePong::ProcessInput(zn::Window& window, double deltaTime)
{
	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window.GLFWWindow(), true);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
		ZMoveMesh(meshPlayerOne_, deltaTime, true);
	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
		ZMoveMesh(meshPlayerOne_, deltaTime, true);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
		ZMoveMesh(meshPlayerOne_, deltaTime, false);
	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
		ZMoveMesh(meshPlayerOne_, deltaTime, false);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_UP) == GLFW_PRESS)
		ZMoveMesh(meshPlayerTwo_, deltaTime, true);
	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
		ZMoveMesh(meshPlayerTwo_, deltaTime, true);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
		ZMoveMesh(meshPlayerTwo_, deltaTime, false);
	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
		ZMoveMesh(meshPlayerTwo_, deltaTime, false);
}

void GamePong::MoveBall(double deltaTime)
{
	meshBall_->position_[0] += static_cast<float>(ballSpeed_ * deltaTime * ballDirection_.x);
	meshBall_->position_[1] += static_cast<float>(ballSpeed_ * deltaTime * ballDirection_.y);
}

void GamePong::Render(zn::Window& window, double deltaTime)
{
	shaderProgram_->Use();
	shaderProgram_->SetMat4f("projection", glm::ortho(0.f, width_, height_, 0.f, -1.f, 1.f));

	for (zn::Mesh* mesh : resourceManager_.GetLoadedMeshes())
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, mesh->position_);
		model = glm::scale(model, mesh->scale_);
		shaderProgram_->SetMat4f("model", model);

		mesh->Draw();
	}
}

void GamePong::ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp)
{
	if (bGoUp)
	{
		mesh->position_.y -= static_cast<float>(deltaTime * playerSpeed_);

		if (mesh->position_.y < 0.05f * height_)
			mesh->position_.y = 0.05f * height_;
	}
	else
	{
		mesh->position_.y += static_cast<float>(deltaTime * playerSpeed_);

		if (mesh->position_.y > 0.95f * height_ - DIST_PLAYER_HEIGHT)
			mesh->position_.y = 0.95f * height_ - DIST_PLAYER_HEIGHT;
	}
}

void GamePong::CheckBallCollision()
{
	for (const zn::Mesh* mesh : resourceManager_.GetLoadedMeshes())
	{
		if (mesh != meshBall_ && mesh->bHasCollision)
		{
			if (meshBall_->CheckCollision(mesh))
			{
				if (mesh == meshBorderBot_ || mesh == meshBorderTop_)
					ballDirection_.y *= -1.f;

				if (mesh == meshPlayerOne_ || mesh == meshPlayerTwo_)
					ballDirection_.x *= -1.f;
			}
		}
	}
}
