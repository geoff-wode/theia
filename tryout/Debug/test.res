        ��  ��                  �       �� ��g     0 	        #version 330

in VertexOutParams
{
	vec4 Colour;
} VertexOutputs;

out FragmentOutParams
{
	vec4 fragColour;
};

void main()
{
	fragColour = VertexOutputs.Colour;
}
   �      �� ��f     0 	        #version 330

#define VERTEX_POSITION	layout(location = 0)
#define VERTEX_COLOUR		layout(location = 3)

VERTEX_POSITION	in vec3 inPosition;
VERTEX_COLOUR		in vec4 inColour;

uniform mat4	ViewProjection;
uniform mat4	World;
uniform float	Radius;

out VertexOutParams
{
	vec4 Colour;
} VertexOutputs;

void main()
{
	vec4 p = normalize(vec4(inPosition, 1));

	gl_Position = ViewProjection * World * p;

	VertexOutputs.Colour = inColour;
}
  