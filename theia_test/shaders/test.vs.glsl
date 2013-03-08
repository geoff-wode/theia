
layout (location = 0) in vec3 inPosition;

out vec3 vertexWorldPos;
out vec3 vertexSurfacePos;
out vec3 vertexNormal;
out vec3 vertexToLight;

void main()
{
	vec4 P = vec4(inPosition, 1);
	gl_Position = WorldViewProjection * P;

	vec3 N = normalize(inPosition);
	vertexNormal = mat3(World) * N;
	vertexWorldPos = World * vec4(inPosition,1);
	vertexSurfacePos = inPosition;
}
