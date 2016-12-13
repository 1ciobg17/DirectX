//////////////////////////
// Filename: FontClass.h
///////////////////////

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;
#include "TextureClass.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	FontType* m_font;
	TextureClass* m_texture;

	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);
};