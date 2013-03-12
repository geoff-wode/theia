
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

#define BRICK_WIDTH			(0.015)
#define BRICK_HEIGHT		(0.01)
#define MORTAR_THICKNESS	(0.001)
#define TOTAL_WIDTH			(BRICK_WIDTH + MORTAR_THICKNESS)
#define TOTAL_HEIGHT		(BRICK_HEIGHT + MORTAR_THICKNESS)
#define MORTAR_WIDTH		(MORTAR_THICKNESS * 0.5 / TOTAL_WIDTH)
#define MORTAR_HEIGHT		(MORTAR_THICKNESS * 0.5 / TOTAL_HEIGHT)

const vec3 brickColour = vec3(0.5, 0.15, 0.14);
const vec3 mortarColour = vec3(0.5);

vec3 BrickColour(vec2 surfaceCoord)
{
	// Get values that vary from 0..1 across a single brick based on the surface coordinate...
	vec2 inBrick = vec2(surfaceCoord.s / TOTAL_WIDTH, surfaceCoord.t / TOTAL_HEIGHT);

	// Create a pattern shift of half a brick on alternate rows...
	if (mod(inBrick.t * 0.5, 1) > 0.5)
	{
		inBrick.s += 0.5;
	}

	vec2 brick = floor(inBrick);	// which brick?
	inBrick -= brick;				// fraction within the brick

	// The brick texture is made of a horizontal (w) and vertical (h) pulse.
	// The pulses are zero within the "mortar" surrounding the brick and rise to 1 inside it.
	float w = step(MORTAR_WIDTH, inBrick.s) - step(1-MORTAR_WIDTH, inBrick.s);
	float h = step(MORTAR_HEIGHT, inBrick.t) - step(1-MORTAR_HEIGHT, inBrick.t);

	// Take the logical AND of the vertical and horizontal pulses (by multiplying).
	// Where the combined pulse is inside the brick area, (w*h) == 1, so the brick colour
	// colour is chosen, everywhere else (w * h) == 0 so the mortar colour is selected.
	return mix(mortarColour, brickColour, w * h);
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
	//	fragColour = vec4(1,0,0,1);
	//}
	//else
	{
		MaterialStruct brick = Material;
		brick.Kd = BrickColour(textureCoord);

		vec3 N = normalize(vertexSurfaceNormal);
		fragColour.rgb = ComputeLight(vertexWorldPos, N, brick);
		fragColour.a = 1.0f;
	}

	//fragColour = vec4(0,1,0,1);
}
