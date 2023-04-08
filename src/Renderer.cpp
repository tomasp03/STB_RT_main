#include "Renderer.h"

static glm::vec3 LerpVec3(glm::vec3 a, glm::vec3 b, float t)
{
    return glm::vec3(std::lerp(a.x, b.x, t), std::lerp(a.y, b.y, t), std::lerp(a.z, b.z, t));
}

static uint32_t ConvertToRGBA(glm::vec4 color)
{
    uint8_t r = (uint8_t)(color.r * 255.0f);
    uint8_t g = (uint8_t)(color.g * 255.0f);
    uint8_t b = (uint8_t)(color.b * 255.0f);
    uint8_t a = (uint8_t)(color.a * 255.0f);

    uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
    return result;
}

static glm::vec4 GammaCorection2(glm::vec4 color)
{
    return glm::vec4(glm::sqrt(color));
}

Renderer::Renderer(int width, int height)
{
    Plane plane, plane2, plane3, plane4, plane5;
    plane.point = glm::vec3(0.0f, -0.5f, 0.0f);
    plane.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    Material PlaneMat;
    PlaneMat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    PlaneMat.emissionColor = glm::vec4(1);
    PlaneMat.emissionStrength = 0.f;
    PlaneMat.smoothness = 0.0f;
    PlaneMat.specularColor = glm::vec4(1);
    PlaneMat.specularProbability = 0.0f;
    plane.mat = PlaneMat;

    m_planes.push_back(plane);

    random.Init();
    m_Spheres = scene1.Load();
    m_image.size = glm::ivec2(width, height);
    m_accumulationImage = new glm::vec4[m_image.size.x * m_image.size.y];
    m_image.data = new uint32_t[m_image.size.x * m_image.size.y];

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

    ImGui::StyleColorsDark();

    ImageVerticalIter.resize(m_image.size.y);
    for (int y = 0; y < m_image.size.y; y++)
    {
        ImageVerticalIter[y] = y;
    }



    loader::GLFW();
    m_window = new Window(m_image.size.x, m_image.size.y, "RT.png");
    loader::GladGL();

    ImGui_ImplGlfw_InitForOpenGL(m_window->GetID(), true);
    ImGui_ImplOpenGL3_Init("#version 460");


    m_shader = new Shader("shaders\\basic.vert", "shaders\\basic.frag");
    // Create reference containers for the Vartex Array Object, the Vertex Buffer Object, and the Element Buffer Object

    OpenGL::Init();

    m_cam = new camera(glm::vec3(0, 0, -3), glm::vec3(0, 1, -1), glm::vec3(0, 1, 0), 90.0f, m_image.AspectRatio());


    stbi_set_flip_vertically_on_load(true);
    skybox.image = stbi_loadf("skyboxes\\container_free_Ref.hdr", &skybox.x, &skybox.y, &skybox.chanels, 0);

}

bool Renderer::Render()
{
    auto start = std::chrono::steady_clock::now();

    int width, height;
    glfwGetFramebufferSize(m_window->GetID(), &width, &height);
    OpenGL::UpdateViewPort(width, height);

    if (frameIndex == 1)
        memset(m_accumulationImage, 0, m_image.size.x * m_image.size.y * sizeof(glm::vec4));



    std::for_each(std::execution::par, ImageVerticalIter.begin(), ImageVerticalIter.end(),
        [&](unsigned int y)
        {
            for (int x = 0; x < m_image.size.x; x++)
            {
                int index = (x + y * m_image.size.x);
                float offset_u = random.Float2(-0.3f, 0.3f) / (float)m_image.size.x;
                float offset_v = random.Float2(-0.3f, 0.3f) / (float)m_image.size.y;


                float u = ((float)(x) / (float)m_image.size.x) + offset_u; // transforms to [-1.0, 1.0]
                float v = ((float)(y) / (float)m_image.size.y) + offset_v;               // transforms to [-1.0, 1.0]


                Ray ray = m_cam->get_ray(u, v);

                m_accumulationImage[index] += Trace(ray);

                glm::vec4 accumulationColor = m_accumulationImage[index];
                accumulationColor /= (float)frameIndex;
                accumulationColor.a = 1.0f;
                m_image.data[index] = ConvertToRGBA(GammaCorection2(accumulationColor));
            }
        }
    );

    m_cam->Recalculate();

    if (accumulate)
        frameIndex++;
    else
        frameIndex = 1;

    OpenGL::Draw(m_image);

    //Imgui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiRender();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_window->SwapBuffers();
    m_window->PollEvents();
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> result = end - start;
    diff = result.count() * 1000.0;
    FPS = 1 / result.count();
    return !glfwWindowShouldClose(m_window->GetID());
}

