#type vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoords;

uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * vec4(aPos, 0.0, 1.0);
	TexCoords = vec2(aTex.x, aTex.y);
}

#type fragment
#version 330 core
in vec2 TexCoords;

out vec4 colour;

uniform sampler2D u_texture;
uniform vec3 u_fontColour;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, TexCoords).r);
	colour = vec4(u_fontColour, 1.0) * sampled;
}