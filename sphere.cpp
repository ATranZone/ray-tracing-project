#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO
    //refer to lecture 3 notes for derivation/explanation
    //derive at^2+bt+c then use the discriminant to determine our hit for a sphere. only treat the discriminant with 2 solutions as a hit, treat other cases as no solutions/hit
    Hit sphere_intersection = {0,0,0}; //initialize because it doesnt work if u dont
    vec3 e = ray.endpoint - center;
    //a not required since its a coefficient of 1
    double b = dot((ray.direction + ray.direction), e);
    double c = dot(e,e) - (radius * radius); 
    double discriminant = (b * b) - (4 * c);
    double t_1 = (-b + sqrt(discriminant))/2;
    double t_2 = (-b - sqrt(discriminant))/2;

    if(discriminant > 0)
    {   
        if(t_1 > small_t)
        {
            if(t_2 > small_t)
            {
                sphere_intersection.object = this;
                sphere_intersection.dist = t_2;
                sphere_intersection.part = part;
            }
            if(t_2 < 0)
            {
                sphere_intersection.object = this;
                sphere_intersection.dist = t_1;
                sphere_intersection.part = part;
            }
        }
    }
    return sphere_intersection;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    //TODO compute the normal direction
    normal = point - center;
    return normal.normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
