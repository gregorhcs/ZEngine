#include "ScopedGLFWContext.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <stdexcept>

namespace zn {

	ScopedGLFWContext::ScopedGLFWContext(int OpenGLVersionMajor, int OpenGLVersionMinor)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLVersionMinor);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	}

	ScopedGLFWContext::~ScopedGLFWContext()
	{
		glfwTerminate();
	}

}
