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
uniform samplerCube u_hdr;
uniform vec3 u_camPos;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
	vec3 I = normalize(FragPos - u_camPos);
	vec3 R = reflect(I, normalize(Normal));

	vec3 hdr = texture(u_hdr, R).rgb;

		// Last is reflection add it to the uniform later
	FragColor = mix(u_baseColor, vec4(hdr, 1.0), 0.5);
}
