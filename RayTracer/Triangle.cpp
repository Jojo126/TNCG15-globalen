#include "Triangle.h"

bool Triangle::rayIntersection(Ray myRay) { //pass by refernce?
    // F�r in en ray med triangeln
    // Hitta sk�rningspunkten mellan triangeln & rayen
    // f�r att se om dom sk�r or nah
    // Om de sk�r, returnera true, annars false
    // Om sk�r, vart �r sk�rningspunkten? Tilldela sen till ray vi fick in
    // Avst�nd mellan �gat & sk�rningspunkten

    const float EPSILON = 0.0000001;
    Vertex vertex1 = this->v1;
    Vertex vertex2 = this->v2;
    Vertex vertex3 = this->v3;
    
    // double
    double a,f,u,v;
    Direction h, s, q;

    // Tv� av triangelns kanter (vektor)
    Vertex edge1 = vertex2 - vertex1;
    Direction dirEdge1 = Direction(edge1.x, edge1.y, edge1.z);
    Vertex edge2 = vertex3 - vertex1;
    Direction dirEdge2 = Direction(edge2.x, edge2.y, edge2.z);

    Direction rayDir = myRay.dir;

    h = rayDir.crossProduct(dirEdge2); // h �r kryssprodukt mellan h & edge2
    a = dirEdge1.dotProduct(h);        // a �r skal�rprodukt mellan h (kryssprodukten) & edge1
    
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.


    f = 1.0/a;      // f (skal�r) normaliserar
    Vertex tempS = myRay.startPoint - vertex1;  // vektor
    s = Direction(tempS.x, tempS.y, tempS.z);
    u = f * s.dotProduct(h);  // u (skal�r) mellan s (vektor) & h (vektor), f normaliserar

    if (u < 0.0 || u > 1.0)
        return false;


    q = s.crossProduct(dirEdge1);  // q (vektor) �r kryssprodukt mellan s (vektor) & edge1
    v = f * rayDir.dotProduct(q); // v (skal�r) �r skal�rprodukt mellan inkommande Ray (direction?) & vektor q

    if (v < 0.0 || u + v > 1.0)
        return false;


    // t �r the intersectionPoint !!
    double t = f * dirEdge2.dotProduct(q);

    if (t > EPSILON) // ray intersection
    {
        myRay.endPoint = myRay.startPoint + Vertex(rayDir.x,rayDir.y,rayDir.z,1) * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
            
    return false;
}


/*
bool RayIntersectsTriangle(Vector3D rayOrigin,
                           Vector3D rayVector,
                           Triangle* inTriangle,
                           Vector3D& outIntersectionPoint)
{
    const float EPSILON = 0.0000001;
    Vector3D vertex0 = inTriangle->vertex0;
    Vector3D vertex1 = inTriangle->vertex1;
    Vector3D vertex2 = inTriangle->vertex2;
    Vector3D edge1, edge2, h, s, q;
    float a,f,u,v;

    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;

    h = rayVector.crossProduct(edge2); // h �r kryssprodukt mellan h & edge2
    a = edge1.dotProduct(h);           // a �r skal�rprodukt mellan h (kryssprodukten) & edge1

    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.

    f = 1.0/a;      // f (skal�r) normaliserar
    s = rayOrigin - vertex0;  // vektor
    u = f * s.dotProduct(h);  // u (skal�r) mellan s (vektor) & h (vektor), f normaliserar

    if (u < 0.0 || u > 1.0)
        return false;

    q = s.crossProduct(edge1);  // q (vektor) �r kryssprodukt mellan s (vektor) & edge1
    v = f * rayVector.dotProduct(q); // v (skal�r) �r skal�rprodukt mellan inkommande Ray (direction?) & vektor q

    if (v < 0.0 || u + v > 1.0)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dotProduct(q);

    if (t > EPSILON) // ray intersection
    {
        outIntersectionPoint = rayOrigin + rayVector * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}
*/