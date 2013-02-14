#version 330

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColour;

smooth out vec4 voutColour;

uniform float time;
uniform float duration;

void main()
{
	float scale = TWO_PI / duration;
	float now = -mod(time, duration);
	vec4 offset = vec4(
		cos(now * scale) * 0.5f,
		sin(now * scale) * 0.5f,
		0, 0);

	gl_Position = inPosition + offset;

	voutColour = inColour;
}
