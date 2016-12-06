#include "SkydomeClass.h"

//class constructor initializes private member pointers to null
SkyDomeClass::SkyDomeClass()
{
	m_model = 0;
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

SkyDomeClass::SkyDomeClass(const SkyDomeClass& other)
{
}

SkyDomeClass::~SkyDomeClass()
{
}

bool SkyDomeClass::Initialize(ID3D11Device* device)
{
	bool result;

	//load in the sky dome model
	result = LoadSkyDomeModel("assets/skybox.txt");
	if (!result)
	{
		return false;
	}

	//load the sky dome into a vertex and index buffer for rendering
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//set the color at the top of the sky dome.
	m_apexColor = XMFLOAT4(0.0f, 0.05f, 0.6f, 1.0f);

	//set the color at the center of the sky dome
	m_centerColor = XMFLOAT4(0.0f, 0.5f, 0.8f, 1.0f);

	return true;
}

void SkyDomeClass::Shutdown()
{
	//Release the vertex and index buffer that were used for rendering the sky dome
	ReleaseBuffers();

	//Release the sky dome model.
	ReleaseSkyDomeModel();

	return;
}

//used to put the sky dome geometry on the graphics pipeline for rendering
void SkyDomeClass::Render(ID3D11DeviceContext* deviceContext)
{
	//render the sky dome
	RenderBuffers(deviceContext);

	return;
}

//index count is used for rendering the sky dome
int SkyDomeClass::GetIndexCount()
{
	return m_indexCount;
}

//return the color of the skydome at the very top
XMFLOAT4 SkyDomeClass::GetApexColor()
{
	return m_apexColor;
}

XMFLOAT4 SkyDomeClass::GetCenterColor()
{
	return m_centerColor;
}

//loads the sky dome model from the file format converted from a .obj file
bool SkyDomeClass::LoadSkyDomeModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	//open the model file.
	fin.open(filename);

	//if it could not open the file then exit
	if (fin.fail())
	{
		return false;
	}

	//read up to the value of the vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//read the vertex count
	fin >> m_vertexCount;

	//set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	//create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	//read up to the beginning of the data
	fin.get(input);
	while (input != '.')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	//read the vertex data
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	//close the model file
	fin.close();

	return true;
}

//releases the sky dome model structure
void SkyDomeClass::ReleaseSkyDomeModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}

//loads the sky dome model structure into the vertex and index buffer
bool SkyDomeClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//load the vertex array and index array with data
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		indices[i] = i;
	}

	//set up the description of the vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//now finally create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//set up the description of the index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release the arrays now that the vertex and the index buffers have been created and loaded;
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void SkyDomeClass::ReleaseBuffers()
{
	//release the index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//Release the vertex buffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void SkyDomeClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//set the vertex buffer to active in the input assembler soo it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}