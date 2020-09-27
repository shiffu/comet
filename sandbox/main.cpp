#include "sandboxApp.h"

int main(int argc, char** argv)
{
    SanboxApp app;

    CM_LOG_DEBUG("=== main entry point: Run the application ===");
    app.run();
    CM_LOG_DEBUG("=== End main entry point ===");
    
    return 0;
}