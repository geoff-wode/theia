//
// Common macros, structures, variables and functions which can be used
// by vertex and fragment shaders.
//
// This file will be first in the compilation chain, so it must have a
// #version line (any #version in the vertex or fragment shader source
// will be disabled).
#version 330

// Useful constants...
#define PI				(3.141592f)
#define TWO_PI			(2.0f * PI)
#define HALF_PI			(0.5f * PI)
#define ONE_OVER_PI		(1.0f / PI)
#define ONE_OVER_2_PI	(1.0f / TWO_PI)

// Surface material values...
struct MaterialStruct
{
	vec3	Ke;		// emissive colour
	vec3	Ka;		// ambient reflectance
	vec3	Kd;		// diffuse colour
	vec4	Ks;		// specular colour in rgb, shininess in a
};

// Standard shader parameters:
uniform mat4	World;		// transforms the model into world space
uniform mat4	View;		// transforms a vector into view space
uniform mat4	Projection;	// transforms a vector into screen space
uniform mat4	WorldView;	// pre-computed (View * World)
uniform mat4	WorldViewProjection;	// pre-computed (Projection * View * World)
uniform vec3	EyePosition;	// world-space position of the "eye"
uniform vec3	AmbientLight;	// RGB value of the ambient light

//-----------------------------------------------------------------------------------
// Return a texture coordinate based on the surface normal of an ellipsoid.
vec2 EllipsoidTextureCoord(vec3 normal)
{
	float u = (atan(normal.z, normal.x) * ONE_OVER_2_PI);
	float v = (asin(normal.y) * ONE_OVER_PI);
	return vec2(u,v) + vec2(0.5);
}
