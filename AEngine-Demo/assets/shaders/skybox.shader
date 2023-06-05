#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;

uniform mat4 u_projectionView;

void main()
{
	TexCoords = aPos;
	vec4 pos = u_projectionView * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 330 core

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoords);
}