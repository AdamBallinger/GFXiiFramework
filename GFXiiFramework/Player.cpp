#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::SetPosition(glm::vec3 _position)
{
	position = _position;
}

glm::vec3 Player::GetPosition()
{
	return position;
}