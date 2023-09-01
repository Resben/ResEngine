#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_projectionView * u_transform * vec4(aPos, 1.0);
	vPos = vec3(u_transform * vec4(aPos, 1.0));
	vNormal = mat3(transpose(inverse(u_transform))) * aNormal;
	vTexCoords = aTexCoord;
}

#type fragment
#version 330 core

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoords;

layout (location = 0) out vec4 FragPos;
layout (location = 1) out vec4 Normal;
layout (location = 2) out vec4 Albedo;

uniform sampler2D u_texture1;

void main()
{
    FragPos = vec4(vPos, 1.0);
	Normal = vec4(normalize(vNormal), 1.0);
	Albedo = texture(u_texture1, vTexCoords);
}
