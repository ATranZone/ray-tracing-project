#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <algorithm>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    //TODO determine the color

    vec3 light_source;
    vec3 light_color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    Ray light_ray;
    Hit temp_hit;

    //SHADOWS
    //search through the vector of lights located in render_world

    //Ambient is I_a = R_a * L_a. Combination of world.ambient_color, world.ambient_intensity and color_ambient
    //
    ambient = world.ambient_color * world.ambient_intensity * color_ambient;
    color += ambient;
    for(unsigned int i = 0; i < world.lights.size(); i++)
    {
        light_source = (world.lights.at(i)->position - intersection_point);
        light_color = (world.lights.at(i)->Emitted_Light(light_source.normalized()) / light_source.magnitude_squared());
        //also check if shadows should be calculated, if world.enable_shadows is true then check if theres an object between the intersection point and the light source
        //if there's an object blocking all light sources then return only the ambient light.
        if(world.enable_shadows)
        {
            //define the temp ray for the checks
            light_ray.direction = light_source.normalized();
            light_ray.endpoint = intersection_point;
            //check if the light ray intersects an object in the world
            temp_hit = world.Closest_Intersection(light_ray);
            
            if(temp_hit.object) //if there was an objected that was intersected
            {
                if(temp_hit.dist < light_source.magnitude_squared()) //If there is an object blocking all your light sources, then you should return only the ambient light component.
                {
                    continue; //only break from this specific iteration and continue with the for loop
                }
            }
        }
         //Phong Reflection Model is calculated as: ambient + diffuse + specular = phone reflection
    
         
         //Diffuse is R_d * L_d * max(n dot l,0)
         //Specular is R_s * Ls * max(cos(phi),0)^(alpha)

         //calculation for diffuse
         double diffuse_intensity = std::max(dot(normal, light_source.normalized()),0.0);
         diffuse = color_diffuse * light_color * diffuse_intensity;
    
         //calculation for specular
         //r=2(N⋅L)N−L
         //cos(phi) = v*r
         // R_s * L_s * max(v⋅r,0.0)^alpha
         vec3 v = ray.direction.normalized() * -1.0;
         vec3 r = (2.0 * (dot(normal,light_source.normalized())) * normal - light_source.normalized());
         double specular_intensity = pow(std::max(dot(v,r),0.0),specular_power);
         specular = color_specular * light_color * specular_intensity;

         //final calculation for the phong shader
         color = color + diffuse + specular;
    }
    return color;
}
