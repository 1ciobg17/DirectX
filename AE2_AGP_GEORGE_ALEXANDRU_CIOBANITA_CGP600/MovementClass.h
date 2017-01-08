//////////////////////////////
// Filename: MovementClass.h
//////////////////////////////
#pragma once

#include <math.h>
#include <windows.h>
#define _XM_NO_INTRINSIcS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>
#include "SceneNode.h"

class MovementClass
{
private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
	float m_scale;
	float m_frameTime;
	float m_forwardSpeed, m_backwardSpeed, m_upwardSpeed, m_downwardSpeed, m_leftTurnSpeed, m_rightTurnSpeed, m_lookUpSpeed, m_lookDownSpeed;
public:
	MovementClass();
	MovementClass(const MovementClass&);
	~MovementClass();
	void SetFrameTime(float);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);
	void SetXPos(float);
	void SetYPos(float);
	void SetZPos(float);
	void SetXRot(float);
	void SetYRot(float);
	void SetZRot(float);
	void SetScale(float);
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetXRot();
	float GetYRot();
	float GetZRot();
	float GetScale();
	void Forward(bool);
	void NewForward(float distance);
	void Backward(bool);
	void Upward(bool);
	void Downward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUp(bool);
	void LookDown(bool);
};