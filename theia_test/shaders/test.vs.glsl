
layout (location = 0) in vec3 inPosition;

out vec3 vertexWorldPos;
out vec3 vertexSurfacePos;
out vec3 vertexSurfaceNormal;

void main()
{
	vec4 P = vec4(inPosition + EyePosition, 1);
	gl_Position = WorldViewProjection * P;

	vec3 N = normalize(inPosition);
	vertexSurfaceNormal = mat3(World) * N;
	vertexWorldPos = World * vec4(inPosition,1);
	vertexSurfacePos = inPosition;
}
