#type vertex
#version 330 core

in vec2 position;

out vec4 clipSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	clipSpace = projection * view * model * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;
}

#type fragment
#version 330 core

in vec4 clipSpace;

out vec4 fragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main()
{
	vec2 normalisedDeviceCoords = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 refractTexCoords = vec2(normalisedDeviceCoords.x, normalisedDeviceCoords.y);
	vec2 reflectTexCoords = vec2(normalisedDeviceCoords.x, -normalisedDeviceCoords.y);
	
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	//fragColor = mix(reflectColor, refractColor, 0.5);
	fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}