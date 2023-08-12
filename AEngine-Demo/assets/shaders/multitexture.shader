#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_projectionView * u_transform * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#type fragment
#version 330 core
in vec2 TexCoord;

out vec4 FragColor;



uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;
uniform sampler2D u_texture5;
uniform sampler2D u_texture6;
uniform sampler2D u_texture7;
uniform sampler2D u_texture8;
uniform sampler2D u_texture9;
uniform sampler2D u_texture10;
uniform sampler2D u_texture11;
uniform sampler2D u_texture12;
uniform sampler2D u_texture13;
uniform sampler2D u_texture14;
uniform sampler2D u_texture15;
uniform sampler2D u_texture16;
uniform sampler2D u_texture17;

void main()
{
    FragColor = texture(u_texture1, TexCoord);
}
