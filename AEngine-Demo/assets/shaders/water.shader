#type vertex
#version 330 core
layout (location = 0) in vec3 position;

out vec2 textureCoords;

uniform mat4 u_projectionView;
uniform mat4 u_model;

void main()
{
	gl_Position = u_projectionView * u_model * vec4(position.x, 0.0, position.z, 1.0);
	textureCoords = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5);
}

#type fragment
#version 330 core

in vec2 textureCoords;

out vec4 fragColor;

uniform float u_tilingFactor;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

uniform float moveFactor;

const float waveStrength = 0.02;

void main()
{
	vec2 tc = textureCoords * u_tilingFactor;
	vec4 baseColor = vec4(0.0, 0.3, 0.8, 1.0);

	vec2 distortion = (texture(dudvMap, vec2(tc.x + moveFactor, tc.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(normalMap, vec2(-tc.x, tc.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;

	vec2 distortedCoords = tc + distortion;
	vec2 distortedCoords2 = tc + distortion2;

	vec4 dudvColor = texture(dudvMap, distortedCoords);
	vec4 normalColor = texture(normalMap, distortedCoords2);
	vec4 normalDudv = mix(dudvColor, normalColor, 0.3);

	fragColor = mix(baseColor, normalDudv, 0.3);
}