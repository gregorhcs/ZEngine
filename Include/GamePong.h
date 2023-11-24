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
		void Render(zn::Window& window, double deltaTime);

		// ---------------------------
		// utils

		void ZMoveMesh(zn::Mesh* mesh, double deltaTime, bool bGoUp);

};

