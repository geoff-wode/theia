#version 330

layout (location = 0) in vec3 inPosition;

uniform mat4	Model;
uniform float	Radius;
uniform mat4	MVP;

out vec3 vertexWorldPos;
out vec3 vertexSurfacePos;
out vec3 vertexNormal;

void main()
{
	vec3 N = normalize(inPosition);
	vec4 P = vec4(N * Radius, 1);

	gl_Position = MVP * P;

	vertexNormal = mat3(Model) * N;
	vertexWorldPos = Model * P;
	vertexSurfacePos = P.xyz;
}
