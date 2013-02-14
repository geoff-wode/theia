#version 330

smooth in vec4 voutColour;
out vec4 fragColour;

void main()
{
	fragColour = voutColour;
}
