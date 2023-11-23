#include "Viewport.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zn {

Viewport::Viewport(Window* owner, int width, int height) :
	owningWindow(owner)
{
	glViewport(0, 0, width, height);

	SetCallbackFramebufferSize(&Viewport::Handle_FramebufferSizeChanged);
}

void Viewport::SetSize(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Viewport::SetCallbackFramebufferSize(CallbackFramebufferSize callbackFramebufferSize)
{
	glfwSetFramebufferSizeCallback(owningWindow->GLFWWindow(), callbackFramebufferSize);
}

Window& Viewport::OwningWindow() const noexcept
{
	return *owningWindow;
}

void Viewport::Handle_FramebufferSizeChanged(GLFWwindow* window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

}
