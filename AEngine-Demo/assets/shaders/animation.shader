#type vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 u_finalBonesMatrices[MAX_BONES];

out vec2 TexCoords;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(pos, 1.0f);
            break;
        }
        vec4 localPosition = u_finalBonesMatrices[boneIds[i]] * vec4(pos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(u_finalBonesMatrices[boneIds[i]]) * norm;
   }

    gl_Position =  u_projectionView * u_transform * totalPosition;
    TexCoords = tex;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_texture1;

void main()
{
    FragColor = texture(u_texture1, TexCoords);
}