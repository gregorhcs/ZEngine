#include "GamePong.h"

#include <glm/gtc/matrix_transform.hpp>

#include "stb/stb_image.h"

#include "Rectangle.h"
#include "Font.h"

#include <iostream>

bool save_screenshot(std::string filename, int w, int h)
{
	//This prevents the images getting padded 
   // when the width multiplied by 3 is not a multiple of 4
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = w * h * 3;
	// First let's create our buffer, 3 channels per Pixel
	char* dataBuffer = (char*)malloc(nSize * sizeof(char));

	if (!dataBuffer) return false;

	// Let's fetch them from the backbuffer	
	// We request the pixels in GL_BGR format, thanks to Berzeger for the tip
	glReadPixels((GLint)0, (GLint)0,
		(GLint)w, (GLint)h,
		GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);

	//Now the file creation
	#pragma warning(suppress : 4996)
	FILE* filePtr = fopen(filename.c_str(), "wb");
	if (!filePtr) return false;


	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6] = { w % 256,w / 256,
					h % 256,h / 256,
					24,0 };
	// We write the headers
	fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	fwrite(header, sizeof(unsigned char), 6, filePtr);
	// And finally our image data
	fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	fclose(filePtr);

	free(dataBuffer);

	return true;
}

GamePong::GamePong() :
	Application(true)
{
	SetClearColor(glm::vec3(0.f, 0.f, 0.f));
	SetWindowAndViewportSize(APP_WIDTH, APP_HEIGHT);
	SetWindowTitle("Pong");

	GLFWimage images[2];
	images[0].pixels = stbi_load("Resources/ZPongIcon16x16.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("Resources/ZPongIcon32x32.png", &images[1].width, &images[1].height, 0, 4);
	glfwSetWindowIcon(GetWindow().GLFWWindow(), 1, images);
	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);

	RegisterTickObserver([this](Application& app, double deltaTime) {
		MoveBall(deltaTime);
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		CheckBallCollision();
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		Render(app.GetWindow(), deltaTime);
		});
	RegisterTickObserver([this](Application& app, double deltaTime) {
		ProcessInput(app.GetWindow(), deltaTime);
		});

	glm::normalize(ballDirection_);

	LoadScene();
}

void GamePong::LoadScene()
{
	shaderProgramNoTexture_ = resourceManager_.LoadShaderProgram(
		"Resources/Rectangle.vert",
		"Resources/Rectangle.frag"
	);

	shaderProgramFont_ = resourceManager_.LoadShaderProgram(
		"Resources/CubeWithTexture.vert",
		"Resources/CubeWithTexture.frag"
	);
	shaderProgramFont_->Use();
	shaderProgramFont_->SetTextureUniforms(1);

	font_ = resourceManager_.LoadFont("Resources/CascadiaMono/");

	// player scores

	meshPlayerOneScore_ = resourceManager_.LoadMesh_CubeWithTexture();
	meshPlayerOneScore_->position_ = glm::vec3(-0.15f, 0.5f, 0.0f);

	meshPlayerTwoScore_ = resourceManager_.LoadMesh_CubeWithTexture();
	meshPlayerTwoScore_->position_ = glm::vec3(0.85f, 0.5f, 0.0f);

	// players

	meshPlayerOne_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(         2 * DIST_MEDIUM + DIST_LARGE, height_/2 - DIST_PLAYER_HEIGHT / 2), DIST_LARGE, DIST_PLAYER_HEIGHT);
	meshPlayerOne_->bHasCollision = true;

	meshPlayerTwo_ = resourceManager_.LoadMesh_Rectangle(glm::vec2(width_ - 2 * DIST_MEDIUM - DIST_LARGE, height_/2 - DIST_PLAYER_HEIGHT / 2), DIST_LARGE, DIST_PLAYER_HEIGHT);
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

	// win bars

	meshWinBarLeft = resourceManager_.LoadMesh_Rectangle(
		glm::vec2(
			- DIST_SMALLEST,
			0.f),
		DIST_SMALLEST,
		height_);
	meshWinBarLeft->bHasCollision = true;

	meshWinBarRight = resourceManager_.LoadMesh_Rectangle(
		glm::vec2(
			width_,
			0.f),
		DIST_SMALLEST,
		height_);
	meshWinBarRight->bHasCollision = true;

	// middle dotted line

	for (float z = height_ /2 - DIST_MEDIUM; z > 3 * DIST_MEDIUM; z -= 2 * DIST_MEDIUM)
	{
		resourceManager_.LoadMesh_Rectangle(
			glm::vec2(width_ /2 - DIST_MEDIUM / 2, z),
			DIST_MEDIUM,
			DIST_MEDIUM);
	}

	for (float z = height_ / 2 + DIST_MEDIUM; z < height_ - 3 * DIST_MEDIUM; z += 2 * DIST_MEDIUM)
	{
		resourceManager_.LoadMesh_Rectangle(
			glm::vec2(width_ / 2 - DIST_MEDIUM / 2, z),
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

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_F12) == GLFW_PRESS)
		save_screenshot("ZScreenshot.tga", width_, height_);

	if (glfwGetKey(window.GLFWWindow(), GLFW_KEY_N) == GLFW_PRESS)
	{
		playerOneScore_ = playerTwoScore_ = '0';
		ballSpeed_ = ballInitialSpeed_;
		ballDirection_ = ballInitialDirection_;

		meshBall_->Reset();
		meshPlayerOne_->Reset();
		meshPlayerTwo_->Reset();
	}

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
	ballSpeed_ += deltaTime * ballSpeedAcceleration_;

	meshBall_->position_[0] += static_cast<float>(ballSpeed_ * deltaTime * ballDirection_.x);
	meshBall_->position_[1] += static_cast<float>(ballSpeed_ * deltaTime * ballDirection_.y);
}

