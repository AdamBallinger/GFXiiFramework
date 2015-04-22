#ifndef boxcollider_h
#define boxcollider_h

#include "WorldStructure.h"
#include "Camera.h"

class BoxCollider
{
private:
	
	WorldStructure* parent;

	glm::vec3 position; // If theres no parent for this collider (E.g collider for the camera)
	glm::vec3 size;

public:

	BoxCollider();
	BoxCollider(WorldStructure&);
	~BoxCollider();

	WorldStructure& GetParent();

	void SetPosition(glm::vec3);
	void SetSize(glm::vec3);

	glm::vec3 GetPosition();
	glm::vec3 GetSize();

	bool CollidesWith(BoxCollider&);

};

#endif