void Renderer::ShutWindow()
{
    m_window->Destroy();
}

HitInfo Renderer::CalculateRayCollision(Ray ray)
{
    HitInfo closestHit;
    closestHit.didHit = false;
    closestHit.dst = FLT_MAX;

    HitInfo closestHitPlane;
    closestHitPlane.didHit = false;
    closestHitPlane.dst = FLT_MAX;

    for (int i = 0; i < m_Spheres.size(); i++)
    {
        Sphere sphere = m_Spheres[i];
        HitInfo hitInfo = RaySphere(ray, sphere);

        if (hitInfo.didHit && hitInfo.dst < closestHit.dst)
        {
            closestHit = hitInfo;
            closestHit.mat = sphere.mat;
        }
    }

    for (int i = 0; i < m_planes.size(); i++)
    {
        Plane plane = m_planes[i];
        HitInfo hitInfo = RayPlane(ray, plane);

        if (hitInfo.didHit && hitInfo.dst < closestHitPlane.dst)
        {
            closestHitPlane = hitInfo;
            closestHitPlane.mat = plane.mat;
        }
    }

    if (!closestHit.didHit && !closestHitPlane.didHit)
        return closestHit;

    return closestHit.dst < closestHitPlane.dst ? closestHit : closestHitPlane;
}

HitInfo Renderer::RaySphere(Ray ray, Sphere sphere)
{
    HitInfo hitInfo;
    hitInfo.didHit = false;

    glm::vec3 offsetRayOrigin = ray.origin - sphere.center;

    float a = glm::dot(ray.direction, ray.direction);
    float half_b = glm::dot(offsetRayOrigin, ray.direction);
    float c = glm::dot(offsetRayOrigin, offsetRayOrigin) - sphere.radius * sphere.radius;

    float discriminant = half_b * half_b - a * c;
    if (discriminant >= 0)
    {
        float dst = (-half_b - glm::sqrt(discriminant)) / a;

        if (dst >= 0)
        {
            hitInfo.didHit = true;
            hitInfo.dst = dst;
            hitInfo.hitPoint = ray.origin + ray.direction * dst;
            hitInfo.normal = glm::normalize(hitInfo.hitPoint - sphere.center);
            hitInfo.hitPoint = hitInfo.hitPoint + hitInfo.normal * 0.00001f;
        }
    }

    return hitInfo;
}

HitInfo Renderer::RayPlane(Ray ray, Plane plane)
{
    HitInfo hitInfo;
    hitInfo.didHit = false;

    float denom = glm::dot(ray.direction, plane.Normal);
    if (denom == 0)
        return hitInfo;
    float numerator = glm::dot(plane.point - ray.origin, plane.Normal);

    float t = numerator / denom;

    if (t < 0)
        return hitInfo;

    hitInfo.didHit = true;
    hitInfo.normal = plane.Normal;
    hitInfo.hitPoint = ray.At(t);
    hitInfo.dst = t;
    hitInfo.hitPoint = hitInfo.hitPoint + hitInfo.normal * 0.00001f;

    return hitInfo;
}

