#version 430 core

#define MAX_MATERIAL_INSTANCES 32
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

in VS_OUT
{
    vec2 tex_coord;
    vec3 normal;
    vec3 to_camera;
    vec3 to_point_lights[MAX_POINT_LIGHTS];
    vec3 to_spot_lights[MAX_SPOT_LIGHTS];
    float distances_to_point_lights[MAX_POINT_LIGHTS];
    flat uint instance_materialID;
} fs_in;

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

struct MaterialInstance
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
    int albedo_texture_idx;
};


// Material uniforms
uniform MaterialInstance material_instances[MAX_MATERIAL_INSTANCES];
uniform sampler2D white_1x1_texture;
uniform sampler2DArray albedo_textures;

// Lights uniforms
uniform uint active_point_lights_count;
uniform uint active_spot_lights_count;
uniform DirectionalLight dir_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

out vec4 color;

vec3 compute_common_light_effect(vec3 to_light, vec3 light_ambient, vec3 light_diffuse, vec3 light_specular, vec3 normal, vec3 to_camera)
{
    MaterialInstance material_instance = material_instances[fs_in.instance_materialID];
    // vec3 tex_color = texture(albedo_textures, vec3(fs_in.tex_coord, material_instance.albedo_texture_idx)).xyz;
    vec3 tex_color;
    
    if (material_instance.albedo_texture_idx >= 0)
    {
        tex_color = texture(albedo_textures, vec3(fs_in.tex_coord, material_instance.albedo_texture_idx)).xyz;
    }
    else
    {
        tex_color = texture(white_1x1_texture, vec2(0)).xyz;
    }

    // Ambient Light
    vec3 ambient = light_ambient * tex_color;

    // Directional Light Diffuse component
    float diffuse_factor = max(0.0, dot(normal, to_light));
    vec3 diffuse = material_instance.diffuse * tex_color * light_diffuse * diffuse_factor;

    // Directional Light Specular component
    vec3 light_reflexion = reflect(-to_light, normal);
    float specular_factor = pow(max(0.0, dot(light_reflexion, to_camera)), material_instance.shininess);
    vec3 specular = specular_factor * light_specular * material_instance.specular;

    return (ambient + diffuse + specular);
}

vec3 compute_dir_light_effect(DirectionalLight light, vec3 normal, vec3 to_camera)
{
    vec3 to_light = normalize(-light.direction);

    return compute_common_light_effect(to_light, light.ambient, light.diffuse, light.specular, normal, to_camera);
}

vec3 compute_point_light_effect(PointLight light, vec3 normal, vec3 to_camera, vec3 to_light, float distance)
{
    vec3 result = compute_common_light_effect(to_light, light.ambient, light.diffuse, light.specular, normal, to_camera);

    float attenuation;
    attenuation = 1.0 / (1.0 + light.linear_attenuation * distance
                        + light.quadratic_attenuation * distance * distance);

    return result * attenuation;
}

vec3 compute_spot_light_effect(SpotLight light, vec3 normal, vec3 to_camera, vec3 to_light)
{
    vec3 result;
    float theta = dot(to_light, normalize(-light.direction));
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    result = compute_common_light_effect(to_light, light.ambient, light.diffuse, light.specular, normal, to_camera);
    result *= intensity;

    return result;
}

void main()
{
    vec3 unit_normal = normalize(fs_in.normal);
    vec3 unit_to_camera = normalize(fs_in.to_camera);

    // Directional Light
    vec3 frag_color;
    frag_color = compute_dir_light_effect(dir_light, unit_normal, unit_to_camera);

    // Point Light(s)
    for(int i = 0; i < active_point_lights_count; i++)
    {
        frag_color += compute_point_light_effect(point_lights[i], unit_normal, unit_to_camera,
                                                    fs_in.to_point_lights[i], fs_in.distances_to_point_lights[i]);
    }

    // Spot Light(s)
    for(int i = 0; i < active_spot_lights_count; i++)
    {
        frag_color += compute_spot_light_effect(spot_lights[i], unit_normal, unit_to_camera,
                                                    fs_in.to_spot_lights[i]);
    }

    color = vec4(frag_color, 1.0f);
}