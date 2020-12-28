#include "cometEditorApp.h"
#include <comet/resourceManager.h>

#include <imgui/imgui.h>

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
        m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
        m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 17.0f});
        getActiveScene().setCameraController(&m_cameraController);

        m_frameBuffer = Framebuffer::create({1280, 720});
        m_frameBuffer->invalidate();


        // TEST SCENE
        auto material = MaterialRegistry::getInstance().createMaterial<PhongMaterial>();
        auto materialInstanceId = material->getInstanceId();
        auto materialTypeHash = material->getTypeHash();

        auto material2 = MaterialRegistry::getInstance().createMaterial<PhongMaterial>();
        auto material2InstanceId = material2->getInstanceId();
        auto material2TypeHash = material2->getTypeHash();

        auto meshHandler = ResourceManager::getInstance().loadStaticMesh("torus.obj");
        auto meshHandlerScooter = ResourceManager::getInstance().loadStaticMesh("scooterBlender.obj");

        auto e1 = getActiveScene().createEntity();
        e1.getComponent<NameComponent>().name = "Torus1";
        e1.getComponent<TransformComponent>().translation = glm::vec3(1.0f, 0.0f, -2.0f);
        e1.addComponent<MeshComponent>(meshHandler.resourceId, materialTypeHash, materialInstanceId);
        
        auto e2 = getActiveScene().createEntity();
        e2.getComponent<NameComponent>().name = "Scooter Remi";
        e2.getComponent<TransformComponent>().translation = glm::vec3(-0.4f, 0.5f, -0.3f);
        e2.getComponent<TransformComponent>().scale = glm::vec3(0.1f);
        e2.getComponent<TransformComponent>().rotation = glm::vec3(.028f, -1.9f, 0.0f);
        e2.addComponent<MeshComponent>(meshHandlerScooter.resourceId, material2TypeHash, material2InstanceId);

        m_directionalLight = std::make_unique<DirectionalLight>(glm::vec3(1.0f, -0.70f, -0.3f));
        m_directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
        m_directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
        getActiveScene().addLight(m_directionalLight.get());

        // Prepare the Scene
        getActiveScene().prepare();
    }

    void CometEditorApp::onEvent(Event& e)
    {
        m_cameraController.onEvent(e);
    }

    bool CometEditorApp::onKeyPressed(KeyPressedEvent& e) {}

    void CometEditorApp::onPreRenderScene()
    {
        m_frameBuffer->bind();
    }

    void CometEditorApp::onPostRenderScene()
    {
        m_frameBuffer->unbind();
    }

    void CometEditorApp::onImGuiInit()
    {
        // Flags
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // Customize Style
        ImGui::StyleColorsDark();
        auto& style = ImGui::GetStyle();
        auto& colors = style.Colors;

        // Colors
        colors[ImGuiCol_WindowBg]           = ImVec4(0.04f, 0.04f, 0.06f, 1.00f);
        colors[ImGuiCol_Border]             = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
        colors[ImGuiCol_FrameBg]            = ImVec4(0.10f, 0.12f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.14f, 0.18f, 0.24f, 1.00f);
        colors[ImGuiCol_FrameBgActive]      = ImVec4(0.16f, 0.24f, 0.27f, 1.00f);
        colors[ImGuiCol_TitleBgActive]      = ImVec4(0.12f, 0.18f, 0.29f, 1.00f);
        colors[ImGuiCol_CheckMark]          = ImVec4(0.15f, 0.30f, 0.50f, 1.00f);
        colors[ImGuiCol_SliderGrab]         = ImVec4(0.20f, 0.31f, 0.43f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        colors[ImGuiCol_Button]             = ImVec4(0.15f, 0.30f, 0.50f, 1.00f);
        colors[ImGuiCol_ButtonHovered]      = ImVec4(0.30f, 0.45f, 0.65f, 1.00f);
        colors[ImGuiCol_ButtonActive]       = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        colors[ImGuiCol_Header]             = ImVec4(0.15f, 0.30f, 0.50f, 1.00f);
        colors[ImGuiCol_HeaderHovered]      = ImVec4(0.30f, 0.45f, 0.65f, 1.00f);
        colors[ImGuiCol_HeaderActive]       = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
        colors[ImGuiCol_Separator]          = ImVec4(0.49f, 0.49f, 0.59f, 0.60f);

        colors[ImGuiCol_Tab]                = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
        colors[ImGuiCol_TabHovered]         = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
        colors[ImGuiCol_TabActive]          = ImVec4(0.20f, 0.20f, 0.30f, 1.00f);

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

    void CometEditorApp::onImGuiDraw()
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
                ImGui::MenuItem("Open...", nullptr);
                ImGui::MenuItem("Save", nullptr);
                ImGui::MenuItem("Save As...", nullptr);
                ImGui::Separator();
                ImGui::MenuItem("Show Demo", "", &showDemoWindow);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (showDemoWindow)
            ImGui::ShowDemoWindow();

        // Viewport Panel
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar(3);
        auto viewportPanelSize = ImGui::GetContentRegionAvail();
        auto framebufferSpec = m_frameBuffer->getSpec();
        
        if (viewportPanelSize.x != framebufferSpec.width || viewportPanelSize.y != framebufferSpec.height)
        {
            framebufferSpec.width = viewportPanelSize.x;
            framebufferSpec.height = viewportPanelSize.y;
            m_frameBuffer->setSpec(framebufferSpec);
        }
        m_frameBuffer->invalidate();
        
        auto texId = m_frameBuffer->getBufferId();
        ImGui::Image((void*)(uint64_t)texId, ImVec2((float)viewportPanelSize.x, (float)viewportPanelSize.y),
                    ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        // Scene Hierarchy Panel
        auto& activeScene = getActiveScene();
        m_sceneHierarchyPanel.onDraw(activeScene);

        // Properties Panel
        auto& selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
        if (selectedEntity.isValid())
        {
            m_propertiesPanel.onDraw(activeScene, selectedEntity);
        }

        ImGui::End(); // Main Docking frame
    }

    void CometEditorApp::onUpdate(double deltaTime) {}
}
