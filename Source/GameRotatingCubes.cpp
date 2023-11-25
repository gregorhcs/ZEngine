#include "GameRotatingCubes.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "ResourceManager.h"

glm::vec3 floatingCubesPositions[10] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

GameRotatingCubes::GameRotatingCubes() : Application(false)
{
	GetWindow().AddCallbackScroll([this](GLFWwindow* window, double xOffset, double yOffset) {
		HandleScrollInput(window, xOffset, yOffset);
		});
	GetWindow().AddCallbackMouse([this](GLFWwindow* window, double xPos, double yPos) {
		HandleMouseInput(window, xPos, yPos);
		});

	RegisterTickObserver([this](Application& app, double deltaTime) {
		ProcessInput(app.GetWindow(), deltaTime);
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		Render(app.GetWindow(), deltaTime);
		});

	LoadScene();
}

void GameRotatingCubes::LoadScene()
{
	shaderProgram_ = resourceManager_.LoadShaderProgram(
		"res/CubeWithTexture.vert",
		"res/CubeWithTexture.frag"
	);

	usedTextures_.push_back(resourceManager_.LoadTexture("res/container.jpg", GL_TEXTURE0, GL_RGB));
	usedTextures_.push_back(resourceManager_.LoadTexture("res/awesomeface.png", GL_TEXTURE1, GL_RGBA));

	shaderProgram_->Use();
	shaderProgram_->SetTextureUniforms(static_cast<unsigned int>(usedTextures_.size()));

	cube_ = resourceManager_.LoadMesh_CubeWithTexture();
	cube_->SendVertexDataToGPU();
}

void GameRotatingCubes::ProcessInput(zn::Window& window, double deltaTime)
{
	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window.GLFWWindow(), true);

	double cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
		camera_.Move(zn::Camera::FORWARDS, cameraSpeed);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
		camera_.Move(zn::Camera::BACKWARDS, cameraSpeed);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
		camera_.Move(zn::Camera::LEFT, cameraSpeed);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
		camera_.Move(zn::Camera::RIGHT, cameraSpeed);
}


void GameRotatingCubes::Render(zn::Window& window, double deltaTime)
{
	for (zn::Texture* texture : usedTextures_)
	{
		texture->ActivateAndBind();
	}

	shaderProgram_->SetMat4f("view", camera_.View());
	shaderProgram_->SetMat4f("projection", camera_.Projection(800, 600));

	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, floatingCubesPositions[i]);
		model = glm::rotate(model, static_cast<GLfloat>(glfwGetTime()) * glm::radians(90.f) + 20.f * i, glm::vec3(0.5f, 1.0f, 0.f));
		shaderProgram_->SetMat4f("model", model);

		shaderProgram_->Use();
		cube_->Draw();
	}
}

//
void GameRotatingCubes::HandleScrollInput(GLFWwindow* window, double xOffset, double yOffset)
{
	camera_.Zoom(yOffset);
}

//
void GameRotatingCubes::HandleMouseInput(GLFWwindow* window, double xPos, double yPos)
{
	if (!firstMouseEncountered_) {

		mouseLastX_ = xPos;
		mouseLastY_ = yPos;

		firstMouseEncountered_ = true;

	}

	double mouseXOffset = xPos - mouseLastX_;
	double mouseYOffset = yPos - mouseLastY_;

	mouseLastX_ = xPos;
	mouseLastY_ = yPos;

	camera_.Look(mouseXOffset, mouseYOffset);
}
