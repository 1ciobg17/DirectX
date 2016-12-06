///////////////////////////
// Filename: TextureClass.h
//////////////////////////

#pragma once

#include <D3D11.h>
#include <D3DX11tex.h>

//////////////////////////////////////
// Class name: TextureClass
/////////////////////////////////////
class TextureClass
{
private:
	ID3D11ShaderResourceView* m_texture;
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
};