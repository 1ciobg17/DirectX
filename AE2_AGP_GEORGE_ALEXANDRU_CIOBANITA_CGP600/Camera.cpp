/////////////////////////
// Filename: Camera.cpp
/////////////////////////
#include "Camera.h"

Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

Camera::Camera(const Camera& other)
{

}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	//Setup the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//setup the position of the camera in the world
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	//setup where the camera is looing by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//Set the yaw, pitch and roll rotations in radians
	pitch = m_rotationX*(XM_PI / 180);
	yaw = m_rotationY*(XM_PI / 180);
	roll = m_rotationZ*(XM_PI / 180);

	//Create the rottion matrix from the yaw pitch and roll values
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	//Finally create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

XMMATRIX Camera::GetViewMatrixXMMATRIX()
{
	XMVECTOR position, lookat, up;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	position = XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0);
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	lookat = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	pitch = m_rotationX*(XM_PI / 180);
	yaw = m_rotationY*(XM_PI / 180);
	roll = m_rotationZ*(XM_PI / 180);

	rotationMatrix=XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
	lookat=XMVector3TransformCoord(lookat, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	lookat = position + lookat;

	XMMATRIX view = XMMatrixLookAtLH(position, lookat, up);

	return view;
}