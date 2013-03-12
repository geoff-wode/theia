//
// Common macros, structures, variables and functions which can be used
// by vertex and fragment shaders.
//
// This file will be first in the compilation chain, so it must have a
// #version line (any #version in the vertex or fragment shader source
// will be disabled).
#version 330

// Useful constants...
#define PI				(3.141592)
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
uniform mat4	World;		// transforms a vector into world space
uniform mat4	View;		// transforms a vector into view space
uniform mat4	Projection;	// transforms a vector into screen space
uniform mat4	WorldView;	// (View * World)
uniform mat4	WorldViewProjection;	// (Projection * View * World)
uniform vec3	AmbientLight;	// RGB value of the ambient light

uniform vec3	EyePosition;	// world-space position of the "eye"

//-----------------------------------------------------------------------------------
// Return a texture coordinate based on the surface normal of an ellipsoid.
vec2 EllipsoidTextureCoord(vec3 normal)
{
	float u = (atan(normal.z, normal.x) * ONE_OVER_2_PI);
	float v = (asin(normal.y) * ONE_OVER_PI);
	return vec2(u,v) + vec2(0.5);
}

//-----------------------------------------------------------------------------------
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
//-----------------------------------------------------------------------------------
