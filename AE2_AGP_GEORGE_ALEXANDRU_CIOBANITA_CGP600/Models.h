////////////////////////
// Filename: Models.h
////////////////////////
#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include "TextureClass.h"
#include <fstream>
#include "objfilemodel.h"
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

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;

	ModelType* m_model;
	ObjFileModel* m_object;

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*, ID3D11Device*, ID3D11DeviceContext*);
	void ReleaseModel();
public:
	Models();
	Models(const Models&);
	~Models();

	bool Initialize(ID3D11Device*, char*, WCHAR*, ID3D11DeviceContext*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetVertexCount();

	ID3D11ShaderResourceView* GetTexture();
};