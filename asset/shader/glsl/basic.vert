#version 430

uniform mat4 modelview, projection;

in vec4 position;
in vec4 inNormal;
in vec2 inUV;

out vec4 outNormal;
out vec2 outUV;

void main()
{	
	gl_Position = projection*modelview*position;

	outNormal = inNormal;

	outUV = inUV;
}
