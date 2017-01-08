///////////////////
//Filename:TextClass.cpp
//////////////////////////
#include "TextClass.h"

TextClass::TextClass()
{
	m_font = 0;
	m_fontShader = 0;
	m_FPSSentence = 0;
	m_CPUSentence = 0;
	m_mouseXPos = 0;
	m_mouseYPos = 0;
	m_collisionCheck = 0;
}
TextClass::TextClass(const TextClass& other)
{
}
TextClass::~TextClass()
{
}
bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	//store the screen width and height
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	//store the base view matrix
	m_baseViewMatrix = baseViewMatrix;

	//create the font object
	m_font = new FontClass;
	if (!m_font)
	{
		return false;
	}

	//initialize the font object
	result = m_font->Initialize(device, "assets/fontdata.txt", L"assets/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object", L"Error", MB_OK);
		return false;
	}

	//create the shader object
	m_fontShader = new FontShaderClass;
	if (!m_fontShader)
	{
		return false;
	}

	//initialize the font shader object
	result = m_fontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_FPSSentence, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_CPUSentence, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_mouseXPos, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the first sentence.
	result = InitializeSentence(&m_mouseYPos, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&m_collisionCheck, 16, device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	//release the first sentence
	ReleaseSentence(&m_FPSSentence);
	
	//release the second sentence
	ReleaseSentence(&m_CPUSentence);

	//release the mouse X sentence
	ReleaseSentence(&m_mouseXPos);

	//release the mouse Y sentence
	ReleaseSentence(&m_mouseYPos);
	
	//release the collision check sentence

	//release the font shader object
	if (m_fontShader)
	{
		m_fontShader->Shutdown();
		delete m_fontShader;
		m_fontShader = 0;
	}

	//release the font object
	if (m_font)
	{
		m_font->Shutdown();
		delete m_font;
		m_font = 0;
	}

	return;
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	//draw the first sentence
	result = RenderSentence(deviceContext, m_FPSSentence, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	//draw the second sentence 
	result = RenderSentence(deviceContext, m_CPUSentence, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	//draw the second sentence 
	result = RenderSentence(deviceContext, m_mouseXPos, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	//draw the second sentence 
	result = RenderSentence(deviceContext, m_mouseYPos, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(deviceContext, m_collisionCheck, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}
	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;
	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;
	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;
	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;
	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}
	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));
	// Initialize the index array.
	for (i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	//store the color of the sentence
	sentence->red = red; 
	sentence->green = green;
	sentence->blue = blue;

	//get the number of letters in the sentence
	numLetters = (int)strlen(text);

	//check for possible buffer overflow
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	//create the vertex array
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	//initialize vertex array to zeros at first
	memset(vertices, 0, (sizeof(VertexType)*sentence->vertexCount));

	//calculate the x and y pixel position on the screen to start drawing to
	drawX = (float)(((m_screenWidth / 2)*-1) + positionX); 
	drawY = (float)(((m_screenHeight / 2)) - positionY);

	//use the font class to build the vertex array from the sentence text and sentence draw location
	m_font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	//lock the vertex buffer so it can be written to
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	//copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));
	if (FAILED(result))
	{
		return false;
	}

	//unlock the vertex buffer
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	//release the vertex array
	delete[] vertices;
	vertices = 0;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}
	return;
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix,
	D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.
	result = m_fontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_font->GetTexture(),
		pixelColor);
	if (!result)
	{
		false;
	}
	return true;
}

bool TextClass::SetFPS(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;

	//truncate the fps to below 10000

	if (fps > 9999)
	{
		fps = 9999;
	}

	//convert the fps integer to string format
	_itoa_s(fps, tempString, 10);

	//setup the fps string
	strcpy_s(fpsString, "FPS:");
	strcat_s(fpsString, tempString);

	//if the fps is 60 or above set the fps color to green
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	//if the fps is bellow 60 set the fps color to blue
	if (fps < 60)
	{
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f;
	}

	//if the fps is bellow 30 set the fps color to red
	if (fps < 60)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	//Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_FPSSentence, fpsString, 20, 20, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCPU(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	//convert the fps integer to string format
	_itoa_s(cpu, tempString, 10);

	//setup the fps string
	strcpy_s(cpuString, "CPU:");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	//Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_CPUSentence, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;

	//conver the mouseX integer to string format
	_itoa_s(mouseX, tempString, 10);

	//setup the mouseX string
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	//update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_mouseXPos, mouseString, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	//convert he mouseY integer to string format
	_itoa_s(mouseY, tempString, 10);

	//setup the mouseY string
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	//update the sentence vertex buffer
	result = UpdateSentence(m_mouseYPos, mouseString, 20, 80, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetUpCollision(string collision, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char collString[16];
	char buffer[16];
	bool result;

	sprintf_s(buffer, "%.4s", collision.c_str());

	strcpy_s(tempString, buffer);

	//setup the coll string
	strcpy_s(collString, "Collision: ");
	strcat_s(collString, tempString);

	result = UpdateSentence(m_collisionCheck, collString, 20, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}