#include "RacingStructure.h"

RacingStructure::RacingStructure(LPCWSTR _meshFile) : RacingStructure::WorldStructure(_meshFile)
{
	collider = new BoxCollider(*this);
	collider->SetSize(glm::vec3(100.0f, 100.0f, 50.0f));
}

RacingStructure::~RacingStructure()
{
	delete collider;
}

BoxCollider& RacingStructure::GetCollider()
{
	return *collider;
}