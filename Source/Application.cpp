#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ScopedGLFWContext.h"
#include "Window.h"
#include "GLADLoadUtil.h"
#include "Viewport.h"

namespace zn {

Application::Application(bool bFullScreen) :
	scopedGlfwContext_(ScopedGLFWContext(3, 3)),
	window_(nullptr),
	viewport_(nullptr),
    frameDeltaTime_(0.f),
    frameLastTime_(0.f),
	clearColor_(glm::vec3(0.2f, 0.3f, 0.3f))
{
	// ---------------------------
	// opengl/window setup - the order matters!!

	window_ = bFullScreen ?
		new Window("Learn OpenGl") : new Window(800, 600, "Learn OpenGl");

	LoadGLADAddresses();

	glEnable(GL_DEPTH_TEST);

	viewport_ = new Viewport(window_, 800, 600);
}

Application::~Application()
{
	if (window_) {
		free(window_);
	}
	if (viewport_) {
		free(viewport_);
	}
}

Window& Application::GetWindow()
{
	return *window_;
}

Viewport& Application::GetViewport()
{
	return *viewport_;
}

void Application::SetWindowAndViewportSize(int width, int height)
{
	window_->SetSize(width, height);
	viewport_->SetSize(width, height);
}

void Application::SetClearColor(const glm::vec3& inClearColor)
{
	clearColor_ = inClearColor;
}

void Application::SetWindowTitle(const char* title)
{
	window_->SetTitle(title);
}

void Application::RegisterTickObserver(TickObserver observer)
{
	tickObservers_.push_back(observer);
}

void Application::Run()
{
	// render loop
	while (!glfwWindowShouldClose(window_->GLFWWindow()))
	{
		// update frame delta time
		double frameCurrentTime = glfwGetTime();
		frameDeltaTime_ = frameCurrentTime - frameLastTime_;
		frameLastTime_ = frameCurrentTime;

		// clear screen
		glClearColor(clearColor_.x, clearColor_.y, clearColor_.z, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// callback all tick observers
		for (TickObserver tickObserver : tickObservers_) {
			tickObserver(*this, frameDeltaTime_);
		}

		// poll IO events, swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window_->GLFWWindow());
	}
}

double Application::DeltaTime() const
{
    return frameDeltaTime_;
}

}
