#include "Player.h"

Player::Player(Camera* _camera)
{
	camera = _camera;
	collider = new BoxCollider();
	collider->SetSize(glm::vec3(10.0f, 10.0f, 10.0f));
	collider->SetPosition(*camera->GetCameraPosition());
	AddSpeed(1.0f);
	SetCurrentRing(0);
}

Camera& Player::GetCamera()
{
	return *camera;
}

BoxCollider& Player::GetCollider()
{
	// When accessing the collider quickly update the position to the latest camera position (Kinda cheaty but whatever)
	collider->SetPosition(*camera->GetCameraPosition());
	return *collider;
}

void Player::AddSpeed(float _speed)
{
	currentSpeed += _speed;
	if (currentSpeed > 7.5f)
	{
		currentSpeed = 7.5f;
	}
	if (currentSpeed < 0.5f)
	{
		currentSpeed = 0.5f;
	}
}

float Player::GetSpeed()
{
	return currentSpeed;
}

void Player::SetCurrentRing(int _currentRing)
{
	currentRing = _currentRing;
}

int Player::GetCurrentRing()
{
	return currentRing;
}