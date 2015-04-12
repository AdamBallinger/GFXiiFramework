#ifndef player_h
#define player_h

#include "WorldStructure.h"

class Player 
{
private:

	WorldStructure* playermodel;

	glm::mat4 viewmatrix;
	glm::vec3 forwardVec;

public:

	Player(LPCWSTR);

	WorldStructure* GetModel();

};

#endif