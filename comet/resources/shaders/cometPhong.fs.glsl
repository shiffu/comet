#version 430 core

in VS_OUT
{
    vec3 normal;
    vec3 to_light;
    vec3 to_camera;
    flat uint instance_materialID;
} fs_in;

uniform vec3 light_diffuse = vec3(1.0);
uniform vec3 light_ambient = vec3(0.07, 0.0, 0.0);

// Material uniforms
uniform vec3 material_diffuse[32];
uniform vec3 material_specular[32];
uniform float material_shininess[32];

out vec4 color;

void main()
{
    vec3 unit_normal = normalize(fs_in.normal);
    vec3 unit_to_light = normalize(fs_in.to_light);
    vec3 unit_to_camera = normalize(fs_in.to_camera);

    // Diffuse component
    float diffuse_factor = max(0.0, dot(unit_normal, unit_to_light));
    vec3 diffuse = material_diffuse[fs_in.instance_materialID] * light_diffuse * diffuse_factor;

    // Specular component
    vec3 light_reflexion = reflect(-unit_to_light, unit_normal);
    float specular_factor = pow(max(0.0, dot(light_reflexion, unit_to_camera)), material_shininess[fs_in.instance_materialID]);
    vec3 specular = specular_factor * material_specular[fs_in.instance_materialID];

    color = vec4(light_ambient + diffuse + specular, 1.0f);
}