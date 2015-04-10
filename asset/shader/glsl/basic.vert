#version 430

layout (location = 0) uniform mat4 view;	//view matrix
layout (location = 1) uniform mat4 projection;	//projection matrix
layout (location = 2) uniform mat4 lightmatrix;
layout (location = 3) uniform mat4 model;	//model matrix
layout (location = 4) uniform mat4 transformation;	// model transformation matrix
layout (location = 5) uniform mat4 normalmatrix; // normal matrix (inverse transpose model)
layout (location = 6) uniform vec3 campos;

layout (location = 0) in vec3 position;	//vertex attribute: position
layout (location = 1) in vec4 inNormal;	//vertex attribute: normal
layout (location = 2) in vec2 inUV;		//vertex attribute: texcoords

out vec4 outNormal;		//output: normal
out vec4 lightvec;		//output: light vector
out vec4 viewvec;		//output: view vector
out vec2 outUV;			//output: texcoords
out vec4 vposition;		// output: vertex position
out vec4 outPosInLight;	//output: vertex position in light space

void main()
{	
	gl_Position = projection * view * transformation * vec4(position, 1.0f);
	
	vec3 viewDir = normalize(campos - position);
	viewvec = vec4(viewDir, 0.0f);
	viewvec = normalize(viewvec);
	outNormal = normalmatrix * inNormal; // Ensure normals are correct after transformations
	outUV = inUV;
	vposition = transformation * vec4(position, 1.0f);
	outPosInLight = lightmatrix * vec4(position, 1.0f);
}
