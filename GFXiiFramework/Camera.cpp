#include "Camera.h"
#include "quaternion.h"

Camera::Camera()
{
	InitCamera();
}

Camera::~Camera()
{
}

void Camera::InitCamera()
{
	//Initialise the camera that sits at the origin looking down the negative z-axis.
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_direction = glm::vec3(0.0f, 0.0, -1.0f);
	m_rightVector = glm::cross(m_direction, m_upVector);
	m_upVector = glm::cross(m_rightVector, m_direction);
	UpdateViewMatrix();
}

void Camera::SetCameraPosition(const glm::vec3* position)
{
	m_position = (*position);
	UpdateViewMatrix();
}

void Camera::SetLookAtPoint(const glm::vec3* lookAt)
{
	m_direction = (*lookAt) - m_position;
	m_direction = glm::normalize(m_direction);

	//Recompute right vector and reinforce orthonormality of this three vectors.
	//Recompute up vector, from new direction and right vector;
	//TODO: Gram-Schmidt orthogonalisation algorithm can be used for this

	m_rightVector = glm::cross(m_direction, m_upVector);
	m_upVector = glm::cross(m_rightVector, m_direction);

	m_rightVector = glm::normalize(m_rightVector);
	m_upVector = glm::normalize(m_upVector);

	UpdateViewMatrix();
}

void Camera::SetUpVector(const glm::vec3* up)
{
	m_upVector = (*up);

	m_rightVector = glm::cross(m_direction, m_upVector);
	m_upVector = glm::cross(m_rightVector, m_direction);

	m_rightVector = glm::normalize(m_rightVector);
	m_upVector = glm::normalize(m_upVector);

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	//float mat[16];

	//mat[0] = m_rightVector[0];
	//mat[4] = m_rightVector[1];
	//mat[8] = m_rightVector[2];

	//mat[1] = m_upVector[0];
	//mat[5] = m_upVector[1];
	//mat[9] = m_upVector[2];

	//mat[2] = -m_direction[0];
	//mat[6] = -m_direction[1];
	//mat[10] = -m_direction[2];

	//mat[3] = mat[7] = mat[11] = 0.0f;

	//mat[12] = -m_position[0];
	//mat[13] = -m_position[1];
	//mat[14] = -m_position[2];
	//mat[15] = 1.0f;

	////m_viewMatrix.SetMatrix(&mat[0]);
	//m_viewMatrix = glm::make_mat4(&mat[0]);
	m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_upVector);
}

void Camera::SetProjectionMatrix(float* projMat)
{
	m_projectionMatrix = glm::make_mat4(&projMat[0]);
}

void Camera::DollyCamera(float _amount)
{
	m_position += m_direction * _amount;
	SetCameraPosition(&m_position);
}

void Camera::StrafeCamera(float _amount)
{
	m_position += m_rightVector * _amount;
	SetCameraPosition(&m_position);
}

void Camera::PedCamera(float _amount)
{
	m_position += m_upVector * _amount;
	SetCameraPosition(&m_position);
}

void Camera::RotateCamera(float _yaw, float _pitch, float _roll)
{
	totalXRotation += _yaw;
	totalYRotation += _pitch;
	totalZRotation += _roll;

	if (totalXRotation > 360.0f) totalXRotation = 0.0f;
	if (totalXRotation < 0.0f) totalXRotation = 360.0f;
	if (totalYRotation > 360.0f) totalYRotation = 0.0f;
	if (totalYRotation < 0.0f) totalYRotation = 360.0f;
	if (totalZRotation > 360.0f) totalZRotation = 0.0f;
	if (totalZRotation < 0.0f) totalZRotation = 360.0f;

	_yaw = glm::radians(_yaw);
	_pitch = glm::radians(_pitch);
	_roll = glm::radians(_roll);

	Vector3 UP = Vector3();
	UP[0] = m_upVector[0];
	UP[1] = m_upVector[1];
	UP[2] = m_upVector[2];

	Vector3 RIGHT = Vector3();
	RIGHT[0] = m_rightVector[0];
	RIGHT[1] = m_rightVector[1];
	RIGHT[2] = m_rightVector[2];

	Vector3 DIR = Vector3();
	DIR[0] = m_direction[0];
	DIR[1] = m_direction[1];
	DIR[2] = m_direction[2];

	Quaternion yawQuat;
	yawQuat.SetQuaternion(UP * glm::sin(_yaw / 2), glm::cos(_yaw / 2));

	Quaternion pitchQuat;
	pitchQuat.SetQuaternion(RIGHT * glm::sin(_pitch / 2), glm::cos(_pitch / 2));

	Quaternion rollQuat;
	rollQuat.SetQuaternion(DIR * glm::sin(_roll / 2), glm::cos(_roll / 2));

	Quaternion upQuat;
	upQuat.SetQuaternion(UP, 0);

	Quaternion dirQuat;
	dirQuat.SetQuaternion(DIR, 0);

	Quaternion yawQuatConj;
	yawQuatConj.SetQuaternion(yawQuat.GetVector() * -1, yawQuat.GetScalar());

	Quaternion pitchQuatConj;
	pitchQuatConj.SetQuaternion(pitchQuat.GetVector() * -1, pitchQuat.GetScalar());

	Quaternion rollQuatConj;
	rollQuatConj.SetQuaternion(rollQuat.GetVector() * -1, rollQuat.GetScalar());

	dirQuat = (pitchQuat * dirQuat * pitchQuatConj) + (yawQuat * dirQuat * yawQuatConj);
	upQuat = (rollQuat * upQuat * rollQuatConj);

	UP = upQuat.GetVector();
	DIR = dirQuat.GetVector();

	m_direction[0] = DIR[0];
	m_direction[1] = DIR[1];
	m_direction[2] = DIR[2];

	m_upVector[0] = UP[0];
	m_upVector[1] = UP[1];
	m_upVector[2] = UP[2];

	m_direction = glm::normalize(m_direction);
	m_upVector = glm::normalize(m_upVector);

	m_rightVector = glm::cross(m_direction, m_upVector);

	UpdateViewMatrix();
}

void Camera::ZoomCamera(float _amount)
{
	m_fov += _amount;

	if (m_fov > 90) m_fov = 90;
	if (m_fov < 10) m_fov = 10;
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 5000.0f);
}