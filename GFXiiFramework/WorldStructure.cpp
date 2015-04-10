#include "WorldStructure.h"

WorldStructure::WorldStructure(LPCWSTR _meshFile)
{
	mesh = new OGLMesh(_meshFile);
	transformationMatrix = glm::mat4(1.0f);
}

WorldStructure::~WorldStructure()
{
	delete mesh;
}

void WorldStructure::Render()
{
	mesh->Render();
}
