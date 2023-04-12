#include "Scene.h"

std::vector<Sphere> Scene::Load()
{
    Sphere sphere1, sphere2, sphere3, sphere4, sphere5, sphere6;

    sphere1.center(glm::vec3(0.0, 0.0, -1.0));
    sphere2.center(glm::vec3(0.0, -0.5, -1.0));
    sphere3.center(glm::vec3(1.0, 0.0, -1.0));
    sphere4.center(glm::vec3(0.0, 1.0, -5.0));
    sphere5.center(glm::vec3(0.0, 100.0, -5.0));
    sphere6.center(glm::vec3(0.0, 100.0, -5.0));



    sphere1.radius(0.5f);
    sphere2.radius(0.5f);
    sphere3.radius(0.5f);
    sphere4.radius(0.5f);
    sphere5.radius(0.5f);
    sphere6.radius(0.5f);

    Material red;
    red.albido = { 1.0f, 0.0f, 0.0f, 1.0f };
    red.smoothness = 0.0f;
    red.specularColor = glm::vec4(1);
    red.specularProbability = 0.0f;
    red.emissionColor = glm::vec4(1);
    red.emissionStrength = 0.0f;


    sphere1.material(red);
    sphere2.material(red);
    sphere3.material(red);
    sphere4.material(red);
    sphere5.material(red);
    sphere6.material(red);

    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);
    //spheres.push_back(sphere5);
    //spheres.push_back(sphere6);

    return spheres;
}

std::vector<Sphere> Scene::Load(std::string path)
{
    std::ifstream stream;
    stream.open(path);

    if (!stream.is_open())
    {
        std::cout << "ERROR: Failed Loading FILE" << std::endl;
        return spheres;
    }

    std::string line;
    Sphere sphere;
    Material mat;
    while (std::getline(stream, line))
    {
        std::string type = line.substr(0, line.find(" "));
        
        if (type == "Radius")
            sphere.radius(std::stof(line.substr(line.find("=") + 2, line.find("\n"))));
        else if (type == "Center")
        {
            int vecLoc = line.find("vec3");
            float x = std::stof(line.substr(vecLoc + 5, vecLoc + 12));
            float y = std::stof(line.substr(vecLoc + 15, vecLoc + 22));
            float z = std::stof(line.substr(vecLoc + 25, vecLoc + 32));
            sphere.center(glm::vec3(x, y, z));
        }
        else if (type == "Albido")
        {
            int vecLoc = line.find("vec3");
            float x = std::stof(line.substr(vecLoc + 5, vecLoc + 12));
            float y = std::stof(line.substr(vecLoc + 15, vecLoc + 22));
            float z = std::stof(line.substr(vecLoc + 25, vecLoc + 32));
            float w = std::stof(line.substr(vecLoc + 35, vecLoc + 42));
            mat.albido = glm::vec4(x, y, z, w);
        }
        else if (type == "Smoothness")
        {
            mat.smoothness = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
        }
        else if (type == "SpecularColor")
        {
            int vecLoc = line.find("vec3");
            float x = std::stof(line.substr(vecLoc + 5, vecLoc + 12));
            float y = std::stof(line.substr(vecLoc + 15, vecLoc + 22));
            float z = std::stof(line.substr(vecLoc + 25, vecLoc + 32));
            float w = std::stof(line.substr(vecLoc + 35, vecLoc + 42));
            mat.specularColor = glm::vec4(x, y, z, w);
        }
        else if (type == "SpecularProb")
        {
            mat.specularProbability = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
        }
        else if (type == "EmissionStrengh")
        {
            mat.emissionStrength = std::stof(line.substr(line.find("=") + 2, line.find("\n")));
        }
        else if (type == "EmissionColor")
        {
            int vecLoc = line.find("vec3");
            float x = std::stof(line.substr(vecLoc + 5, vecLoc + 12));
            float y = std::stof(line.substr(vecLoc + 15, vecLoc + 22));
            float z = std::stof(line.substr(vecLoc + 25, vecLoc + 32));
            float w = std::stof(line.substr(vecLoc + 35, vecLoc + 42));
            mat.emissionColor = glm::vec4(x, y, z, w);
        }
        else
        {
            sphere.material(mat);
            spheres.push_back(sphere);
        }
    }
    stream.close();


    return spheres;
}

void Scene::Save(std::string name)
{
    std::ofstream stream;
    stream.open(name);

    if (!stream.is_open())
    {
        std::cout << "ERROR: Failed Loading FILE" << std::endl;
        return;
    }

    for (int i = 0; i < spheres.size(); i++)
    {
        Sphere sphere = spheres[i];
        std::string number = std::to_string(i);
        std::string radius = "Radius " + number;
        std::string center = "Center " + number;
        std::string albido = "Albido " + number;
        std::string smoothness = "Smoothness " + number;
        std::string specularColor = "SpecularColor " + number;
        std::string specularProb = "SpecularProb " + number;
        std::string EmissionStrengh = "EmissionStrengh " + number;
        std::string EmissionColor = "EmissionColor " + number;


        stream << radius << " = " << sphere.radius() << "\n";
        stream << center << " = " << converter::ToString(sphere.center()) << "\n";
        stream << albido << " = " << converter::ToString(sphere.material().albido) << "\n";
        stream << smoothness << " = " << sphere.material().smoothness << "\n";
        stream << specularColor << " = " << converter::ToString(sphere.material().specularColor) << "\n";
        stream << specularProb << " = " << sphere.material().specularProbability << "\n";
        stream << EmissionStrengh << " = " << sphere.material().emissionStrength << "\n";
        stream << EmissionColor << " = " << converter::ToString(sphere.material().emissionColor) << "\n";
        stream << "\n";
    }

    stream.close();
}