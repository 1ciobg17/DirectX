#pragma once

#include"objfilemodel.h"

class Model
{
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	ObjFileModel* m_pObject;
	ID3D11VertexShader* m_pVShader;
	ID3D11PixelShader* m_pPShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBuffer;

	float m_x, m_y, m_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scale;
public:
	Model(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext);
	~Model();
	int LoadObjModel(char* filename);
	void Draw(XMMATRIX* view, XMMATRIX* projection);
	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);
	void SetXRot(float x);
	void SetYRot(float y);
	void SetZRot(float z);
	void SetScale(float scale);
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetXRot();
	float GetYRot();
	float GetZRot();
	float GetScale();
	void ChangeXPos(float x);
	void ChangeYPos(float y);
	void ChangeZPos(float z);
	void ChangeXRot(float x);
	void ChangeYRot(float y);
	void ChangeZRot(float z);
	void ChangeScale(float scale);
	void LookAt_XZ(float x, float z);
	void MoveForward(float distance);
};