#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

namespace zn {

class Viewport {

	typedef void CallbackFramebufferSize(GLFWwindow*, int, int);

	public:

		Viewport(Window* owner, int width, int height);

		void SetSize(int width, int height);

		void SetCallbackFramebufferSize(CallbackFramebufferSize callbackFramebufferSize);

		Window& OwningWindow() const noexcept;

	private:

		Window* owningWindow;

		// callback targeting the case, that the framebuffer size is changed
		// results in adjusting the viewport to the same size
		static void Handle_FramebufferSizeChanged(GLFWwindow* window, int width, int height);

};

}
