#ifndef racingstructure_h
#define racingstructure_h

#include "WorldStructure.h"
#include "BoxCollider.h"

class RacingStructure : public WorldStructure
{
private:

	BoxCollider* collider;

public:

	RacingStructure(LPCWSTR);
	~RacingStructure();

	BoxCollider& GetCollider();

};

#endif