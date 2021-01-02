#include <comet/application.h>
#include <comet/log.h>
#include <comet/window.h>
#include <platforms/sfml/SFMLWindow.h>
#include <rendering/imguiWrapper.h>

#include <imgui/imgui.h>

#include <functional>
#include <chrono>
#include <thread>

namespace comet
{
    #define BIND_METHOD(m) std::bind(&m, this, std::placeholders::_1)

    Application::Application(const WindowSpec& spec)
    {
        init(spec);
        CM_CORE_LOG_DEBUG("Application contructor");
    }

    Application::~Application()
    {
        CM_CORE_LOG_DEBUG("Application destructor");
    };

    void Application::setFPSCap(unsigned int fpsCap)
    {
        m_fpsCap = fpsCap;
    }

    void Application::init(const WindowSpec& spec)
    {
        Log::init();
        
        CM_CORE_LOG_DEBUG("Initializing the application");
        m_window = Window::create(spec);

        // Deactivate VSync (slowness of window motion on Linux when VSync is on)
        m_window->setVSync(false);

        // set Event callback
        m_window->addEventCallback(BIND_METHOD(Application::onEventDispatch));

        // set Imgui Wrapper
        m_imguiWrapper = ImguiWrapper::create();
        if (m_imguiWrapper)
        {
            m_imguiWrapper->init();
        }

        m_isInitialized = true;
        CM_CORE_LOG_DEBUG("Finished initializing the application");
    }

    void Application::onStart() {}
    void Application::onUpdate(double deltaTime) {}
    void Application::onFixedUpdate(float fixedDeltaTime) {}

    void Application::onEventDispatch(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.dispatch<WindowResizedEvent>(BIND_METHOD(Application::onWindowResized));
        dispatcher.dispatch<WindowLostFocusEvent>(BIND_METHOD(Application::onWindowLostFocus));
        dispatcher.dispatch<WindowGainedFocusEvent>(BIND_METHOD(Application::onWindowGainedFocus));
        dispatcher.dispatch<KeyPressedEvent>(BIND_METHOD(Application::onKeyPressed));
        dispatcher.dispatch<KeyReleasedEvent>(BIND_METHOD(Application::onKeyReleased));
        dispatcher.dispatch<KeyTextEnteredEvent>(BIND_METHOD(Application::onTextEntered));
        dispatcher.dispatch<MouseMovedEvent>(BIND_METHOD(Application::onMouseMoved));
        dispatcher.dispatch<MouseEnteredWindowEvent>(BIND_METHOD(Application::onMouseEnteredWindow));
        dispatcher.dispatch<MouseLeftWindowEvent>(BIND_METHOD(Application::onMouseLeftWindow));
        dispatcher.dispatch<VerticalMouseWheelScrolledEvent>(BIND_METHOD(Application::onVerticalMouseWheelScrolled));
        dispatcher.dispatch<HorizontalMouseWheelScrolledEvent>(BIND_METHOD(Application::onHorizontalMouseWheelScrolled));
        dispatcher.dispatch<MouseButtonPressedEvent>(BIND_METHOD(Application::onMouseButtonPressed));
        dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_METHOD(Application::onMouseButtonRelease));

