#include "sandboxApp.h"

int main(int argc, char** argv)
{
    comet::Application* app = SandboxApp::getInstance();

    CM_LOG_DEBUG("=== Comet Editor Application ===");
    // app->setFPSCap(120);
    app->run();    
    CM_LOG_DEBUG("=== End Comet Editor Application ===");
    
    return 0;
}