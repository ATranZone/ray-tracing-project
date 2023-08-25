#include "plane.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{
    //TODO
    //ray-plane intersection formula: ((p-e) dot n)/ u dot n

    Hit plane_intersection = {0,0,0}; //initialize because it doesnt work if u dont
    vec3 p_minus_e = x1 - ray.endpoint; //e is the start of the ray
    double numerator = dot(p_minus_e, normal);
    double denominator = dot(ray.direction, normal);
    //avoid case 2 where u dot n is 0 meaning ray never touches the plane
    if(denominator != 0)
    {
        double t = numerator/denominator;
        if(t < small_t)
        {
            return plane_intersection;
        }
        if(t > small_t) //if true then there is an intersection
        {
            plane_intersection.object = this;
            plane_intersection.dist = t;
            plane_intersection.part = part;
        }
    }

    return plane_intersection;
}

vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}
