#version 430 core

layout (location = 0) in vec3 position;
layout (location = 10) in mat4 instance_model_matrix;
layout (location = 14) in uint instance_materialID;

out vec4 pass_color;

uniform mat4 vp_matrix;
uniform vec4 u_flatColor[10];


void main()
{
    pass_color = u_flatColor[instance_materialID];
    gl_Position = vp_matrix * instance_model_matrix * vec4(position, 1.0);
}