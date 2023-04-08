#pragma once

#include "Sphere.h"
#include "Files.h"
#include "ToString.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <string>

class Scene
{
private:
    std::vector<Sphere> spheres;
    Files file;

public:
    std::vector<Sphere> Load();
    std::vector<Sphere> Load(std::string path);

    void Save(std::string name, std::vector<Sphere> spheres);
};
