#include "Camera.h"

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

	totalXRotation = 0.0f;
	totalYRotation = 0.0f;
	totalZRotation = 0.0f;
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

	glm::fquat pitchQuat = glm::fquat(glm::cos(_pitch / 2.0f), m_rightVector * glm::sin(_pitch / 2.0f));
	glm::fquat yawQuat = glm::fquat(glm::cos(_yaw / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f) * glm::sin(_yaw / 2.0f));
	glm::fquat rollQuat = glm::fquat(glm::cos(_roll / 2.0f), m_direction * glm::sin(_roll / 2.0f));

	glm::fquat rotQuat = yawQuat * pitchQuat * rollQuat;

	m_direction = rotQuat * m_direction * glm::conjugate(rotQuat);
	m_upVector = rotQuat * m_upVector * glm::conjugate(rotQuat);
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