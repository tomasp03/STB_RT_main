#include "Scene.h"

std::vector<Sphere> Scene::Load()
{
    std::vector<Sphere> Spheres;
    Sphere sphere1, sphere2, sphere3, sphere4, sphere5, sphere6;

    sphere1.center = glm::vec3(0.0, 0.0, -1.0);
    sphere2.center = glm::vec3(0.0, -0.5, -1.0);
    sphere3.center = glm::vec3(1.0, 0.0, -1.0);
    sphere4.center = glm::vec3(0.0, 1.0, -5.0);
    sphere5.center = glm::vec3(0.0, 100.0, -5.0);
    sphere6.center = glm::vec3(0.0, 100.0, -5.0);



    sphere1.radius = 0.5f;
    sphere2.radius = 0.5f;
    sphere3.radius = 0.5f;
    sphere4.radius = 0.5f;
    sphere5.radius = 0.5f;
    sphere6.radius = 0.5f;




    sphere1.mat.albido = { 1.0f, 1.0f, 1.0f, 1.0f };
    sphere2.mat.albido = { 0.5f, 0.0f, 0.0f, 1.0f };
    sphere3.mat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sphere4.mat.albido = { 0.f, 0.f, 0.f, 0.0f };
    sphere5.mat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    sphere6.mat.albido = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    sphere1.mat.smoothness = 0.0f;
    sphere2.mat.smoothness = 0.f;
    sphere3.mat.smoothness = 0.0f;
    sphere4.mat.smoothness = 0.f;
    sphere5.mat.smoothness = 0.f;
    sphere6.mat.smoothness = 0.f;

    sphere1.mat.specularColor = glm::vec4(1);
    sphere2.mat.specularColor = glm::vec4(1);
    sphere3.mat.specularColor = glm::vec4(1);
    sphere4.mat.specularColor = glm::vec4(1);
    sphere5.mat.specularColor = glm::vec4(1);
    sphere6.mat.specularColor = glm::vec4(1);

    sphere1.mat.specularProbability = 0.0f;
    sphere2.mat.specularProbability = 0.0f;
    sphere3.mat.specularProbability = 0.0f;
    sphere4.mat.specularProbability = 0.0f;
    sphere5.mat.specularProbability = 0.0f;
    sphere6.mat.specularProbability = 0.0f;

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

    Spheres.push_back(sphere1);
    Spheres.push_back(sphere2);
    Spheres.push_back(sphere3);
    Spheres.push_back(sphere4);
    //Spheres.push_back(sphere5);
    //Spheres.push_back(sphere6);

    return Spheres;
}