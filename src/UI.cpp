#include "UI.h"

void UI::ImGuiRender()
{
    std::vector<Sphere>& Spheres = m_ActiveRenderer->GetScnPtr()->spheres;
    std::vector<Plane>& Planes = m_ActiveRenderer->GetScnPtr()->planes;
    const Image* image = m_ActiveRenderer->GetImgPtr();
    Camera* camera = m_ActiveRenderer->GetCamPtr();
    Scene* scene = m_ActiveRenderer->GetScnPtr();

    ImGui::Begin("pepa");
    std::string ms = std::to_string(m_sceneDiff);
    std::string Frames = std::to_string(m_sceneFPS) + " FPS";
    ImGui::Text("%.3fms", ms);
    ImGui::Text(Frames.c_str());
    ImGui::Checkbox("Accumulation", &m_ActiveRenderer->GetSettings().Accumulate);
    if (ImGui::Button("Save"))
    {
        stbi_flip_vertically_on_write(true);
        stbi_write_png("IMAGE_NAME.png", image->size.x, image->size.y, 4, image->data, image->size.x * 4);
    }

    for (int i = 0; i < Spheres.size(); i++)
    {
        std::string number = " " + std::to_string(i);
        std::string radius = "Radius" + number;
        std::string smoothness = "Smoothness" + number;
        std::string albido = "Albido" + number;
        std::string center = "Center" + number;
        std::string specularColor = "SpecularColor" + number;
        std::string specularProb = "SpecularProb" + number;

        Sphere sphere = Spheres[i];
        ImGui::SliderFloat3(center.c_str(), &Spheres[i].centerPtr()->x, -2.0f, 2.0f);
        ImGui::SliderFloat(radius.c_str(), Spheres[i].radiusPtr(), 0.1f, 3.0f);
        ImGui::ColorEdit4(albido.c_str(), &Spheres[i].materialPtr()->albido.x);
        ImGui::ColorEdit4(specularColor.c_str(), &Spheres[i].materialPtr()->specularColor.x);
        ImGui::SliderFloat(smoothness.c_str(), &Spheres[i].materialPtr()->smoothness, 0.0f, 1.0f);
        ImGui::SliderFloat(specularProb.c_str(), &Spheres[i].materialPtr()->specularProbability, 0.0f, 1.0f);
    }
    ImGui::End();

    ImGui::Begin("Camera");
    ImGui::SliderFloat2("From", &camera->m_lookfrom.x, -3.0f, 3.0f);
    ImGui::SliderFloat2("At", &camera->m_lookat.x, -3.0f, 3.0f);
    ImGui::End();

    ImGui::Begin("Planes");
    for (int i = 0; i < Planes.size(); i++)
    {
        std::string number = " " + std::to_string(i);
        std::string smoothness = "Smoothness" + number;
        std::string albido = "Albido" + number;
        std::string center = "Point" + number;
        std::string specularColor = "SpecularColor" + number;
        std::string specularProb = "SpecularProb" + number;

        ImGui::SliderFloat(center.c_str(), &Planes[i].pointPtr()->y, -3.0f, 3.0f);
        ImGui::ColorEdit4(albido.c_str(), &Planes[i].materialPtr()->albido.x);
        ImGui::ColorEdit4(specularColor.c_str(), &Planes[i].materialPtr()->specularColor.x);
        ImGui::SliderFloat(smoothness.c_str(), &Planes[i].materialPtr()->smoothness, 0.0f, 1.0f);
        ImGui::SliderFloat(specularProb.c_str(), &Planes[i].materialPtr()->specularProbability, 0.0f, 1.0f);
    }
    ImGui::End();

    ImGui::Begin("Scene");
    if (ImGui::Button("Save"))
    {
        scene->Save("pepa.txt");
    }
    ImGui::End();
}

UI::UI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.WantCaptureMouse = true;
    ImGuiPlatformIO& p_io = ImGui::GetPlatformIO(); (void)p_io;
    ImGui::RenderPlatformWindowsDefault();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    //io.ConfigViewportsNoAutoMerge = true;

    ImGuiWindowClass wclass;
    io.ConfigViewportsNoDefaultParent = true;
    wclass.ViewportFlagsOverrideSet = ImGuiWindowFlags_NoBringToFrontOnFocus;
    wclass.ViewportFlagsOverrideSet = ImGuiViewportFlags_CanHostOtherWindows;

    loader::GLFW();
    m_window = new Window(800, 800, "RT.png");
    loader::GladGL();

    ImGui_ImplGlfw_InitForOpenGL(m_window->GetID(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    OpenGL::Init();


    ImGui::StyleColorsDark();
}

void UI::UpdateImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiRender();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void UI::Update(Renderer& renderer, float FPS, float diff)
{
    m_ActiveRenderer = &renderer;
    m_sceneFPS = FPS;
    m_sceneDiff = diff;
    int width, height;
    glfwGetFramebufferSize(m_window->GetID(), &width, &height);
    OpenGL::UpdateViewPort(width, height);
    OpenGL::Draw(m_ActiveRenderer->GetImgPtr());

    UpdateImGui();

    m_window->SwapBuffers();
    m_window->PollEvents();
    glfwMakeContextCurrent(m_window->GetID());
}

bool UI::IsRunning()
{
    return !glfwWindowShouldClose(m_window->GetID());
}
