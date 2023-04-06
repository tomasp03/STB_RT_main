#include "Renderer.h"

static glm::vec3 LerpVec3(glm::vec3 a, glm::vec3 b, float t)
{
    return glm::vec3(std::lerp(a.x, b.x, t), std::lerp(a.y, b.y, t), std::lerp(a.z, b.z, t));
}

//static uint32_t convertToRGBA(glm::vec4 color)
//{
//    uint8_t r = (uint8_t)(color.r * 255.0f);
//    uint8_t g = (uint8_t)(color.g * 255.0f);
//    uint8_t b = (uint8_t)(color.b * 255.0f);
//    uint8_t a = (uint8_t)(color.a * 255.0f);
//
//    uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
//    return result;
//}

Renderer::Renderer(int width, int height, camera* cam)
{
    m_cam = cam;
    dimesnisons = glm::ivec2(width, height);
    Sphere sphere1, sphere2, sphere3, sphere4, sphere5, sphere6;

    sphere1.center = glm::vec3(0.0, 0.0, -1.0);
    sphere2.center = glm::vec3(0.0, -100.5, -1.0);
    sphere3.center = glm::vec3(1.0, 0.0, -1.0);
    sphere4.center = glm::vec3(0.0, 100.0, -5.0);
    sphere5.center = glm::vec3(0.0, 100.0, -5.0);
    sphere6.center = glm::vec3(0.0, 100.0, -5.0);



    sphere1.radius = 0.5f;
    sphere2.radius = 100.0F;
    sphere3.radius = 0.5f;
    sphere4.radius = 80.0f;
    sphere5.radius = 0.5f;
    sphere6.radius = 0.5f;




    sphere1.mat.albido = { 1.0f, 1.0f, 1.0f, 1.0f };
    sphere2.mat.albido = { 0.5f, 0.0f, 0.0f, 1.0f };
    sphere3.mat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sphere4.mat.albido = { 0.f, 0.f, 0.f, 0.0f };
    sphere5.mat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sphere6.mat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);




    sphere1.mat.roughness = 0.f;
    sphere2.mat.roughness = 0.f;
    sphere3.mat.roughness = 0.5f;
    sphere4.mat.roughness = 0.f;

    sphere1.mat.smoothness = 0.0f;
    sphere2.mat.smoothness = 0.f;
    sphere3.mat.smoothness = 0.0f;
    sphere4.mat.smoothness = 0.f;
    sphere5.mat.smoothness = 0.f;
    sphere6.mat.smoothness = 0.f;



    sphere1.mat.emissionColor = glm::vec4(1);
    sphere1.mat.emissionStrength = 0.0f;

    sphere2.mat.emissionColor = glm::vec4(1);
    sphere2.mat.emissionStrength = 0.0f;

    sphere3.mat.emissionColor = glm::vec4(1);
    sphere3.mat.emissionStrength = 0.0f;

    sphere4.mat.emissionColor = { 1.f, 1.f, 1.f, 1.0f };;
    sphere4.mat.emissionStrength = 20.0f;

    sphere5.mat.emissionColor = glm::vec4(1);
    sphere5.mat.emissionStrength = 0.0f;

    sphere6.mat.emissionColor = glm::vec4(1);
    sphere6.mat.emissionStrength = 0.0f;

    m_Spheres.push_back(sphere3);
    m_Spheres.push_back(sphere1);
    m_Spheres.push_back(sphere2);
    m_Spheres.push_back(sphere4);
    m_Spheres.push_back(sphere5);
    m_Spheres.push_back(sphere6);

    stbi_set_flip_vertically_on_load(true);
    skybox.image = stbi_loadf("skyboxes\\container_free_Ref.hdr", &skybox.x, &skybox.y, &skybox.chanels, 0);

}

HitInfo Renderer::CalculateRayCollision(Ray ray)
{
    HitInfo closestHit;
    closestHit.didHit = false;
    closestHit.dst = FLT_MAX;

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

    return closestHit;
}

HitInfo Renderer::RaySphere(Ray ray, Sphere sphere)
{
    HitInfo hitInfo;
    hitInfo.didHit = false;

    glm::vec3 offsetRayOrigin = ray.origin - sphere.center;

    float a = glm::dot(ray.direction, ray.direction);
    float b = 2 * glm::dot(offsetRayOrigin, ray.direction);
    float c = glm::dot(offsetRayOrigin, offsetRayOrigin) - sphere.radius * sphere.radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant >= 0)
    {
        float dst = (-b - sqrt(discriminant)) / (2.0 * a);

        if (dst >= 0)
        {
            hitInfo.didHit = true;
            hitInfo.dst = dst;
            hitInfo.hitPoint = ray.origin + ray.direction * dst;
            hitInfo.normal = glm::normalize(hitInfo.hitPoint - sphere.center);
            hitInfo.hitPoint = ray.origin + ray.direction * dst + hitInfo.normal * 0.00001f;

        }
    }

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
            glm::vec3 diffuseDir = glm::normalize(hitInfo.normal + RandomDirecion());
            glm::vec3 specularDir = glm::reflect(ray.direction, hitInfo.normal);
            ray.direction = LerpVec3(diffuseDir, specularDir, material.smoothness);

            glm::vec4 emittedLight = material.emissionColor * material.emissionStrength;
            incomingLight += (emittedLight * rayColor);
            rayColor *= material.albido;
        }
        else
        {
            incomingLight += Skybox(ray) * rayColor;
            break;
        }
    }

    return glm::min(incomingLight, glm::vec4(1.0f));
}

void Renderer::ImGuiRender(bool& accumulate, uint32_t* image, camera& cam)
{
    ImGui::Begin("pepa");
    ImGui::Checkbox("Accumulation", &accumulate);
    if (ImGui::Button("Save"))
    {
        stbi_flip_vertically_on_write(true);
        stbi_write_png("IMAGE_NAME.png", dimesnisons.x, dimesnisons.y, 4, image, dimesnisons.x * 4);
    }

    for (int i = 0; i < m_Spheres.size(); i++)
    {
        std::string number = " " + std::to_string(i);
        std::string radius = "Radius" + number;
        std::string smoothness = "Smoothness" + number;
        std::string albido = "Albido" + number;
        std::string center = "Center" + number;


        Sphere sphere = m_Spheres[i];
        ImGui::SliderFloat(radius.c_str(), &m_Spheres[i].radius, 0.1f, 3.0f);
        ImGui::SliderFloat(smoothness.c_str(), &m_Spheres[i].mat.smoothness, 0.0f, 1.0f);
        ImGui::ColorEdit4(albido.c_str(), &m_Spheres[i].mat.albido.x);
        ImGui::SliderFloat3(center.c_str(), &m_Spheres[i].center.x, -2.0f, 2.0f);
    }

    //ImGui::Text("Camera");
    //ImGui::SliderFloat3("From", &cam.m_lookfrom.x, -2.0f, 2.0f);
    //ImGui::SliderFloat3("At", &cam.m_lookat.x, -2.0f, 2.0f);

    //if (ImGui::Button("Recalculate"))
    //{
    //    cam.Recalculate();
    //}

    ImGui::End();
}

void Renderer::Render()
{
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
