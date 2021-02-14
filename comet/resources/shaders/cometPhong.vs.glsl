#version 430 core

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

// Vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoordinate;

// Instance attributes
layout (location = 10) in mat4 instance_model_matrix;
layout (location = 14) in uint instance_materialID;

// View & Projection Matrices uniforms
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear_attenuation;
    float quadratic_attenuation;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Light uniforms
uniform DirectionalLight dir_light;

// Point Lights uniforms
uniform uint active_point_lights_count;
uniform uint active_spot_lights_count;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

out VS_OUT
{
    vec2 tex_coord;
    vec3 normal;
    vec3 to_camera;
    vec3 to_point_lights[MAX_POINT_LIGHTS];
    vec3 to_spot_lights[MAX_SPOT_LIGHTS];
    float distances_to_point_lights[MAX_POINT_LIGHTS];
    flat uint instance_materialID;
} vs_out;


void main()
{
    vs_out.tex_coord = texCoordinate;
    
    vec3 world_normal = mat3(instance_model_matrix) * normal;
    world_normal = normalize(world_normal);
    vec4 world_position = instance_model_matrix * vec4(position, 1.0);
    
    for (int i = 0; i < active_point_lights_count; i++)
    {
        vs_out.to_point_lights[i] = point_lights[i].position - world_position.xyz;
        vs_out.distances_to_point_lights[i] = length(vs_out.to_point_lights[i]);
        vs_out.to_point_lights[i] = normalize(vs_out.to_point_lights[i]);
    }
    
    for (int i = 0; i < active_spot_lights_count; i++)
    {
        vs_out.to_spot_lights[i] = normalize(spot_lights[i].position - world_position.xyz);
    }

    vec3 to_camera = (inverse(view_matrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_position.xyz;

    vs_out.normal = world_normal;
    vs_out.to_camera = to_camera;
    vs_out.instance_materialID = instance_materialID;

    gl_Position = projection_matrix * view_matrix * world_position;
}