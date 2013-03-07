#version 330

layout (location = 0) in vec3 inPosition;

uniform float	Radius;

out vec3 vertexWorldPos;
out vec3 vertexSurfacePos;
out vec3 vertexNormal;

void main()
{
	vec3 N = normalize(inPosition);
	vec4 P = vec4(N * Radius, 1);

	gl_Position = WorldViewProjection * P;

	vertexNormal = mat3(World) * N;
	vertexWorldPos = World * P;
	vertexSurfacePos = P.xyz;
}
