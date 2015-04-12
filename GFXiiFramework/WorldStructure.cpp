#include "WorldStructure.h"

WorldStructure::WorldStructure(LPCWSTR _meshFile)
{
	mesh = new OGLMesh(_meshFile);
	LoadTransformationIdentity();
}

WorldStructure::~WorldStructure()
{
	delete mesh;
}

void WorldStructure::Render()
{
	mesh->Render();
}
