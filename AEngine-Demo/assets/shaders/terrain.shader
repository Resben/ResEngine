#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out float YValue;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

uniform vec2 u_yRanges[10];
uniform sampler2D u_textures[10];
uniform int u_numTextures;


void main()
{
	gl_Position = u_projectionView * u_transform * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	YValue = aPos.y;
}

#type fragment
#version 330 core
in vec2 TexCoord;
in float YValue;

out vec4 FragColor;

uniform vec2 u_yRanges[10];
uniform sampler2D u_textures[10];
uniform int u_numTextures;

void main()
{
	int index = 0;
	for(int i = 0; i < u_numTextures; i++)
	{
		if(YValue >= u_yRanges[i].x && YValue < u_yRanges[i].y)
		{
			index = i;
			break;
		}
	}

    FragColor = texture(u_textures[index], TexCoord);
}
