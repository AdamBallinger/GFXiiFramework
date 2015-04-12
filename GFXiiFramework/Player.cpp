#include "Player.h"

Player::Player(LPCWSTR _meshFile)
{
	playermodel = new WorldStructure(_meshFile);
	viewmatrix = glm::mat4(1.0f);
	forwardVec = glm::vec3(0.0f, 0.0f, 1.0f);
}

WorldStructure* Player::GetModel()
{
	return playermodel;
}