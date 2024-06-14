#pragma once

#include <iostream>
#include <memory>

#include <glslang/Public/ShaderLang.h>

#include "device_queue.h"

class ShaderCompileEnv
{
public:
    static void initialize()
    {
        static ShaderCompileEnv env;
        initialized = true;
    }

    static bool is_initialized()
    {
        return initialized;
    }

private:
    static bool initialized;

    ShaderCompileEnv()
    {
        std::cout << "glslang version: " << glslang::GetEsslVersionString() << "(essl), "
                  << glslang::GetGlslVersionString() << "(glsl)" << std::endl;
        glslang::InitializeProcess();
    }

    ShaderCompileEnv(const ShaderCompileEnv &) = delete;
    ShaderCompileEnv &operator=(const ShaderCompileEnv &) = delete;
    ShaderCompileEnv(const ShaderCompileEnv &&) = delete;
    ShaderCompileEnv &operator=(const ShaderCompileEnv &&) = delete;

    ~ShaderCompileEnv()
    {
        glslang::FinalizeProcess();
        initialized = false;
    }
};

class ShaderModule
{
public:
    ShaderModule(std::shared_ptr<LogicalDevice> device, const VkShaderStageFlagBits shader_type, const std::string &file_path);
    ShaderModule(std::shared_ptr<LogicalDevice> device, const VkShaderStageFlagBits shader_type, const std::vector<char> &shader_source);

    ShaderModule(const ShaderModule &) = delete;
    ShaderModule &operator=(const ShaderModule &) = delete;
    ShaderModule(const ShaderModule &&) = delete;
    ShaderModule &operator=(const ShaderModule &&) = delete;

    ~ShaderModule();

    bool valid() const
    {
        return is_valid;
    }

private:
    std::vector<uint32_t> spirv;
    VkShaderModule module = VK_NULL_HANDLE;
    std::shared_ptr<LogicalDevice> device;
    bool is_valid = false;
    VkShaderStageFlagBits shader_type;
};