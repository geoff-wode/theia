#version 330

layout (location = 0) in vec3 inPosition;

uniform mat4	ViewProjection;
uniform mat4	World;
uniform float	Radius;

out vec3 vertexWorldPos;
out vec3 vertexObjectPos;

void main()
{
	vec3 P = normalize(inPosition);
	vec4 worldPos = World * vec4(Radius * P, 1);
	gl_Position = ViewProjection * worldPos;

	vertexWorldPos = worldPos.xyz;
	vertexObjectPos = inPosition.xyz;
}
