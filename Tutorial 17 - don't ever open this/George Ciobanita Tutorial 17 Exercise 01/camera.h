#include <math.h>
#include <d3d11.h>
#define _XM_NO_INTRINSIcS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>

class Camera
{
private:
	float m_x, m_y, m_z, m_dx,m_dy, m_dz, m_camera_rotation;
	XMVECTOR m_position, m_lookat, m_up;
public:
	Camera(float x, float y, float z, float camera_rotation);
	void Rotate(float degree_rotation);
	void Forward(float distance);
	void Up(float y_height);
	XMMATRIX GetViewMatrix(void);
	float GetX();
	float GetY();
	float GetZ();
	XMVECTOR ReturnLookAtVector();
	void SetPosition(XMVECTOR pos);
};