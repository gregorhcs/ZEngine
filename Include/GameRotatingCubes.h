#pragma once

#include "Application.h"

#include "ResourceManager.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Camera.h"

class GameRotatingCubes : public zn::Application
{
	public:

		GameRotatingCubes();

	protected:

		// ---------------------------
		// scene

		zn::ResourceManager resourceManager_;

		std::vector<zn::Texture*> usedTextures_;
		zn::ShaderProgram* shaderProgram_;
		zn::Mesh* cube_;

		void LoadScene();

		// ---------------------------
		// camera / input

		zn::Camera camera_;

		double mouseLastX_;
		double mouseLastY_;

		GLboolean firstMouseEncountered_ = false;

		void HandleScrollInput(GLFWwindow*, double, double);
		void HandleMouseInput(GLFWwindow*, double, double);

		// ---------------------------
		// ticked functions

		void ProcessInput(zn::Window& window, double deltaTime);
		void Render(zn::Window& window, double deltaTime);
};

