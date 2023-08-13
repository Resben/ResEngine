#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

void main()
{
    gl_Position = u_projectionView * u_transform * vec4(aPos, 1.0);
    FragPos = vec3(u_transform * vec4(aPos, 1.0));
	Normal = aNormal;
}

#type fragment
#version 330 core

uniform vec4 u_baseColor;
uniform vec3 u_specularColor;
uniform vec3 u_ambientColor;
uniform vec3 u_emissionColor;
uniform float u_shininess;
uniform float u_reflectivity;
uniform float u_ior;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = u_baseColor.rgb * diff;

    vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0) - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess);
    vec3 specular = u_specularColor * spec;

    vec3 ambient = u_ambientColor;

    vec3 emission = u_emissionColor;

    vec3 finalColor = (diffuse + specular + ambient + emission) * u_reflectivity;

    float fresnel = pow(1.0 - dot(Normal, viewDir), 5.0);
    finalColor = mix(finalColor, vec3(1.0, 1.0, 1.0), fresnel * (1.0 - u_ior));

    FragColor = vec4(finalColor, 1.0);
}
