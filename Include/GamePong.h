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
		void MoveBall(double deltaTime);
		void Render(zn::Window& window, double deltaTime);

		// ---------------------------
		// game state

		char playerOneScore_ = '0';
		char playerTwoScore_ = '0';

		// ---------------------------
		// gameplay

		float ballSpeed_ = 500.f;
		float playerSpeed_ = 550.f;

		glm::vec2 ballDirection_ = glm::vec2(1.f, 0.75f);

		void ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp);

		void CheckBallCollision();

};