        // Dispatch the event further
        onEvent(e);
    }

    bool Application::onWindowResized(WindowResizedEvent& e) {}
    bool Application::onWindowLostFocus(WindowLostFocusEvent& e) {}
    bool Application::onWindowGainedFocus(WindowGainedFocusEvent& e) {}
    bool Application::onKeyPressed(KeyPressedEvent& e) {}
    bool Application::onKeyReleased(KeyReleasedEvent& e) {}
    bool Application::onTextEntered(KeyTextEnteredEvent& e) {}
    bool Application::onMouseMoved(MouseMovedEvent& e) {}
    bool Application::onMouseEnteredWindow(MouseEnteredWindowEvent& e) {}
    bool Application::onMouseLeftWindow(MouseLeftWindowEvent& e) {}
    bool Application::onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e) {}
    bool Application::onHorizontalMouseWheelScrolled(HorizontalMouseWheelScrolledEvent& e) {}
    bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) {}
    bool Application::onMouseButtonRelease(MouseButtonReleasedEvent& e) {}

    void Application::onImGuiDebugDraw()
    {
        ImGui::Begin("Comet Debug");
        ImGui::Text("Scene statistics debug information");
        
        auto FPS = ImGui::GetIO().Framerate;
        ImGui::Text("Frame time %.3f ms (%.1f FPS)", 1000.0f / FPS, FPS);
        
        auto& stats = m_activeScene.getStatistics();
        ImGui::Text("Lights: %d", stats.lightsCount);
        ImGui::Text("Entities: %d", stats.entitiesCount);
        ImGui::Text("Vertices: %d / Indices: %d", stats.verticesCount, stats.indicesCount);
        ImGui::Text("Draw calls: %d / Draw commands: %d", stats.drawCalls, stats.drawCommandsCount);

        ImGui::End();
    }

    void Application::run()
    {
        if (m_imguiWrapper)
        {
            onImGuiInit();
            m_imguiWrapper->initPlatform(m_window);
        }

        CM_CORE_LOG_INFO("Start running the application");
        m_isRunning = true;

        CM_CORE_LOG_DEBUG("Calling onStart()");
        onStart();

        CM_CORE_LOG_DEBUG("Starting main loop");
        using namespace std::chrono;

        duration<double, std::nano> lag(0);
        duration<double, std::milli> fixedUpdateTime(m_fixedUpdateTime);
        auto previousTime = steady_clock::now();
        auto currentTime = previousTime;
        duration<double, std::nano> timeSinceLastFPSDisplay;
        duration<double, std::nano> elapsedTime;
        unsigned int frameCounter = 0;
        duration<float, std::milli> fpsCapTime(0.0f);
        double deltaTime(0.0f);

        if (m_fpsCap)
        {
            fpsCapTime = duration<float, std::milli>(1000.0f / m_fpsCap);
            CM_CORE_LOG_DEBUG("FPS Cap Time set to: {}ms", fpsCapTime.count());
        }

        while(!m_window->isCloseRequested())
        {
            T_gameloop.resume();
            currentTime = steady_clock::now();
            elapsedTime = currentTime - previousTime;
            previousTime = currentTime;
            lag += elapsedTime;

            // poll window events
            m_window->pollEvent();

            T_fixed_udpate.resume();
            // Update as lag permits
            while(lag >= fixedUpdateTime)
            {
                // CM_CORE_LOG_DEBUG("calling onFixedUpdate function. lag: {}", lag.count());
                onFixedUpdate(fixedUpdateTime.count());
                lag -= fixedUpdateTime;
            }
            T_fixed_udpate.pause();

            T_update.resume();
            deltaTime = duration_cast<duration<double, std::milli>>(elapsedTime).count();
            onUpdate(deltaTime);
            T_update.pause();

            T_draw_imgui.resume();
            if (m_imguiWrapper)
            {
                m_imguiWrapper->newFrame();
                onImGuiDraw();
                onImGuiDebugDraw();
            }
            T_draw_imgui.pause();

            // Compute and display FPS (every 2s)
            // frameCounter++;
            // timeSinceLastFPSDisplay += elapsedTime;
            // if (timeSinceLastFPSDisplay > seconds(2))
            // {
            //     float fps = frameCounter / (duration_cast<seconds>(timeSinceLastFPSDisplay).count());
            //     frameCounter = 0;
            //     timeSinceLastFPSDisplay = milliseconds::zero();
            //     CM_CORE_LOG_DEBUG("FPS: {}", fps);
            // }

            T_render.resume();
            m_window->clearBuffers();
            onPreRenderScene();
            m_activeScene.render();
            onPostRenderScene();
            T_render.pause();

            T_draw_imgui.resume();
            if (m_imguiWrapper)
            {
                m_imguiWrapper->render();
            }
            T_draw_imgui.pause();

            m_window->swapBuffers();

            T_gameloop.pause();

            // Is FPS Cap needed?
            if (m_fpsCap)
            {
                auto t1 = steady_clock::now();
                auto waitTime = fpsCapTime - duration_cast<milliseconds>(t1 - currentTime);
                if (waitTime.count() > 1.0f)
                {
                    std::this_thread::sleep_for(waitTime);
                }
            }
            else
            {
                std::this_thread::sleep_for(microseconds(5));
            }
        }
        
        if (m_imguiWrapper)
        {
            m_imguiWrapper->shutdown();
            m_imguiWrapper = nullptr;
        }
        
        m_window->close();
        CM_CORE_LOG_DEBUG("Exit main loop");
    }

} // namespace comet