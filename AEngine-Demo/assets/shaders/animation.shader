#type vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 u_finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0);
    vec3 totalNormal = vec3(0.0);

    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(pos, 1.0);
            break;
        }
        vec4 localPosition = u_finalBonesMatrices[boneIds[i]] * vec4(pos,1.0);
        totalPosition += localPosition * weights[i];

        vec3 localNormal = mat3(u_finalBonesMatrices[boneIds[i]]) * norm;
        totalNormal += localNormal * weights[i];
   }

    totalNormal = normalize(totalNormal);

    gl_Position =  u_projectionView * u_transform * totalPosition;

    vPos = vec3(u_transform * totalPosition);
	vNormal = mat3(transpose(inverse(u_transform))) * totalNormal;
	vTexCoords = tex;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoords;

layout (location = 0) out vec4 FragPos;
layout (location = 1) out vec4 Normal;
layout (location = 2) out vec4 Albedo;

uniform vec4 u_baseColor;
uniform int u_hasTextures;
uniform sampler2D u_texture1;

void main()
{
    FragPos = vec4(vPos, 1.0);
	Normal = vec4(normalize(vNormal), 1.0);

	if(u_hasTextures == 1)
	{
		Albedo = texture(u_texture1, vTexCoords);
	}
	else
	{
		Albedo = u_baseColor;
	}
}