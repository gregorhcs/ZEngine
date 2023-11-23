#pragma once

#include "Window.h"

#include <vector>
#include <functional>

#include "ScopedGLFWContext.h"
#include "Viewport.h"

#include "Camera.h"

namespace zn {

class Application
{
	typedef std::function<void(Application&, double)> TickObserver;

	public:

		Application(bool bFullScreen);
		~Application();

		Window& GetWindow();
		Viewport& GetViewport();

		void SetWindowAndViewportSize(int width, int height);
		void SetClearColor(const glm::vec3& inClearColor);
		void SetWindowTitle(const char* title);

		void RegisterTickObserver(TickObserver);

		void Run();
		double DeltaTime() const;

	private:

		// ---------------------------
		// opengl/window setup

		ScopedGLFWContext scopedGlfwContext_;
		Window* window_;
		Viewport* viewport_;

		// ---------------------------
		// main application loop

		double frameDeltaTime_;
		double frameLastTime_;

		glm::vec3 clearColor_;

		std::vector<TickObserver> tickObservers_;

};

}
