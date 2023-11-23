#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>

struct GLFWwindow;

namespace zn {

class Window {

	public:

		typedef std::function<void(GLFWwindow*, double, double)> CallbackScroll;
		typedef std::function<void(GLFWwindow*, double, double)> CallbackMouse;

		// Creates a window and sets viewport settings.
		// Throws runtime_error if shader GLFW window could not be created.
		Window(int width, int height, const char* title);

		// Creates a fullscreen window and sets viewport settings.
		// Throws runtime_error if shader GLFW window could not be created.
		Window(const char* title);

		~Window() noexcept;

		void SetSize(int width, int height);
		void SetTitle(const char* title);

		void MakeCurrentContext();

		void AddCallbackScroll(CallbackScroll callbackScroll);
		void AddCallbackMouse(CallbackMouse callbackMouse);

		GLFWwindow* GLFWWindow() const noexcept;

	protected:

		Window(GLFWwindow* GLFWWindow);

		GLFWwindow* CreateRegularGLFWWindow(int width, int height, const char* title);
		GLFWwindow* CreateFullScreenGLFWWindow(const char* title);

	private:

		GLFWwindow* GLFWWindow_;

};

}
