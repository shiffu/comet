#include "sandboxApp.h"

int main(int argc, char** argv)
{
    SanboxApp app;
    app.run();

    CM_LOG_DEBUG("=== End main entry point ===");
    
    return 0;
}