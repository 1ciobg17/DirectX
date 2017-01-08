////////////////////////
// Filename: Models.h
////////////////////////
#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include "TextureClass.h"
#include <fstream>
#include "objfilemodel.h"
#include "LightShaderClass.h"
#include "LightClass.h"
using namespace std;

/////////////////////////
// Class name: Models
/////////////////////////
class Models
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	ObjFileModel* m_object;

	float m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, m_bounding_sphere_centre_radius;

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*, ID3D11Device*, ID3D11DeviceContext*);
public:
	Models();
	Models(const Models&);
	~Models();

	bool Initialize(ID3D11Device*, char*, WCHAR*, ID3D11DeviceContext*);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, LightShaderClass*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, LightClass*);

	int GetIndexCount();
	int GetVertexCount();

	ID3D11ShaderResourceView* GetTexture();

	//SceneNode requirered methods
	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();
	float GetBoundingSphereRadius();
	float GetBoundingSphereX();
	float GetBoundingSphereY();
	float GetBoundingSphereZ();
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
};