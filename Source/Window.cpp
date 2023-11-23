#include "Window.h"

#include <stdexcept>

#include <GLFW/glfw3.h>

#include "GLFWWindowCallbackBindUtil.h"

namespace zn {

void HandleScrollInput(GLFWwindow* window, double xOffset, double yOffset) 
{
	GLFWWindowCallbackBindUtil::Instance()->DispatchCallbackScroll(window, xOffset, yOffset);
};
void HandleMouseInput(GLFWwindow* window, double xPos, double yPos)
{
	GLFWWindowCallbackBindUtil::Instance()->DispatchCallbackMouse(window, xPos, yPos);
};


Window::Window(int width, int height, const char* title) :
	Window(CreateRegularGLFWWindow(width, height, title))
{}

Window::Window(const char* title) :
	Window(CreateFullScreenGLFWWindow(title))
{}

Window::Window(GLFWwindow* GLFWWindow) :
	GLFWWindow_(GLFWWindow)
{
	if (!GLFWWindow_) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window.");
	}

	MakeCurrentContext();

	glfwSetInputMode(GLFWWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetScrollCallback(GLFWWindow_, &HandleScrollInput);
	glfwSetCursorPosCallback(GLFWWindow_, &HandleMouseInput);
}

Window::~Window() noexcept 
{
	glfwDestroyWindow(GLFWWindow_);
}

void Window::SetSize(int width, int height)
{
	glfwSetWindowSize(GLFWWindow_, width, height);
}

void Window::SetTitle(const char* title)
{
	glfwSetWindowTitle(GLFWWindow_, title);
}

void Window::MakeCurrentContext()
{
	glfwMakeContextCurrent(GLFWWindow_);
}

void Window::AddCallbackScroll(CallbackScroll callbackScroll)
{
	GLFWWindowCallbackBindUtil::Instance()->AddCallbackScroll(callbackScroll);
}

void Window::AddCallbackMouse(CallbackMouse callbackMouse)
{
	GLFWWindowCallbackBindUtil::Instance()->AddCallbackMouse(callbackMouse);
}

GLFWwindow* Window::GLFWWindow() const noexcept
{
	return GLFWWindow_;
}

GLFWwindow* Window::CreateRegularGLFWWindow(int width, int height, const char* title)
{
	return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

GLFWwindow* Window::CreateFullScreenGLFWWindow(const char* title)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	return glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);
}

}
