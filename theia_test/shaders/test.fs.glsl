#version 330

// Surface material values...
struct MaterialStruct
{
	vec3	Ke;		// emissive colour
	vec3	Ka;		// ambient reflectance
	vec3	Kd;		// diffuse colour
	vec4	Ks;		// specular colour in rgb, shininess in a
};

uniform vec3	LightPos;
uniform vec3	EyePos;
uniform vec3	LightColour;
uniform vec3	AmbientLightColour;
uniform MaterialStruct Material;

// Variables controlling a lat/lon grid.
// The x components defines the value for longitude, y components define latitude values.
// For resolution, the actual number of lines is given as 1/resolution in [0,1], i.e. for
// 20 lines, set resolution = 0.05.
uniform vec2 GridLineWidth;
uniform vec2 GridResolution;

in vec3 vertexNormal;
in vec3 vertexWorldPos;		// vertex world space position
in vec3 vertexSurfacePos;	// vertex object space coordinate

out vec4 fragColour;

#define ONE_OVER_PI		1.0f / PI
#define ONE_OVER_2_PI	1.0f / TWO_PI
const vec2 aHalf = vec2(0.5f);

const vec3 day = vec3(0.7);
const vec3 night = vec3(0,0,1);
const float blendTransition = 0.15;

// Compute and return a texture coordinate based on the surface normal
// of an ellipsoid.
vec2 ComputeTextureCoord(vec3 normal)
{
	float u = (atan(normal.z, normal.x) * ONE_OVER_2_PI);
	float v = (asin(normal.y) * ONE_OVER_PI);
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

// Lighting calculation.
vec3 Light(vec3 P, vec3 N, vec3 Eye, MaterialStruct material)
{
	vec3 emissive = Material.Ke;
	vec3 ambient = Material.Ka * AmbientLightColour;

	vec3 L = normalize(LightPos - P);
	float NdotL = dot(N,L);
	float diffuseAmount = max(NdotL, 0);

	vec3 colour;
	
	if (NdotL > blendTransition)
	{
		//vec3 diffuse = material.Kd * LightColour * diffuseAmount;
		//colour = emissive + ambient + diffuse;
		colour = day;
	}
	else if (NdotL < -blendTransition)
	{
		colour = night;
	}
	else
	{
		colour = mix(night, day, (NdotL + blendTransition) * 4);
	}

	return colour;
}

void main()
{
	// compute the 2D texture coordinate at this surface point...
	vec3 objectNormal = normalize(vertexSurfacePos);
	vec2 textureCoord = ComputeTextureCoord(objectNormal);

	// check to see if this fragment is on a lat/lon line and don't bother
	// with the expensive lighting calculation if it is...
	if (OnLatLonLine(textureCoord))
	{
		fragColour = vec4(1,0,0,1);
	}
	else
	{
		vec3 N = normalize(vertexNormal);
		fragColour.rgb = Light(vertexWorldPos, N, EyePos, Material);
		fragColour.a = 1.0f;
	}
}
