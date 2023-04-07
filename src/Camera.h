#pragma once
#include "glm/glm.hpp"
#include "Ray.h"

class camera
{
public:
    camera(
        glm::vec3 lookfrom,
        glm::vec3 lookat,
        glm::vec3 vup,
        float vfov, // vertical field-of-view in degrees
        float aspect_ratio
    )
    {
        m_lookfrom = lookfrom;
        m_lookat = lookat; 
        m_vup = vup;
        m_vfov = vfov;
        m_aspect_ratio = aspect_ratio;

        float h = glm::tan(vfov / 2);
        float viewport_height = 2.0 * h;
        float viewport_width = aspect_ratio * viewport_height;

        glm::vec3 w = glm::normalize(lookfrom - lookat);
        glm::vec3 u = glm::normalize(glm::cross(vup, w));
        glm::vec3 v = glm::cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
    }

    Ray get_ray(float s, float t) const
    {
        return Ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

    void Recalculate()
    {
        float h = glm::tan(m_vfov / 2);
        float viewport_height = 2.0 * h;
        float viewport_width = m_aspect_ratio * viewport_height;

        glm::vec3 w = glm::normalize(m_lookfrom - m_lookat);
        glm::vec3 u = glm::normalize(glm::cross(m_vup, w));
        glm::vec3 v = glm::cross(w, u);

        origin = m_lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
    }

    glm::vec3 m_lookfrom;
    glm::vec3 m_lookat;
    glm::vec3 m_vup;
    float m_vfov;
    float m_aspect_ratio;
private:

    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};