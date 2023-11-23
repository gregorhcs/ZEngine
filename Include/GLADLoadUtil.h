#pragma once

#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zn {

inline void LoadGLADAddresses() {
	// initialize GLAD: load all GL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD!");
	}
};

}
