#include <memory>
#include <cstdlib>
#include <vector>

#include "instance.h"
#include "surface.h"

using std::shared_ptr;

class PhysicalDevice
{
public:
    PhysicalDevice(shared_ptr<Instance> instance, shared_ptr<Surface> surface);

    PhysicalDevice(const PhysicalDevice &) = delete;
    PhysicalDevice &operator=(const PhysicalDevice &) = delete;
    PhysicalDevice(const PhysicalDevice &&) = delete;
    PhysicalDevice &operator=(const PhysicalDevice &&) = delete;

    ~PhysicalDevice()
    {
        handle = VK_NULL_HANDLE;
    }

private:
    VkPhysicalDevice handle;
    shared_ptr<Instance> instance;
    uint32_t queue_family_index;
};