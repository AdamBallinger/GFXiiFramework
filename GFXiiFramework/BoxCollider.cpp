#include "BoxCollider.h"
#include <iostream>

BoxCollider::BoxCollider()
{
	SetSize(glm::vec3(0.0f, 0.0f, 0.0f));
}

BoxCollider::BoxCollider(WorldStructure& _parent)
{
	parent = &_parent;
}

BoxCollider::~BoxCollider()
{
	delete parent;
}

WorldStructure& BoxCollider::GetParent()
{
	return *parent;
}

void BoxCollider::SetPosition(glm::vec3 _position)
{
	position = _position;
}

void BoxCollider::SetSize(glm::vec3 _size)
{
	size = _size;
}

glm::vec3 BoxCollider::GetPosition()
{
	return position;
}

glm::vec3 BoxCollider::GetSize()
{
	return size;
}

bool BoxCollider::CollidesWith(BoxCollider& _other)
{
	if (parent != nullptr)
	{
		position = parent->ExtractPosition();
	}
		
	glm::vec3 positionOther;

	if (&_other.GetParent() != nullptr)
	{
		positionOther = _other.GetParent().ExtractPosition();
	}
	else
	{
		positionOther = _other.GetPosition();
	}

	if (position[0] - size[0] / 2 < positionOther[0] + _other.GetSize()[0] / 2 &&
		position[0] + size[0] / 2 > positionOther[0] - _other.GetSize()[0] / 2 &&
		position[1] - size[1] / 2 < positionOther[1] + _other.GetSize()[1] / 2 &&
		position[1] + size[1] / 2 > positionOther[1] - _other.GetSize()[1] / 2 &&
		position[2] - size[2] / 2 < positionOther[2] + _other.GetSize()[2] / 2 &&
		position[2] + size[2] / 2 > positionOther[2] - _other.GetSize()[2] / 2)
	{
		return TRUE;
	}
	return FALSE;
}