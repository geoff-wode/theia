#version 330

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColour;

smooth out vec4 voutColour;

uniform vec2 offset;

void main()
{
	gl_Position = inPosition;
	gl_Position.xy += offset;
	voutColour = inColour;
}
