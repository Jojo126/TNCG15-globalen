#define _USE_MATH_DEFINES
#include <cmath>

#include "Triangle.h"
#include "Ray.h"

ColorDbl Triangle::getLightColor(Ray ray, glm::vec3 light, Triangle surfaceObject, float lightIntensity) {

    // TODO: make sure correct falloff value
    // Decrease intensity for incoming light when surface is far away from the lightsource 
    float r = glm::length(ray.endPoint - light);
    glm::vec3 distanceShade = glm::vec3(1.0, 1.0, 1.0) * lightIntensity / (4 * float(M_PI) * sqrt(r));

    // Get surface normal and compare with light source normal to decrease incoming light from an angle
    glm::vec3 lightDirection = glm::normalize(light - ray.endPoint);
    double angleShade = std::max(0.0f, glm::dot(lightDirection, surfaceObject.normal.direction));

    return ColorDbl(surfaceObject.rgb.R * distanceShade.r * angleShade, surfaceObject.rgb.G * distanceShade.g * angleShade, surfaceObject.rgb.B * distanceShade.b * angleShade);
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