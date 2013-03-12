
uniform MaterialStruct Material;

// Variables controlling a lat/lon grid.
// The x components define the values for longitude, y components define latitude values.
// For resolution, the actual number of lines is given as 1/resolution in [0,1], i.e. for
// 20 lines, set resolution = 0.05.
uniform vec2 GridLineWidth;
uniform vec2 GridResolution;

in vec3 vertexSurfaceNormal;
in vec3 vertexWorldPos;		// vertex world space position
in vec3 vertexSurfacePos;	// vertex object space coordinate

out vec4 fragColour;

//-----------------------------------------------------------------------------------
// Return a value fractal Brownian motion value for point P.
// "lacunarity" controls frequency over each octave.
// "gain" controls amplitude over each octave.
float fBm(vec2 P, float octaves, float lacunarity, float gain)
{
  float frequency = 1;
  float amplitude = 0.5;
  float sum = 0;
  for (int i = 0; i < octaves; i++)
  {
    sum += snoise(P * frequency) * amplitude;
    frequency *= lacunarity;
    amplitude *= gain;
  }
  return sum;
}

// Similar to fBm but uses the sum of abs(noise).
float Turbulence(vec2 P, int octaves, float lacunarity, float gain)
{
  float frequency = 1;
  float amplitude = 0.5;
  float sum = 0;
  for (int i = 0; i < octaves; i++)
  {
    sum += abs(snoise(P * frequency)) * amplitude;
    frequency *= lacunarity;
    amplitude *= gain;
  }
  return sum;
}

float LengthSquared(vec2 v) { return (v.x * v.x) + (v.y * v.y); }
float LengthSquared(vec3 v) { return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }

vec2 VectorNoise(vec2 P) { return vec2(snoise(P), snoise(P + 3.33)); }

float DistNoise(vec2 P, float distortion) { return snoise(P + (distortion * VectorNoise(P + 0.5))); }

#define MINFILTWIDTH 1.0e-6
#define MINFILTWIDTH_SQUARED  (MINFILTWIDTH * MINFILTWIDTH)
#define FILTERWIDTH(x)  max( LengthSquared(abs(dFdx(x)) + abs(dFdy(x))), MINFILTWIDTH_SQUARED)

bool IsLand(vec2 P)
{
  const float lacunarity = 2.1759;
  const float octaves = 7;
  const float gain = 0.5;
  const float seaLevel = 0;
  const float mountainScale = 1;
  const float bumpScale = 0.07;
  float bumpy = fBm(P, octaves, lacunarity, gain);
  float chaos = bumpy * bumpScale;
  return (chaos > seaLevel);
}

//-----------------------------------------------------------------------------------

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
vec3 ComputeLight(vec3 P, vec3 N, MaterialStruct material)
{
	vec3 emittedLight = Material.Ke;
	vec3 ambientLight = Material.Ka * AmbientLight;

	// Assume that the sun is always at the origin...
	vec3 L = normalize(-vertexWorldPos);
	float NdotL = dot(L,N);
	float diffuseAmount = max(NdotL, 0);
	vec3 diffuseLight = material.Kd * diffuseAmount; // sunlight colour is assumed to be == 1.0

	return emittedLight + ambientLight + diffuseLight;
}

const vec3 land = vec3(0,1,0);
const vec3 sea = vec3(0,0,1);
void main()
{
	// compute the 2D texture coordinate at this surface point...
	vec3 objectNormal = vec3(normalize(vertexSurfacePos));
	vec2 textureCoord = EllipsoidTextureCoord(objectNormal);

	// check to see if this fragment is on a lat/lon line and don't bother
	// with the expensive lighting calculation if it is...
	//if (OnLatLonLine(textureCoord))
	//{
	//	fragColour = vec4(1,0,0,1);
	//}
	//else
	{
		MaterialStruct material = Material;

		//material.Kd = vec3(Turbulence(textureCoord, 16, 5.1753, 0.49357));
    material.Kd = mix(sea, land, IsLand(textureCoord));

		vec3 N = normalize(vertexSurfaceNormal);
		fragColour.rgb = ComputeLight(vertexWorldPos, N, material);
		fragColour.a = 1.0f;
	}

	//fragColour = vec4(0,1,0,1);
}
