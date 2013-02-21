#version 330

// Global lighting values...
uniform vec3	LightPosition;
uniform vec3	LightColour;
uniform vec3	AmbientLightColour;

// Surface material values...
uniform vec3	Ke;		// emissive material colour
uniform vec3	Ka;		// ambient material reflectance
uniform vec3	Kd;		// diffuse material colour

in vec3 vertexPosition;	// object space position
in vec3 vertexNormal;		// object space normal

out vec4 fragColour;

void main()
{
	vec3 emissive = Ke;
	vec3 ambient = Ka * AmbientLightColour;

	vec3 toLight = normalize(LightPosition - vertexPosition);
	float diffuseAmount = max( dot(vertexNormal, toLight), 0);
	vec3 diffuse = Kd * LightColour * diffuseAmount;

	vec3 materialColour = emissive + ambient + diffuse;
	fragColour.rgb = materialColour;
	fragColour.a = 1.0f;
}
