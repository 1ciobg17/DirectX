//////////////////////////////
// Filename: MovementClass.h
//////////////////////////////
#pragma once

#include <math.h>
#include <windows.h>
#include <xnamath.h>

class MovementClass
{
private:
	float m_posX, m_posY, m_posZ;
	float m_rotX, m_rotY, m_rotZ;
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
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetXRot();
	float GetYRot();
	float GetZRot();
	void Forward(bool);
	void Backward(bool);
	void Upward(bool);
	void Downward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUp(bool);
	void LookDown(bool);
};