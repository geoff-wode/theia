#version 330

#define VERTEX_POSITION	layout(location = 0)
#define VERTEX_COLOUR		layout(location = 1)

VERTEX_POSITION	in vec3 inPosition;
VERTEX_COLOUR		in vec4 inColour;

uniform mat4	Projection;
uniform mat4	View;
uniform mat4	World;

out VertexOutParams
{
	vec4 Colour;
} VertexOutputs;

void main()
{
	gl_Position = Projection * View * World * vec4(inPosition, 1);

	VertexOutputs.Colour = inColour;
}
