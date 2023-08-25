#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    //TODO
    double min_t = std::numeric_limits<double>::max(); //set min_t to a large value
    Hit closest_hit_object = {0,0,0,}; //hit that we will be returning
    for(unsigned int i=0; i<objects.size(); i++) //for all objects o do
    {
        Hit check = objects[i]->Intersection(ray,small_t); //use  o->Intersect to get closest hit with the object
        if(check.dist<min_t && check.dist>small_t) //if hit is the closest so far and larger than small_t then
        {                                          //
            closest_hit_object = check;            //store the hit as the closest hit
        }
    }
    return closest_hit_object;                      //return closest hit
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    //TODO set up the initial view ray here
    Ray ray;
    vec3 ray_end_point = camera.position; //endpoint of a ray is where the camera is in the environment
    vec3 direction = this->camera.World_Position(pixel_index); 
    ray.endpoint = ray_end_point; //setting the ray's endpoint to be the camera position
    ray.direction = (direction - ray_end_point).normalized(); //unit vector from the camera position to the world position of the pixel
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    vec3 empty;
    //TODO determine the color here
    Hit closest_hit = Closest_Intersection(ray); //grab closest hit with an object
    if(closest_hit.object != NULL)  //if the closest object hit is not nothing then we have a hit
    {
        //get the intersection point using the ray object and the normal using the object pointer inside the hit object
        vec3 intersected_point = ray.Point(closest_hit.dist);  
        vec3 normal = closest_hit.object->Normal(intersected_point,closest_hit.part);
        color = closest_hit.object->material_shader->Shade_Surface(ray, intersected_point, normal, recursion_depth); //set the color using shader_surface 
    }
    else
    {
        color = background_shader->Shade_Surface(ray, empty, empty, recursion_depth);
    }
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
