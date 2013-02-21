#version 330

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4	ViewProjection;
uniform mat4	World;

out vec3 vertexPosition;
out vec3 vertexNormal;

void main()
{
	gl_Position = ViewProjection * World * inPosition;

	vertexPosition = inPosition.xyz;
	vertexNormal = inNormal;
}
