#ifndef SUPERNOVA_ENGINE_INCLUDED
#define SUPERNOVA_ENGINE_INCLUDED

#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

namespace Supernova {
	class Supernova {
	public:
		void run() {
			
		}
	};
};

#endif
