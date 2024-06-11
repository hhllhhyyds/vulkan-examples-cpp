#include <iostream>
#include <stdexcept>

#include "src/window.h"
#include "src/instance.h"

class HelloTriangleApplication
{
public:
    void run()
    {

        mainLoop();
        cleanup();
    }

private:
    Window window;
    Instance instance;

    void mainLoop()
    {
        while (!window.should_close())
        {
            window.poll_event();
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