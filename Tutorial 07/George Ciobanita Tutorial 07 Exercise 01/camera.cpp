#include "camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_camera_rotation = camera_rotation;

	m_dx = sin(m_camera_rotation*(XM_PI / 180));//second part after * is to convert to radians
	m_dz = cos(m_camera_rotation*(XM_PI / 180));
}

void Camera::Rotate(float degree_rotation)
{
	m_camera_rotation += degree_rotation;

	m_dx = sin(m_camera_rotation*(XM_PI / 180));//second part after * is to convert to radians
	m_dz = cos(m_camera_rotation*(XM_PI / 180));
}

void Camera::Forward(float distance)
{
	m_x += distance*m_dx;
	m_z += distance*m_dz;
}

void Camera::Up(float y_height)
{
	m_up = XMVectorSet(0.0f, y_height, 0.0f, 0.0f);
}

XMMATRIX Camera::GetViewMatrix()
{
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(m_position, m_lookat, m_up);

	return view;
}