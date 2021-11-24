#define _USE_MATH_DEFINES
#include <cmath>

#include "Triangle.h"
#include "Ray.h"

ColorDbl Triangle::getLightColor(Ray ray, glm::vec3 light, float lightIntensity) {
    
    //lightIntensity = 1;

    // Decrease intensity for incoming light when surface is far away from the lightsource 
    float r = glm::length(ray.endPoint - light);
    // incidentLightEnergy = lightIntensity * lightColor / (4 * PI * r^2), r = distance between the lightPos and the intersectionPoint
    float falloff = 1.0 * lightIntensity / (r*r); //4 * float(M_PI) * 

    // Get surface normal and compare with light source normal to decrease incoming light from depending on the angle
    glm::vec3 lightDirection = glm::normalize(light - ray.endPoint);
    double cosTheta = std::max(0.0f, glm::dot(lightDirection, normal.direction));
    // Diffuse surface color = surfaceAlbedo / PI * incidentLightEnergy * cos(theta)
    return ColorDbl(rgb.R / M_PI * falloff * cosTheta, rgb.G / M_PI * falloff * cosTheta, rgb.B / M_PI * falloff * cosTheta);
}

bool Triangle::getIntersectionPoint(Ray& ray, float& t_nearest) {
    // Rename triangle vertices
    glm::vec3 p_s = ray.startPoint;
    //glm::vec3 p_e = pixelCoord;
    glm::vec3 v0 = this->v1.coords;
    glm::vec3 v1 = this->v2.coords;
    glm::vec3 v2 = this->v3.coords;

    glm::vec3 T = p_s - v0; // T = P_s - v_0
    glm::vec3 E1 = v1 - v0;
    glm::vec3 E2 = v2 - v0;
    glm::vec3 D = ray.direction.direction; // !! D = P_e - P_s
    glm::vec3 P = glm::cross(D, E2); // P = D x E_2
    glm::vec3 Q = glm::cross(T, E1); // Q = T x E_1 

    double denom = glm::dot(P, E1);
    float t;
    if (denom != 0) {
        t = glm::dot(Q, E2) / denom;
        double u = glm::dot(P, T) / denom;
        double v = glm::dot(Q, D) / denom;

        // Check if point inside triangle
        // t > 0: triangle in front of camera 
        if (t > 0 && u >= 0 && v >= 0 && u + v <= 1) {

            // Need only color from triangle with nearest intersection point to p_s
            if (t_nearest > t) {
                t_nearest = t;

                // Get calculated intersectionpoint and color for pixel from traced ray
                ray.endPoint = ray.startPoint + ray.direction.direction * t;

                return true;
            }
        }
    }
    return false;
}