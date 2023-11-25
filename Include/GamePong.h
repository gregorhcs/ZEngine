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

		zn::ShaderProgram* shaderProgram_;

		zn::Mesh* meshPlayerOne_;
		zn::Mesh* meshPlayerTwo_;
		zn::Mesh* meshBall_;

		void LoadScene();

		// ---------------------------
		// ticked functions

		void ProcessInput(zn::Window& window, double deltaTime);
		void MoveBall(double deltaTime);
		void Render(zn::Window& window, double deltaTime);

		// ---------------------------
		// utils

		float ballSpeed_ = 500.f;
		float playerSpeed_ = 550.f;

		glm::vec2 ballDirection_ = glm::vec2(1.f, 0.75f);

		void ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp);

};

