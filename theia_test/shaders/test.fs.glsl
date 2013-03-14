const float VERY_SMALL = 0.0001;

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
// Compute the result of a Catmull-Rom spline. See pp34-35 of the texturing book.

// GLSL needs to know how big an array is at compile time, so we set an upper limit and
// take the hit of possibly copying more values than are actually needed.
#define SPLINE_MAX_KNOTS  (10)
vec3 spline(float param, int numKnots, vec3 knots[SPLINE_MAX_KNOTS])
{
  int numSpans = numKnots - 3;
  if (numSpans < 1)
  {
    // need at least 4 knot points - oh, for an ASSERT macro... ;-)
    return vec3(0);
  }

  // Find the start of the span...
  param = clamp(param, 0, 1) * numSpans;
  int span = int(param);
  if (span >= numKnots - 3)
  {
    span = numKnots - 3;
  }
  param -= span;

  // Basis matrix coefficients...
  const float CR00 = -0.5;
  const float CR01 = 1.5;
  const float CR02 = -1.5;
  const float CR03 = 0.5;
  const float CR10 = 1.0;
  const float CR11 = -2.5;
  const float CR12 = 2.0;
  const float CR13 = -0.5;
  const float CR20 = -0.5;
  const float CR21 = 0.0;
  const float CR22 = 0.5;
  const float CR23 = 0.0;
  const float CR30 = 0.0;
  const float CR31 = 1.0;
  const float CR32 = 0.0;
  const float CR33 = 0.0;
  // Evaluate the cubic at "param":
  vec3 c3 = (CR00 * knots[span]) + (CR01 * knots[span+1]) + (CR02 * knots[span+2]) + (CR03 * knots[span+3]);
  vec3 c2 = (CR10 * knots[span]) + (CR11 * knots[span+1]) + (CR12 * knots[span+2]) + (CR13 * knots[span+3]);
  vec3 c1 = (CR20 * knots[span]) + (CR21 * knots[span+1]) + (CR22 * knots[span+2]) + (CR23 * knots[span+3]);
  vec3 c0 = (CR30 * knots[span]) + (CR31 * knots[span+1]) + (CR32 * knots[span+2]) + (CR33 * knots[span+3]);

  return ((c3 * param + c2) * param + c1) * param + c0;
}

//-----------------------------------------------------------------------------------
// Return a value fractal Brownian motion value for point P.
// "lacunarity" controls frequency over each octave.
// "gain" controls amplitude over each octave.
float fBm(vec3 P, float octaves, float lacunarity, float gain)
{
  float freq = 1.0;
  float amp = 0.5;
  float sum = 0;
  for(int i = 0; i < int(octaves); i++)
  {
    sum += snoise(P * freq) * amp;
    freq *= lacunarity;
    amp *= gain;
  }	
  octaves -= int(octaves);
  if (octaves > VERY_SMALL)
  {
    sum += octaves * snoise(P) * amp;
  }
  if (sum <= 0)
  {
    sum = -pow(-sum, 0.07);
  }
  else
  {
    sum = pow(sum, 1 + (snoise(P) * sum));
  }	
  return sum;
}

vec3 VectorNoise(vec3 P) { return vec3(snoise(P), snoise(P+3.33), snoise(P+7.77)); }

float DistortNoise(vec3 P, float distortion)
{
  vec3 offset = P + 0.5;
  offset = VectorNoise(offset);
  offset *= distortion;
  P += offset;
  return snoise(P);
}

// Similar to fBm but uses the sum of abs(noise).
float Turbulence(vec3 P, float octaves, float lacunarity, float gain)
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

float GetHeightAt(vec3 P)
{
  const float lacunarity = 2;
  const float octaves = 7;
  const float gain = 0.5;
  float height = fBm(P, octaves, lacunarity, gain);
  return height;
}


//-----------------------------------------------------------------------------------
// Latitude and longitude: see p26 of the virtual globes book.

// Return the latitude in radians of a point P on a unit sphere.
// Latitude ranges [0,+Pi].
float Latitude(vec3 P) { return asin(P.y); }

// Return the longitude in radians of a point P on a unit sphere.
// Longitude ranges [-Pi,+Pi]. Positive is "east", negative is "west".
// Special care may need to be take at the meridians (0 & +/- 180).
float Longitude(vec3 P) { return atan(P.z, P.x); }

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

void main()
{
  // compute the 2D texture coordinate at this surface point...
  vec3 objectNormal = vec3(normalize(vertexSurfacePos));
  vec2 textureCoord = EllipsoidTextureCoord(objectNormal);

  // check to see if this fragment is on a lat/lon line and don't bother
  // with the expensive lighting calculation if it is...
  //if (OnLatLonLine(textureCoord))
  //{
  //	fragColour = vec4(0.05,0.05,0.05,1);
  //}
  //else
  {
    MaterialStruct material = Material;

    const float ice_caps = 0.9;
    const float mottle_limit = 0.75;
    const float mottle_scale = 20;
    const float mottle_dim = 0.25;
    const float mottle_mag = 0.02;
    const float nonlinear = 0;
    const float purt_scale = 0.9;
    const float map_exp = 0;
    const float lat_scale = 0.95;
    const float depth_scale = 1;
    const float depth_max = 0.5;

    float latitude = abs(Latitude(objectNormal));
    float height = GetHeightAt(objectNormal);

    // purturb the colour map lookup...
    latitude += height * (nonlinear * (1 - latitude) + purt_scale);
    latitude *= lat_scale;
    if (map_exp > 0)
    {
      latitude *= pow(latitude, map_exp);
    }

    if (height > 0)
    {
      // select colour based on latitude...
      const vec3[] Colours = vec3[](
          vec3(.5, .39, .2),
          vec3(.5, .39, .2),
          vec3(.5, .39, .2),
          vec3(.2, .3,  0),
          vec3(.085, .2, .04),
          vec3(.065, .22, .04),
          vec3(.5, .42, .28),
          vec3(.6, .5, .23),
          vec3(1,1,1),
          vec3(1,1,1)
        );
      material.Kd = spline(abs(latitude), 10, Colours);

      // mottle the colour so it's not just a horizontal band...
      if (latitude < mottle_limit)
      {
        float purt = fBm(objectNormal, 6, 2, mottle_dim);
        material.Kd += (mottle_mag * purt) * vec3(0.5,0.175,0.5);
      }
    }
    else
    {
      if (latitude > ice_caps)
      {
        material.Kd = vec3(1);
      }
      else
      {
        // make deeper water darker...
        height *= depth_scale;
        height = max(height, -depth_max);
        material.Kd = vec3(0.12,0.3,0.6) * (1 + height);
      }
    }
    
    vec3 N = normalize(vertexSurfaceNormal);
    fragColour.rgb = ComputeLight(vertexWorldPos, N, material);
    fragColour.a = 1.0f;
  }

  //fragColour = vec4(0,1,0,1);
}
