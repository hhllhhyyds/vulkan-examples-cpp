#include <iostream>
#include <stdexcept>

#include "window.h"
#include "instance.h"
#include "surface.h"
#include "device_queue.h"

using std::shared_ptr;

class HelloTriangleApplication
{
public:
    void run()
    {

        mainLoop();
        cleanup();
    }

    HelloTriangleApplication() : window(std::make_shared<Window>("Hello Triangle")),
                                 instance(std::make_shared<Instance>()),
                                 surface(std::make_shared<Surface>(instance, window))
    {
        physical_device = std::make_shared<PhysicalDevice>(instance, surface);
        device = std::make_shared<LogicalDevice>(physical_device);
    }

private:
    shared_ptr<Window> window;
    shared_ptr<Instance> instance;
    shared_ptr<Surface> surface;
    shared_ptr<PhysicalDevice> physical_device;
    shared_ptr<LogicalDevice> device;

    void mainLoop()
    {
        while (!window->should_close())
        {
            window->poll_event();
        }
    }

    void cleanup()
    {
    }
};

int main()
{
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}