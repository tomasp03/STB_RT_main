#pragma once

#include "Shapes/AllShapes.h"
#include "Files.h"
#include "ToString.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

struct Scene
{
    std::vector<Material> materials;
    Files file;
    glm::ivec2 dimensions;
    std::vector<Sphere> Load();
    std::vector<Sphere> Load(std::string path);
    void Save(std::string name);

    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
};
