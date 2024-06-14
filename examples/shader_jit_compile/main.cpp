#include "shader.h"
#include "window.h"
#include "surface.h"
#include "instance.h"
#include "device_queue.h"

int main()
{
    auto window = std::make_shared<Window>("");
    auto instance = std::make_shared<Instance>();
    auto surface = std::make_shared<Surface>(instance, window);

    auto physical_device = std::make_shared<PhysicalDevice>(instance, surface);
    auto device = std::make_shared<LogicalDevice>(physical_device);

    ShaderCompileEnv::initialize();
    auto this_source = std::string(__FILE__);
    auto dir = this_source.substr(0, this_source.size() - std::string("main.cpp").size());
    ShaderModule(device, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, dir + "example.frag");
}