#version 330

// Surface material values...
uniform vec3	Ke;		// emissive material colour
uniform vec3	Ka;		// ambient material reflectance
uniform vec3	Kd;		// diffuse material colour
uniform vec4	Ks;		// specular material colour in rgb, shininess in a

// Variables controlling a lat/lon grid.
// The x components defines the value for longitude, y components define latitude values.
// For resolution, the actual number of lines is given as 1/resolution in [0,1], i.e. for
// 20 lines, set resolution = 0.05.
uniform vec2 GridLineWidth;
uniform vec2 GridResolution;

in vec3 vertexWorldPos;		// vertex's world space position
in vec3 vertexObjectPos;	// vertex's object space coordinate

out vec4 fragColour;

// Compute and return a texture coordinate based on the surface normal
// of an ellipsoid.
#define ONE_OVER_PI		1.0f / PI
#define ONE_OVER_2_PI	1.0f / TWO_PI
vec2 ComputeTextureCoord(vec3 normal)
{
	float u = (atan(normal.z, normal.x) * ONE_OVER_2_PI);
	float v = (asin(normal.y) * ONE_OVER_PI);
	vec2 aHalf = vec2(0.5f);
	return vec2(u,v) + aHalf;
}

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

void main()
{
	vec3 N = normalize(vertexWorldPos);

	vec3 objectNormal = normalize(vertexObjectPos);
	vec2 textureCoord = ComputeTextureCoord(objectNormal);

	// check to see if this fragment is on a lat/lon line and don't bother
	// with the expensive lighting calculation if it is...
	if (OnLatLonLine(textureCoord))
	{
		fragColour = vec4(1,0,0,1);
	}
	else
	{
		vec3 emissive = Ke;
		vec3 ambient = Ka * AmbientLightColour;

		vec3 L = normalize(LightPosition - vertexWorldPos);
		float diffuseAmount = max(dot(N, L), 0);
		vec3 diffuse = Kd * LightColour * diffuseAmount;

		fragColour.rgb = emissive + ambient + diffuse;
		fragColour.a = 1.0f;
	}
}
