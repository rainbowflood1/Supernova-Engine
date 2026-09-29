#ifndef SUPERNOVA_ENGINE_INCLUDED
#define SUPERNOVA_ENGINE_INCLUDED

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
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
		vk::raii::Context context;
		vk::raii::Instance instance = nullptr;

		void initWindow() {
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			
			window = glfwCreateWindow(width, height, "Supernova", nullptr, nullptr);
		}
		void initVulkan() {
			createInstance();
		}
		
		void createInstance() {
			constexpr vk::ApplicationInfo appInfo{.pApplicationName = "Supernova",
							  .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
							  .pEngineName = "Supernova",
							  .engineVersion = VK_MAKE_VERSION(1, 0, 0),
							  .apiVersion = vk::ApiVersion14};

			uint32_t glfwExtensionCount = 0;
			auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			auto extensionProperties = context.enumerateInstanceExtensionProperties();

			for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
				if (std::ranges::none_of(extensionProperties,
							 [glfwExtension = glfwExtensions[i]] (auto const& extensionProperty)
							 {return strcmp(extensionProperty.extensionName, glfwExtension) == 0; }))
				{
					throw std::runtime_error("Required GLFW extension not supported " + std::string(glfwExtensions[i]));
				}
			}

			vk::InstanceCreateInfo createInfo{
				.pApplicationInfo = &appInfo,
				.enabledExtensionCount = glfwExtensionCount,
				.ppEnabledExtensionNames = glfwExtensions
			};

			instance = vk::raii::Instance(context, createInfo);
		}

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
