#version 330

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
