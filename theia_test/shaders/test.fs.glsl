
uniform MaterialStruct Material;

// Variables controlling a lat/lon grid.
// The x components define the values for longitude, y components define latitude values.
// For resolution, the actual number of lines is given as 1/resolution in [0,1], i.e. for
// 20 lines, set resolution = 0.05.
uniform vec2 GridLineWidth;
uniform vec2 GridResolution;

in vec3 vertexNormal;
in vec3 vertexWorldPos;		// vertex world space position
in vec3 vertexSurfacePos;	// vertex object space coordinate

out vec4 fragColour;

// Determine wether the surface texture coordinate coincides with where a lat/lon line
// should be drawn.
bool OnLatLonLine(vec2 textureCoord)
{
	vec2 distanceToLine = mod(textureCoord, GridResolution);
	vec2 dx = abs(dFdx(textureCoord));
	vec2 dy = abs(dFdy(textureCoord));
	vec2 dF = vec2(max(dx.s, dy.s), max(dx.t, dy.t)) * GridLineWidth;
	return any(lessThan(distanceToLine, dF));
}

// Lighting calculation.
vec3 ComputeLight(vec3 P, vec3 N, vec3 Eye, MaterialStruct material)
{
	vec3 emittedLight = Material.Ke;
	vec3 ambientLight = Material.Ka * AmbientLight;

	// Assume that the sun is always the origin of the world coordinate space...
	vec3 L = normalize(-vertexWorldPos);
	float NdotL = dot(L,N);
	float diffuseAmount = max(NdotL, 0);
	vec3 diffuseLight = material.Kd * diffuseAmount; // sunlight colour is assumed to be == 1.0

	return emittedLight + ambientLight + diffuseLight;
}

void main()
{
	// compute the 2D texture coordinate at this surface point...
	vec3 objectNormal = normalize(vertexSurfacePos);
	vec2 textureCoord = EllipsoidTextureCoord(objectNormal);

	// check to see if this fragment is on a lat/lon line and don't bother
	// with the expensive lighting calculation if it is...
	if (OnLatLonLine(textureCoord))
	{
		fragColour = vec4(1,0,0,1);
	}
	else
	{
		vec3 N = normalize(vertexNormal);
		fragColour.rgb = ComputeLight(vertexWorldPos, N, EyePosition, Material);
		fragColour.a = 1.0f;
	}
}
