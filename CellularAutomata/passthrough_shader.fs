#version 330 core

in vec2 fragTexCoord;
in vec4 fragColour;

out vec4 finalColour;

uniform int particles[];
uniform int windowWidth = 0;

const vec4 ParticleColours[] = vec4[](
	vec4(0, 0, 0, 0),		 // None
	vec4(235, 235, 73, 255), // Sand
	vec4(59, 121, 163, 255)  // Water
);

void main()
{
	finalColour = ParticleColours[0];
	if(windowWidth <= 0)
		return;
	// finalColour = ParticleColours[(int)(gl_FragCoord.y * windowWidth + gl_FragCoord.x)];
}