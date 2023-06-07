#type vertex
#version 330 core
layout (location = 0) in vec3 position;

out vec2 textureCoords;

uniform mat4 u_projectionView;
uniform mat4 u_model;

void main()
{
	gl_Position = u_projectionView * u_model * vec4(position.x, 0.0, position.z, 1.0);
	textureCoords = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * tiling;
}

#type fragment
#version 330 core

in vec2 textureCoords;

out vec4 fragColor;

uniform sampler2D dudvMap;

void main()
{
	vec2 distortion = (texture(dudvMap, vec2(textureCoords.x, textureCoords.y)).rg * 2.0 - 1.0);
	
	fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}