#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

extern const bool ENABLE_VALIDATION_LAYERS;
extern const std::vector<const char *> VALIDATION_LAYERS;