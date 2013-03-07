#version 330

uniform Light	Lights[1];
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

const vec3 day = vec3(0.7);
const vec3 night = vec3(0,0,1);
const float blendTransition = 0.15;

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
vec3 ComputeLight(vec3 P, vec3 N, vec3 Eye, MaterialStruct material, Light light)
{
	vec3 emissive = Material.Ke;
	vec3 ambient = Material.Ka * AmbientLight;
	vec3 diffuse = vec3(0);

	if (light.active)
	{
		vec3 L = mix(light.position.xyz, normalize(light.position.xyz - P), light.position.w);
		float NdotL = dot(N,L);
		float diffuseAmount = max(NdotL, 0);
		diffuse = material.Kd * light.colour * diffuseAmount;
	}
	return emissive + ambient + diffuse;
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
		fragColour.rgb = ComputeLight(vertexWorldPos, N, EyePosition, Material, Lights[0]);
		fragColour.a = 1.0f;
	}
}
