///////////////////////////////////
// Filename: MovementClass.cpp
///////////////////////////////////
#include "MovementClass.h"

MovementClass::MovementClass()
{
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_rotX = 0;
	m_rotY = 0;
	m_rotZ = 0;
	m_frameTime = 0;
	m_forwardSpeed = 0;
	m_backwardSpeed = 0;
	m_upwardSpeed = 0;
	m_downwardSpeed = 0;
	m_leftTurnSpeed = 0;
	m_rightTurnSpeed = 0;
	m_leftTurnSpeed = 0;
	m_rightTurnSpeed = 0;
}

MovementClass::MovementClass(const MovementClass& other)
{

}

MovementClass::~MovementClass()
{

}

void MovementClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void MovementClass::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	return;
}

void MovementClass::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
	return;
}

void MovementClass::GetPosition(float& x, float& y, float& z)
{
	x = m_posX;
	y = m_posY;
	z = m_posZ;
	return;
}

void MovementClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotX;
	y = m_rotY;
	z = m_rotZ;
	return;
}

void MovementClass::SetXPos(float x)
{
	m_posX = x;
	return;
}

void MovementClass::SetYPos(float y)
{
	m_posY = y;
	return;
}

void MovementClass::SetZPos(float z)
{
	m_posZ = z;
	return;
}

void MovementClass::SetXRot(float x)
{
	m_rotX = x;
	return;
}

void MovementClass::SetYRot(float y)
{
	m_rotY = y;
	return;
}

void MovementClass::SetZRot(float z)
{
	m_rotZ = z;
	return;
}

float MovementClass::GetXPos()
{
	return m_posX;
}

float MovementClass::GetYPos()
{
	return m_posY;
}

float MovementClass::GetZPos()
{
	return m_posZ;
}

float MovementClass::GetXRot()
{
	return m_rotX;
}

float MovementClass::GetYRot()
{
	return m_rotY;
}

float MovementClass::GetZRot()
{
	return m_rotZ;
}

void MovementClass::Forward(bool input)
{
	//convert to radians
	float radians=m_rotY*(XM_PI / 180);

	//update the forward speed movement based on the frame time and whether the user is holding the key down or not
	if (input)
	{
		m_forwardSpeed += m_frameTime*0.1f;
		if (m_forwardSpeed > (m_frameTime*0.5f))
		{
			m_forwardSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime*0.1f;
		if (m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	//update the position
	m_posX += sinf(radians)*m_forwardSpeed;
	m_posZ += cosf(radians)*m_forwardSpeed;

	return;
}

void MovementClass::Backward(bool input)
{
	float radians=m_rotY*(XM_PI / 180);

	if (input)
	{
		m_backwardSpeed += m_frameTime*0.1f;
		if (m_backwardSpeed > (m_frameTime*0.5f))
		{
			m_backwardSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime*0.1f;
		if (m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	m_posX -= sinf(radians)*m_backwardSpeed;
	m_posZ -= cosf(radians)*m_backwardSpeed;

	return;
}

void MovementClass::Upward(bool input)
{
	if (input)
	{
		m_upwardSpeed += m_frameTime*0.1f;
		if (m_upwardSpeed > (m_frameTime*0.5f))
		{
			m_upwardSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime*0.1f;
		if (m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	m_posY += m_upwardSpeed;

	return;
}

void MovementClass::Downward(bool input)
{
	if (input)
	{
		m_downwardSpeed += m_frameTime*0.1f;
		if (m_downwardSpeed > (m_frameTime*0.5f))
		{
			m_downwardSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime*0.1f;
		if (m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	m_posY -= m_downwardSpeed;

	return;
}

void MovementClass::TurnLeft(bool input)
{
	if (input)
	{
		m_leftTurnSpeed += m_frameTime*0.1f;
		if (m_leftTurnSpeed > (m_frameTime*0.5f))
		{
			m_leftTurnSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime*0.1f;
		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotY -= m_leftTurnSpeed;

	//keep the rotation in the 0 to 360 range
	if (m_rotY < 0.0f)
	{
		m_rotY += 360.0f;
	}

	return;
}

void MovementClass::TurnRight(bool input)
{
	if (input)
	{
		m_rightTurnSpeed += m_frameTime*0.1f;
		if (m_rightTurnSpeed > (m_frameTime*0.5f))
		{
			m_rightTurnSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime*0.1f;
		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotY += m_rightTurnSpeed;

	//keep rotation in the 0 to 360 range
	if (m_rotY > 360.0f)
	{
		m_rotY -= 360.0f;
	}

	return;
}

void MovementClass::LookUp(bool input)
{
	if (input)
	{
		m_lookUpSpeed += m_frameTime*0.1f;

		if (m_lookUpSpeed > (m_frameTime*0.5f))
		{
			m_lookUpSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime*0.1f;
		if (m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	m_rotX -= m_lookUpSpeed;

	//keep the rotation maximum 90 degrees
	if (m_rotX > 90.0f)
	{
		m_rotX = 90.0f;
	}

	return;
}

void MovementClass::LookDown(bool input)
{
	if (input)
	{
		m_lookDownSpeed += m_frameTime*0.1f;
		if (m_lookDownSpeed > (m_frameTime*0.5f))
		{
			m_lookDownSpeed = m_frameTime*0.5f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime*0.1f;
		if (m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	m_rotX += m_lookDownSpeed;

	if (m_rotX < -90.0f)
	{
		m_rotX = -90.0f;
	}

	return;
}