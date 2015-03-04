#version 430

uniform sampler2D texColour;

in vec4 outColour;
in vec2 outUV;
out vec4 outFrag;

void main()
{
	//outFrag = outColour;
	outFrag = texture(texColour, outUV);
}
