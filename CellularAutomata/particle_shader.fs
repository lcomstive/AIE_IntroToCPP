#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColour;

uniform int particles[];
uniform vec2 resolution = vec2(0, 0);

const vec4 ParticleColours[] = vec4[](
	vec4(0, 0, 0, 0),		 // None
	vec4(235, 235, 73, 255), // Sand
	vec4(59, 121, 163, 255)  // Water
);

void main()
{
	// finalColour = ParticleColours[particles[(int)(gl_FragCoord.y * windowWidth + gl_FragCoord.x)]];

	vec2 screenspace = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y);
	int particleIndex = int(screenspace.y * resolution.x + screenspace.x);
	finalColour = ParticleColours[particles[particleIndex]];
}