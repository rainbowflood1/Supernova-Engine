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
#include <vector>

const std::vector<char const*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

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
		vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

		void initWindow() {
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			
			window = glfwCreateWindow(width, height, "Supernova", nullptr, nullptr);
		}
		void initVulkan() {
			createInstance();
			setupDebugMessenger();
		}
		
		void createInstance() {
			constexpr vk::ApplicationInfo appInfo{.pApplicationName = "Supernova",
							  .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
							  .pEngineName = "Supernova",
							  .engineVersion = VK_MAKE_VERSION(1, 0, 0),
							  .apiVersion = vk::ApiVersion14};


			// Get the required extensions
			auto requiredExtensions = getRequiredInstanceExtensions();

			auto extensionProperties = context.enumerateInstanceExtensionProperties();

			auto unsupportedPropertyIt = std::ranges::find_if(requiredExtensions, [&extensionProperties](auto const &requiredExtension) {
				return std::ranges::none_of(extensionProperties, [requiredExtension](auto const &extensionProperty) {return strcmp(extensionProperty.extensionName, requiredExtension) == 0;});
			});

			if (unsupportedPropertyIt != requiredExtensions.end()) {
				throw std::runtime_error("Required extension not supported: " + std::string(*unsupportedPropertyIt));
			}

			// Get the required layers
			std::vector<char const*> requiredLayers;
			if (enableValidationLayers) {
				requiredLayers.assign(validationLayers.begin(), validationLayers.end());
			}

			// Check if the layers are supported by Vulkan

			auto layerProperties = context.enumerateInstanceLayerProperties();
			auto unsupportedLayerIt = std::ranges::find_if(requiredLayers,
								       [&layerProperties](auto const& requiredLayer) {
								       	return std::ranges::none_of(layerProperties, [requiredLayer] (auto const &layerProperty) {return strcmp(layerProperty.layerName, requiredLayer) == 0;});
								       });

			if (unsupportedLayerIt != requiredLayers.end()) {
				throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
			}


			vk::InstanceCreateInfo createInfo{
				.pApplicationInfo = &appInfo,
				.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
				.ppEnabledLayerNames = requiredLayers.data(),
				.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
				.ppEnabledExtensionNames = requiredExtensions.data()
			};

			instance = vk::raii::Instance(context, createInfo);
		}

		std::vector<const char*> getRequiredInstanceExtensions() {
			uint32_t glfwExtensionCount = 0;
			auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
			if (enableValidationLayers) {
				extensions.push_back(vk::EXTDebugUtilsExtensionName);
			}

			return extensions;
		}

		static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
								      vk::DebugUtilsMessageTypeFlagsEXT type,
								      const vk::DebugUtilsMessengerCallbackDataEXT * pCallbackData,
								      void * pUserData) {
			std::cerr << "Validation layer: type" << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

			return vk::False;
		}

		void setupDebugMessenger() {
			if (!enableValidationLayers) return;

			vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
									    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
			vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

			vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{.messageSeverity = severityFlags,
											      .messageType = messageTypeFlags,
											      .pfnUserCallback = &debugCallback};

			debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
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
