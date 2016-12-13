///////////////////////////
// Filename: TextClass.h
//////////////////////////
#pragma once

#include "FontClass.h"
#include "FontShaderClass.h"

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	FontClass* m_font;
	FontShaderClass* m_fontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	//two sentences now
	SentenceType* m_FPSSentence;
	SentenceType* m_CPUSentence;
	SentenceType* m_mouseXPos;
	SentenceType* m_mouseYPos;

	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);
public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

	bool SetFPS(int, ID3D11DeviceContext*);
	bool SetCPU(int, ID3D11DeviceContext*);

	bool SetMousePosition(int, int, ID3D11DeviceContext*);
};