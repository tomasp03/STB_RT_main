#pragma once
#include "Material.h"
#include "..\\HitInfo.h"
#include "..\\Ray.h"
#include "glm/glm.hpp"

class Shape
{
protected:
	Material m_mat;

public:
	Material material() { return m_mat; }
	void material(Material mat) { m_mat = mat; }
	Material* materialPtr() { return &m_mat; }
	virtual HitInfo Intersection(const Ray& ray) = 0;
};

