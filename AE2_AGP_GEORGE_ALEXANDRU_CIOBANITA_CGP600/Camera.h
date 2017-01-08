//////////////////////////
//Filename: Camera.h
///////////////////////////
#pragma once

#include <D3DX10math.h>
#define _XM_NO_INTRINSIcS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>

//////////////////////
//Class name: Camera
////////////////////////
class Camera
{
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	
	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	XMMATRIX GetViewMatrixXMMATRIX();
};