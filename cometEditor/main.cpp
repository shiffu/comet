#include "cometEditorApp.h"

int main(int argc, char** argv)
{
    comet::Application* app = comet::CometEditorApp::getInstance();

    CM_LOG_DEBUG("=== main entry point: Run the application ===");
    app->run();    
    CM_LOG_DEBUG("=== End main entry point ===");
    
    return 0;
}