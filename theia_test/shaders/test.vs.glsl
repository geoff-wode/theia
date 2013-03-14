
layout (location = 0) in vec3 inPosition;

uniform float Radius;

out vec3 vertexWorldPos;
out vec3 vertexSurfacePos;
out vec3 vertexSurfaceNormal;

void main()
{
  // normalise the incoming vertex to create a unit sphere:
	vec3 N = normalize(inPosition);

  // Scale the sphere by the radius:
	vec4 P = vec4(N * Radius, 1);

  // Compute final perspective transform of the sphere:
	gl_Position = WorldViewProjection * P;

  // Compute just the object transform to world space (no view or perspective):
	vertexWorldPos = World * P;

  // Rotate the surface normal:
	vertexSurfaceNormal = mat3(World) * N;

  // Pass the unit sphere location to the fragment shader:
	vertexSurfacePos = N;
}
