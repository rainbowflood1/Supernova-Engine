#ifndef SUPERNOVA_ENGINE_INCLUDED
#define SUPERNOVA_ENGINE_INCLUDED

#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace Supernova {
	class Supernova {
	public:
		uint32_t width = 800;
		uint32_t height = 600;
		void run() {
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}
	private:
		GLFWwindow* window = nullptr;
		void initWindow() {
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			
			window = glfwCreateWindow(width, height, "Supernova", nullptr, nullptr);
		}
		void initVulkan() {}
		void mainLoop() {
			while (!glfwWindowShouldClose(window)) {
				glfwPollEvents();
			}
		}
		void cleanup() {
			glfwDestroyWindow(window);
			glfwTerminate();
		}
	};
};

#endif