glm::vec4 Renderer::BackgroundColor(Ray ray)
{
    glm::vec3 unit_direction = glm::normalize(ray.direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec4(1.0, 1.0, 1.0, 1.0) + t * glm::vec4(0.5, 0.7, 1.0, 1.0);
}

glm::vec4 Renderer::Trace(Ray& ray)
{
    glm::vec4 incomingLight = glm::vec4(0.0, 0.0, 0.0, 0.0);
    glm::vec4 rayColor = glm::vec4(1, 1, 1, 1);
    HitInfo firstHitInfo = CalculateRayCollision(ray);

    for (int i = 0; i <= MaxBounces; i++)
    {
        HitInfo hitInfo = CalculateRayCollision(ray);
        if (hitInfo.didHit)
        {

            Material material = hitInfo.mat;
            ray.origin = hitInfo.hitPoint;
            glm::vec3 diffuseDir = glm::normalize(hitInfo.normal + random.InUnitSphere());
            glm::vec3 specularDir = glm::reflect(ray.direction, hitInfo.normal);
            bool isSpecularBounce = material.specularProbability >= random.Float();
            ray.direction = LerpVec3(diffuseDir, specularDir, material.smoothness * isSpecularBounce);

            glm::vec4 emittedLight = material.emissionColor * material.emissionStrength;
            incomingLight += (emittedLight * rayColor);
            rayColor *= glm::vec4(LerpVec3(material.albido, material.specularColor, isSpecularBounce), 1.0f);
        }
        else
        {
            incomingLight += Skybox(ray) * rayColor;
            break;
        }
    }

    return glm::min(incomingLight, glm::vec4(1.0f));
}

void Renderer::ImGuiRender()
{
    ImGui::Begin("pepa");
    std::string ms = std::to_string(diff) + " ms";
    std::string Frames = std::to_string(FPS) + " FPS";
    ImGui::Text(ms.c_str());
    ImGui::Text(Frames.c_str());
    ImGui::Checkbox("Accumulation", &accumulate);
    if (ImGui::Button("Save"))
    {
        stbi_flip_vertically_on_write(true);
        stbi_write_png("IMAGE_NAME.png", m_image.size.x, m_image.size.y, 4, m_image.data, m_image.size.x * 4);
    }

    for (int i = 0; i < m_Spheres.size(); i++)
    {
        std::string number = " " + std::to_string(i);
        std::string radius = "Radius" + number;
        std::string smoothness = "Smoothness" + number;
        std::string albido = "Albido" + number;
        std::string center = "Center" + number;
        std::string specularColor = "SpecularColor" + number;
        std::string specularProb = "SpecularProb" + number;




        Sphere sphere = m_Spheres[i];
        ImGui::SliderFloat3(center.c_str(), &m_Spheres[i].center.x, -2.0f, 2.0f);
        ImGui::SliderFloat(radius.c_str(), &m_Spheres[i].radius, 0.1f, 3.0f);
        ImGui::ColorEdit4(albido.c_str(), &m_Spheres[i].mat.albido.x);
        ImGui::ColorEdit4(specularColor.c_str(), &m_Spheres[i].mat.specularColor.x);
        ImGui::SliderFloat(smoothness.c_str(), &m_Spheres[i].mat.smoothness, 0.0f, 1.0f);
        ImGui::SliderFloat(specularProb.c_str(), &m_Spheres[i].mat.specularProbability, 0.0f, 1.0f);
    }
    ImGui::End();

    ImGui::Begin("Camera");
    ImGui::SliderFloat2("From", &m_cam->m_lookfrom.x, -3.0f, 3.0f);
    ImGui::SliderFloat2("At", &m_cam->m_lookat.x, -3.0f, 3.0f);
    ImGui::End();

    ImGui::Begin("Planes");
    for (int i = 0; i < m_planes.size(); i++)
    {
        std::string number = " " + std::to_string(i);
        std::string smoothness = "Smoothness" + number;
        std::string albido = "Albido" + number;
        std::string center = "Point" + number;
        std::string specularColor = "SpecularColor" + number;
        std::string specularProb = "SpecularProb" + number;

        ImGui::SliderFloat(center.c_str(), &m_planes[i].point.y, -3.0f, 3.0f);
        ImGui::ColorEdit4(albido.c_str(), &m_planes[i].mat.albido.x);
        ImGui::ColorEdit4(specularColor.c_str(), &m_planes[i].mat.specularColor.x);
        ImGui::SliderFloat(smoothness.c_str(), &m_planes[i].mat.smoothness, 0.0f, 1.0f);
        ImGui::SliderFloat(specularProb.c_str(), &m_planes[i].mat.specularProbability, 0.0f, 1.0f);
    }
    ImGui::End();

    ImGui::Begin("Scene");
    if (ImGui::Button("Save"))
    {
        scene1.Save("pepa.txt", m_Spheres);
    }
    ImGui::End();
}

glm::vec4 Renderer::Skybox(Ray ray)
{
    glm::vec3 dir = glm::normalize(ray.direction);
    float u = 0.5f + (glm::atan(dir.x, dir.z)) / (2.0f * PI);
    float v = 0.5f + (glm::asin(dir.y)) / PI;

    int x = (int)(u * (skybox.x - 1));
    int y = (int)(v * (skybox.y - 1));

    int index = (x + y * skybox.x) * skybox.chanels;

    glm::vec4 color = { skybox.image[index + 0], skybox.image[index + 1], skybox.image[index + 2], 1.0f };

    return color;
}
