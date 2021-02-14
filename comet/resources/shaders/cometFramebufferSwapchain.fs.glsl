#version 430 core

in VS_OUT
{
    vec2 texCoords;
} fs_in;

out vec4 color;

uniform sampler2D framebufferTexture;

void main()
{
    color = texture(framebufferTexture, fs_in.texCoords);
}