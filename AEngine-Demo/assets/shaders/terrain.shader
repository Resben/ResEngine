#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out float YValue;
out vec3 vPos;

uniform mat4 u_projectionView;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_projectionView * u_transform * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	YValue = aPos.y + 0.5;
	vPos = vec3(u_transform * vec4(aPos, 1.0));
}

#type fragment
#version 330 core
in vec2 TexCoord;
in float YValue;
in vec3 vPos;

out vec4 FragColor;

// texture settings
uniform float u_yRanges[10];
uniform sampler2D u_textures[10];
uniform float u_tilingFactor;

// lighting intensity
uniform float u_minLightingIntensity;
uniform float u_maxLightingIntensity;

layout (location = 0) out vec4 FragPos;
layout (location = 2) out vec4 Albedo;

float calculateLightingFactor(float intensity)
{
	float taperedIntensity = smoothstep(0.0, 1.0, intensity);
	return mix(u_minLightingIntensity, u_maxLightingIntensity, taperedIntensity);
}

void main()
{
	vec2 texCoord = TexCoord * u_tilingFactor;
	vec4 texel1, texel2, texel3, resultColor;

	float factor1 = smoothstep(u_yRanges[0], u_yRanges[1], YValue);
	float factor2 = smoothstep(u_yRanges[1], u_yRanges[2], YValue);
	texel1 = texture(u_textures[0], texCoord);
	texel2 = texture(u_textures[1], texCoord);
	texel3 = texture(u_textures[2], texCoord);

	resultColor = mix(texel1, texel2, factor1);
	resultColor = mix(resultColor, texel3, factor2);

	// apply lighting
	resultColor *= calculateLightingFactor(YValue);
	
	FragPos = vec4(vPos, 1.0);
    Albedo = resultColor;
}
