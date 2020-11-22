#version 430 core

// Vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoordinate;

// Instance attributes
layout (location = 10) in mat4 instance_model_matrix;
layout (location = 14) in uint instance_materialID;

// View & Projection Matrices uniforms
uniform mat4 vp_matrix;
uniform mat4 view_matrix;

// Light(s) uniforms
uniform vec3 light_position = vec3(-5.0, 10.0, 10.0);

out VS_OUT
{
    vec3 normal;
    vec3 to_light;
    vec3 to_camera;
    flat uint instance_materialID;
} vs_out;


void main()
{
    vec3 world_normal = mat3(instance_model_matrix) * normal;
    world_normal = normalize(world_normal);
    vec4 world_position = instance_model_matrix * vec4(position, 1.0);
    vec3 to_light = light_position - world_position.xyz;
    to_light = normalize(to_light);
    vec3 to_camera = (inverse(view_matrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_position.xyz;

    vs_out.normal = world_normal;
    vs_out.to_light = to_light;
    vs_out.to_camera = to_camera;
    vs_out.instance_materialID = instance_materialID;

    gl_Position = vp_matrix * world_position;
}