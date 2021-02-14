#include "cometEditorApp.h"

#include <core/imguiWrapper.h>
#include <core/sceneSerializer.h>
#include <core/imguiUtils.h>

namespace comet
{

    Application* Application::getInstance()
    {
        static auto instance = std::make_unique<CometEditorApp>();
        return instance.get();
    }

    CometEditorApp::~CometEditorApp()
    {
        CM_LOG_DEBUG("CometEditorApp destructor");
    }

    void CometEditorApp::onStart()
    {
        m_imguiWrapper = ImguiWrapper::create();
        if (m_imguiWrapper)
        {
            m_imguiWrapper->init();
            m_imguiWrapper->initPlatform(getWindow());
        }

        setActiveScene(&m_editorScene);
        initImGuiTheme();
    }

    void CometEditorApp::initImGuiTheme()
    {
        // Flags
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Customize Style
        ImGui::StyleColorsDark();
        auto& style = ImGui::GetStyle();
        auto& colors = style.Colors;

        // Colors
        colors[ImGuiCol_WindowBg]           = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
        colors[ImGuiCol_Border]             = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBg]            = ImVec4(0.15f, 0.18f, 0.24f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.18f, 0.23f, 0.31f, 1.00f);
        colors[ImGuiCol_FrameBgActive]      = ImVec4(0.24f, 0.34f, 0.38f, 1.00f);
        colors[ImGuiCol_TitleBg]            = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBgActive]      = ImVec4(0.21f, 0.21f, 0.25f, 1.00f);
        colors[ImGuiCol_CheckMark]          = ImVec4(0.09f, 0.31f, 0.56f, 1.00f);
        colors[ImGuiCol_SliderGrab]         = ImVec4(0.20f, 0.31f, 0.43f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        colors[ImGuiCol_Button]             = ImVec4(0.09f, 0.31f, 0.56f, 1.00f);
        colors[ImGuiCol_ButtonHovered]      = ImVec4(0.30f, 0.45f, 0.65f, 1.00f);
        colors[ImGuiCol_ButtonActive]       = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        colors[ImGuiCol_Header]             = ImVec4(0.09f, 0.31f, 0.56f, 1.00f);
        colors[ImGuiCol_HeaderHovered]      = ImVec4(0.30f, 0.45f, 0.65f, 1.00f);
        colors[ImGuiCol_HeaderActive]       = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        colors[ImGuiCol_Separator]          = ImVec4(0.49f, 0.49f, 0.59f, 0.60f);
        colors[ImGuiCol_Tab]                = ImVec4(0.09f, 0.09f, 0.14f, 1.00f);
        colors[ImGuiCol_TabHovered]         = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
        colors[ImGuiCol_TabActive]          = ImVec4(0.15f, 0.18f, 0.24f, 1.00f);
        colors[ImGuiCol_TabUnfocused]       = ImVec4(0.06f, 0.07f, 0.08f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06f, 0.07f, 0.08f, 1.00f);

        // Sizes
        style.FramePadding = ImVec2(4.0f, 6.0f);
        style.CellPadding = ImVec2(4.0f, 1.0f);
        style.ItemSpacing = ImVec2(6.0f, 1.0f);

        // Fonts
        auto fontPath = ResourceManager::getInstance().getResourcePath(ResourceType::FONT, "Open_Sans/OpenSans-Regular.ttf");
        auto boldFontPath = ResourceManager::getInstance().getResourcePath(ResourceType::FONT, "Open_Sans/OpenSans-Bold.ttf");
        io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 17.0f);
        io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 17.0f);
    }

    
    void CometEditorApp::drawFramebuffer(Scene& scene, uint8_t renderPassIndex /*= 0*/)
    {
        auto viewportPanelSize = ImGui::GetContentRegionAvail();
        auto renderPass = scene.getRenderPass(renderPassIndex);
        auto framebuffer = renderPass->getTarget();
        auto framebufferSpec = framebuffer->getSpec();
        
        if (viewportPanelSize.x != framebufferSpec.width || viewportPanelSize.y != framebufferSpec.height)
        {
            framebufferSpec.width = (uint16_t)viewportPanelSize.x;
            framebufferSpec.height = (uint16_t)viewportPanelSize.y;

            framebuffer->setSpec(framebufferSpec);
        }
        
        auto texId = framebuffer->getColorAttachmentId();
        ImGui::Image((void*)(uint64_t)texId, ImVec2((float)viewportPanelSize.x, (float)viewportPanelSize.y),
                    ImVec2(0, 1), ImVec2(1, 0));
    }

    void CometEditorApp::drawEditor()
    {
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("DockSpace", nullptr, window_flags);

        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        // Main Menu
        static bool showDemoWindow = false;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New", nullptr);
                ImGui::Separator();
                
                if (ImGui::MenuItem("Open...", nullptr))
                {
                    m_editorScene.stop();
                    SceneSerializer::deserialize(m_editorScene, "EditorScene.scn");
                    m_editorScene.start();
                }
                
                if (ImGui::MenuItem("Save", nullptr))
                {
                    SceneSerializer::serialize(m_editorScene);
                }

                ImGui::MenuItem("Save As...", nullptr);
                ImGui::Separator();

                ImGui::MenuItem("Show Demo", "", &showDemoWindow);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (showDemoWindow)
            ImGui::ShowDemoWindow();

        // Debug Window
        drawEditorDebug();

        // Viewport Panel
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar(3);
        if (!m_isGamePlaying)
        {
            drawFramebuffer(m_editorScene);
        }
        ImGui::End();

        // Game panel
        ImGui::Begin("Game");
        if (m_isGamePlaying)
        {
            drawFramebuffer(m_gameScene);
        }
        ImGui::End();

        ImGui::Begin("GamePreview");
        if (!m_isGamePlaying)
        {
            drawFramebuffer(m_editorScene, 1);
        }
        ImGui::End();

        // Toolbar
        ImGui::Begin("Toolbar");
        bool playClicked;
        bool stopClicked;
        {
            ItemDisabler playButtonDisabler{m_isGamePlaying};
            playClicked = ImGui::Button("Play"); ImGui::SameLine();
        }
        {
            ItemDisabler stopButtonDisabler{!m_isGamePlaying};
            stopClicked = ImGui::Button("Stop");
        }

        if (playClicked && !m_isGamePlaying)
        {
            m_isGamePlaying = true;
            setActiveScene(&m_gameScene);
        }

        if (stopClicked && m_isGamePlaying)
        {
            m_isGamePlaying = false;
            setActiveScene(&m_editorScene);
        }
        ImGui::End();

        // Scene Hierarchy Panel
        m_sceneHierarchyPanel.onDraw(m_editorScene);

        // Properties Panel
        auto& selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
        if (selectedEntity.isValid())
        {
            m_propertiesPanel.onDraw(m_editorScene, selectedEntity);
        }

        ImGui::End(); // Main Docking frame
    }

    void CometEditorApp::drawEditorDebug()
    {
        ImGui::Begin("Debug");
        
        auto FPS = ImGui::GetIO().Framerate;
        ImGui::Text("Frame time %.3f ms (%.1f FPS)", 1000.0f / FPS, FPS);
        
        auto& stats = m_editorScene.getStatistics();
        ImGui::Text("Lights: %d", stats.lightsCount);
        ImGui::Text("Entities: %d", stats.entitiesCount);
        ImGui::Text("Vertices: %d / Indices: %d", stats.verticesCount, stats.indicesCount);
        ImGui::Text("Draw calls: %d / Draw commands: %d", stats.drawCalls, stats.drawCommandsCount);

        ImGui::End();
    }

    void CometEditorApp::onStop()
    {
        if (m_imguiWrapper)
        {
            m_imguiWrapper->shutdown();
            m_imguiWrapper = nullptr;
        }
    }

    void CometEditorApp::onUpdate()
    {
        if (m_imguiWrapper)
        {
            m_imguiWrapper->newFrame();
            drawEditor();
        }
    }

    void CometEditorApp::onRender()
    {
        if (m_imguiWrapper)
        {
            m_imguiWrapper->render();
        }
    }
}
