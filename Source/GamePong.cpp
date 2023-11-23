#include "GamePong.h"

#include "RectangleMesh.h"

#define DIST_SMALLEST 0.025f
#define DIST_REGULAR 0.1f
#define DIST_PLAYER_HEIGHT 0.3f

#define DIST_MAXHALF 1.0f
#define DIST_MAX 2.0f

GamePong::GamePong() :
	Application(false)
{
	SetClearColor(glm::vec3(0.f, 0.f, 0.f));
	SetWindowAndViewportSize(1000, 500);
	SetWindowTitle("Pong");

	RegisterTickObserver([this](Application& app, double deltaTime) {
		ProcessInput(app.GetWindow(), deltaTime);
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		Render(app.GetWindow(), deltaTime);
		});

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
}

void GamePong::Render(zn::Window& window, double deltaTime)
{
	shaderProgram_->Use();

	for (zn::Mesh* mesh : resourceManager_.GetLoadedMeshes())
	{
		mesh->Draw();
	}
}
