//////////////////////////////
// Filename: FontClass.cpp
//////////////////////////////

#include "FontClass.h"

FontClass::FontClass()
{
	m_font = 0;
	m_texture = 0;
}

FontClass::FontClass(const FontClass& other)
{

}

FontClass::~FontClass()
{

}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;

	//load in the text file containing the font data
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	//load the texture that has the font characters on it
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	//release the font texture
	ReleaseTexture();

	//release the font data
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	//Create the font spacing buffer
	m_font = new FontType[95];
	if (!m_font)
	{
		return false;
	}

	//Read in the font size and spacing between chars
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	//Read in the 95 used ascii characters for text
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}

	//close the file
	fin.close();
	
	return true;
}

void FontClass::ReleaseFontData()
{
	//Release the font data array
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	//Create the texture object
	m_texture = new TextureClass;
	if (!m_texture)
	{
		return false;
	}

	//initialize the texture object
	result = m_texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	//release the texture object
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;

	//coerce the input vertices into a vertextype structure
	vertexPtr = (VertexType*)vertices;

	//get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	//initialize the index to th vertex array
	index=0;

	// Draw each letter onto a quad.
	for (i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;
		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;
			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;
			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
			index++;
			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;
			vertexPtr[index].position = D3DXVECTOR3(drawX + m_font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
			index++;
			vertexPtr[index].position = D3DXVECTOR3((drawX + m_font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;
			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_font[letter].size + 1.0f;
		}
	}
	return;
}