#include <iostream>
#include <comet/comet.h>

class SanboxApp : public comet::Application
{
public:
    SanboxApp() { CM_LOG_DEBUG("SanboxApp contructor"); } ;
    virtual ~SanboxApp() { CM_LOG_DEBUG("SanboxApp destructor"); };
};


int main(int argc, char** argv)
{
    comet::Log::init();
    CM_LOG_DEBUG("=== Begin main entry point ===");

    auto app = SanboxApp();
    app.run();

    CM_LOG_DEBUG("=== End main entry point ===");
    
    return 0;
}