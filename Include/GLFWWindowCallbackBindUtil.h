#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>
#include <functional>

namespace zn {

class GLFWWindowCallbackBindUtil
{
	public:

		typedef std::function<void(GLFWwindow*, double, double)> CallbackScroll;
		typedef std::function<void(GLFWwindow*, double, double)> CallbackMouse;

		GLFWWindowCallbackBindUtil();
        GLFWWindowCallbackBindUtil(GLFWWindowCallbackBindUtil& other) = delete;
        void operator=(const GLFWWindowCallbackBindUtil&) = delete;

		static GLFWWindowCallbackBindUtil* Instance();

		void DispatchCallbackScroll(GLFWwindow*, double, double);
		void DispatchCallbackMouse(GLFWwindow*, double, double);

		void AddCallbackScroll(CallbackScroll);
		void AddCallbackMouse(CallbackMouse);

	private:

		static GLFWWindowCallbackBindUtil* instance_;

		std::vector<CallbackScroll> callbacksScroll_;
		std::vector<CallbackScroll> callbacksMouse_;

};

}
