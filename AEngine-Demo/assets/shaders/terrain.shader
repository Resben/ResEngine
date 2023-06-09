#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

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
	YValue = aPos.y + 0.5;
}

#type fragment
#version 330 core
in vec2 TexCoord;
in float YValue;

out vec4 FragColor;

uniform vec2 u_yRanges[10];
uniform sampler2D u_textures[10];
uniform int u_numTextures;
uniform float u_tilingFactor;

void main()
{
	vec2 texCoord = TexCoord * u_tilingFactor;
	vec4 texel1, texel2, resultColor;

	int index = 0;
	for(int i = 0; i < u_numTextures; i++)
	{
		float range = u_yRanges[i].y - u_yRanges[i].x;
		if (YValue >= u_yRanges[i].x && YValue < u_yRanges[i].y)
		{
			texel1 = texture(u_textures[i], texCoord);
			if (i != u_numTextures - 1)
			{
				texel2 = texture(u_textures[i + 1], texCoord);
				resultColor = mix(texel2, texel1, u_yRanges[i + 1].y - YValue);
			}
			else
			{
				resultColor = texel1;
			}
		}
	}

    FragColor = resultColor;
}
