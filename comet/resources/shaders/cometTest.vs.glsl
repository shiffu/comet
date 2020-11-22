#version 430 core

// Vertex attributes
layout (location = 0) in vec3 position;

// Instance attributes
layout (location = 10) in mat4 instance_model_matrix;

// View & Projection Matrices uniform
uniform mat4 vp_matrix;

out VS_OUT
{
    vec3 color;
} vs_out;

void main()
{
    vec4 world_position = instance_model_matrix * vec4(position, 1.0);

    float value = (world_position.z + 2.0f) / 4.0f;
    vs_out.color = vec3(value, value, value);

    gl_Position = vp_matrix * world_position;
}