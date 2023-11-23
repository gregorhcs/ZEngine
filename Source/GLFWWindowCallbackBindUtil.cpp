#include "GLFWWindowCallbackBindUtil.h"

namespace zn {

GLFWWindowCallbackBindUtil::GLFWWindowCallbackBindUtil() {}

GLFWWindowCallbackBindUtil* GLFWWindowCallbackBindUtil::Instance()
{
	return instance_ ?
		instance_ : instance_ = new GLFWWindowCallbackBindUtil();
}

void GLFWWindowCallbackBindUtil::DispatchCallbackScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	for (CallbackScroll& callback : callbacksScroll_)
	{
		callback(window, xOffset, yOffset);
	}
}

void GLFWWindowCallbackBindUtil::DispatchCallbackMouse(GLFWwindow* window, double xPos, double yPos)
{
	for (CallbackScroll& callback : callbacksMouse_)
	{
		callback(window, xPos, yPos);
	}
}

void GLFWWindowCallbackBindUtil::AddCallbackScroll(CallbackScroll callback)
{
	callbacksScroll_.push_back(callback);
}

void GLFWWindowCallbackBindUtil::AddCallbackMouse(CallbackMouse callback)
{
	callbacksMouse_.push_back(callback);
}

GLFWWindowCallbackBindUtil* GLFWWindowCallbackBindUtil::instance_ = nullptr;

}
