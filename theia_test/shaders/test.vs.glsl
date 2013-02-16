#version 330

layout (location = 0) in vec4 inPosition;

uniform mat4	ViewProjection;
uniform mat4	World;

void main()
{
	gl_Position = ViewProjection * World * inPosition;
}
