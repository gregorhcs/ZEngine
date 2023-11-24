#include "GamePong.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RectangleMesh.h"

#define DIST_SMALLEST 0.025f
#define DIST_REGULAR 0.1f
#define DIST_PLAYER_HEIGHT 0.3f

#define DIST_MAXHALF 1.0f
#define DIST_MAX 2.0f

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

	meshPlayerOne_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(-0.9f, - DIST_PLAYER_HEIGHT / 2), DIST_SMALLEST, DIST_PLAYER_HEIGHT);
	meshPlayerTwo_ = resourceManager_.LoadMesh_Rectangle(glm::vec2( 0.9f, - DIST_PLAYER_HEIGHT / 2), DIST_SMALLEST, DIST_PLAYER_HEIGHT);

	// ball

	meshBall_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(-0.5f, -0.7f), 0.5f * DIST_SMALLEST, DIST_SMALLEST);

	// top & bottom border

	resourceManager_.LoadMesh_Rectangle(
		glm::vec2(
			- (DIST_MAXHALF - DIST_SMALLEST), 
			- (DIST_MAXHALF - 1 * DIST_SMALLEST)), 
		DIST_MAX - 2 * DIST_SMALLEST, 
		DIST_SMALLEST);

	resourceManager_.LoadMesh_Rectangle(
		glm::vec2(
			-(DIST_MAXHALF - DIST_SMALLEST),
			+(DIST_MAXHALF - 2 * DIST_SMALLEST)),
		DIST_MAX - 2 * DIST_SMALLEST,
		DIST_SMALLEST);

	// middle dotted line

	for (float z = -DIST_SMALLEST / 2; z > -(DIST_MAXHALF - 2 * DIST_SMALLEST); z -= 2 * DIST_SMALLEST)
	{
		resourceManager_.LoadMesh_Rectangle(
			glm::vec2(
				-DIST_SMALLEST / 2,
				z),
			DIST_SMALLEST / 2,
			DIST_SMALLEST);
	}

	for (float z = -DIST_SMALLEST / 2 + 2*DIST_SMALLEST; z < (DIST_MAXHALF - 2 * DIST_SMALLEST) - DIST_SMALLEST; z += 2 * DIST_SMALLEST)
	{
		resourceManager_.LoadMesh_Rectangle(
			glm::vec2(
				-DIST_SMALLEST / 2,
				z),
			DIST_SMALLEST / 2,
			DIST_SMALLEST);
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

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
		ZMoveMesh(meshPlayerOne_, deltaTime, false);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_UP) == GLFW_PRESS)
		ZMoveMesh(meshPlayerTwo_, deltaTime, true);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
		ZMoveMesh(meshPlayerTwo_, deltaTime, false);
}

void GamePong::MoveBall(double deltaTime)
{
	meshBall_->transform[0] += 0.7f * deltaTime * ballDirection_.x;
	meshBall_->transform[1] += 0.7f * deltaTime * ballDirection_.y;
}

void GamePong::Render(zn::Window& window, double deltaTime)
{
	shaderProgram_->Use();

	for (zn::Mesh* mesh : resourceManager_.GetLoadedMeshes())
	{
		shaderProgram_->SetVec4f("transform", mesh->transform);
		mesh->Draw();
	}
}

void GamePong::ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp)
{
	if (bGoUp)
	{
		mesh->transform[1] += deltaTime * 1.5f;

		if (mesh->transform[1] > 0.8f)
			mesh->transform[1] = 0.8f;
	}
	else
	{
		mesh->transform[1] -= deltaTime * 1.5f;

		if (mesh->transform[1] < -0.8f)
			mesh->transform[1] = -0.8f;
	}
}
