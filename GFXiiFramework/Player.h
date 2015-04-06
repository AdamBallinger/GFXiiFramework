#ifndef player_h
#define player_h

#include "glm\glm.hpp"

class Player 
{
private:

	glm::vec3 position;

public:

	Player();
	~Player();

	void SetPosition(glm::vec3 _position);
	glm::vec3 GetPosition();

};

#endif