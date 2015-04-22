#ifndef player_h
#define player_h

#include "WorldStructure.h"
#include "BoxCollider.h"

class Player 
{
private:

	Camera* camera;
	BoxCollider* collider;

	float currentSpeed;

	int currentRing;

public:

	Player(Camera*);

	Camera& GetCamera();

	BoxCollider& GetCollider();

	void AddSpeed(float);
	float GetSpeed();

	void SetCurrentRing(int);
	int GetCurrentRing();
};

#endif