void GamePong::Render(zn::Window& window, double deltaTime)
{
	shaderProgramNoTexture_->Use();
	shaderProgramNoTexture_->SetMat4f("projection", glm::ortho(0.f, width_, height_, 0.f, -1.f, 1.f));

	shaderProgramFont_->Use();
	shaderProgramFont_->SetMat4f("view", zn::Camera().View());
	shaderProgramFont_->SetMat4f("projection", zn::Camera().Projection(width_, height_));

	for (zn::Mesh* mesh : resourceManager_.GetLoadedMeshes())
	{
		zn::ShaderProgram* usedShaderProgram = nullptr;
		zn::Texture* usedTexture = nullptr;

		// mesh specific code
		if (mesh == meshPlayerOneScore_ || mesh == meshPlayerTwoScore_)
		{
			usedShaderProgram = shaderProgramFont_;
			usedTexture = font_->FindTextureOfChar(mesh == meshPlayerOneScore_ ? playerOneScore_ : playerTwoScore_);
		}
		else {
			usedShaderProgram = shaderProgramNoTexture_;
		}
		// ~ mesh specific code


		if (usedShaderProgram) {
			usedShaderProgram->Use();
			usedShaderProgram->SetMat4f("model", glm::scale(glm::translate(glm::mat4(1.f), mesh->position_), mesh->scale_));
		}

		if (usedTexture) {
			usedTexture->ActivateAndBind();
		}

		mesh->Draw();
	}
}

void GamePong::ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp)
{
	if (bGoUp)
	{
		mesh->position_.y -= static_cast<float>(deltaTime * playerSpeed_);

		if (mesh->position_.y < playerMinY_)
			mesh->position_.y = playerMinY_;
	}
	else
	{
		mesh->position_.y += static_cast<float>(deltaTime * playerSpeed_);

		if (mesh->position_.y > playerMaxY_)
			mesh->position_.y = playerMaxY_;
	}
}

void GamePong::HandleOnBorderCollided()
{
	ballDirection_.y *= -1.f;
}

void GamePong::HandleOnPlayerCollided(const zn::Mesh* playerMesh)
{
	const float yPlayerBallCollisionRange = DIST_PLAYER_HEIGHT + 2 * DIST_MEDIUM;

	const zn::Mesh* yPlayerPosition = playerMesh == meshPlayerOne_ ? meshPlayerOne_ : meshPlayerTwo_;

	// calc mid y positions
	const float playerMidY = yPlayerPosition->position_.y + DIST_PLAYER_HEIGHT / 2;
	const float ballMidY   = meshBall_->position_.y       + DIST_MEDIUM / 2;

	const float yDirectionAdjustmentFactor = abs(ballMidY-playerMidY) / abs(yPlayerBallCollisionRange/2);

	ballDirection_.x *= -1.f;
	ballDirection_.y = ballPlayerYAdjustmentScale * yDirectionAdjustmentFactor;

	if (yDirectionAdjustmentFactor > 0.82f) {
		ballDirection_.y *= -1;
	}
}

void GamePong::HandleOnWinBarCollided(const zn::Mesh* mesh)
{
	if (mesh == meshWinBarLeft)
	{
		if (playerTwoScore_ < '9')
			playerTwoScore_++;

		meshBall_->position_ = glm::vec3(width_ - 100.f, 100.f, 0.f);

		ballSpeed_ = ballInitialSpeed_;
		ballDirection_ = glm::vec2(-1.f, -0.2f);
	}
	else if (mesh == meshWinBarRight)
	{
		if (playerOneScore_ < '9')
			playerOneScore_++;

		meshBall_->position_ = glm::vec3(100.f, 100.f, 0.f);

		ballSpeed_ = ballInitialSpeed_;
		ballDirection_ = glm::vec2(1.f, -0.2f);
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
					HandleOnBorderCollided();

				if (mesh == meshPlayerOne_ || mesh == meshPlayerTwo_)
					HandleOnPlayerCollided(mesh);

				if (mesh == meshWinBarLeft || mesh == meshWinBarRight)
					HandleOnWinBarCollided(mesh);
			}
		}
	}
}
