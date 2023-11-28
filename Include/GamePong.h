#pragma once

#include "Application.h"
#include "ShaderProgram.h"

class GamePong : public zn::Application
{
	public:

		GamePong();

	protected:

		// ---------------------------
		// scene

		const float APP_WIDTH = 1920.f;
		const float APP_HEIGHT = 1080.f;

		const float DIST_SMALLEST = 7.f;
		const float DIST_MEDIUM = 11.f;
		const float DIST_LARGE = 16.f;

		const float DIST_PLAYER_HEIGHT = 150.f;

		zn::ResourceManager resourceManager_;

		zn::ShaderProgram* shaderProgramNoTexture_;
		zn::ShaderProgram* shaderProgramFont_;

		zn::Font* font_;

		zn::Mesh* meshPlayerOneScore_;
		zn::Mesh* meshPlayerTwoScore_;
		zn::Mesh* meshBorderTop_;
		zn::Mesh* meshBorderBot_;
		zn::Mesh* meshPlayerOne_;
		zn::Mesh* meshPlayerTwo_;
		zn::Mesh* meshBall_;
		zn::Mesh* meshWinBarLeft;
		zn::Mesh* meshWinBarRight;

		void LoadScene();

		// ---------------------------
		// ticked functions

		void ProcessInput(zn::Window& window, double deltaTime);
		void CheckBallCollision();
		void Render(zn::Window& window, double deltaTime);
		void MoveBall(double deltaTime);

		// ---------------------------
		// game state

		char playerOneScore_ = '0';
		char playerTwoScore_ = '0';

		// ---------------------------
		// gameplay

		const float ballSpeedAcceleration_ = 10.f;
		const float ballInitialSpeed_ = 500.f;
		float ballSpeed_ = 500.f;

		const float ballPlayerYAdjustmentScale = 2.f;

		const float playerSpeed_ = 650.f;
		const float playerMaxY_ = APP_HEIGHT - 2 * DIST_MEDIUM - DIST_PLAYER_HEIGHT;
		const float playerMinY_ = 3 * DIST_MEDIUM;

		const glm::vec2 ballInitialDirection_ = glm::vec2(1.f, 0.75f);
		glm::vec2 ballDirection_ = glm::vec2(1.f, 0.75f);

		void ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp);

		void HandleOnBorderCollided();
		void HandleOnPlayerCollided(const zn::Mesh* playerMesh);
		void HandleOnWinBarCollided(const zn::Mesh* mesh);

};

