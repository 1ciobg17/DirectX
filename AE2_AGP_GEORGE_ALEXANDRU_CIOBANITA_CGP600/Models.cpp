//////////////////
//Filename: Models.cpp
//////////////////////

#include "Models.h"

Models::Models()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Models::Models(const Models& other)
{

}

Models::~Models()
{

}

void Models::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
	return;
}

void Models::SetRotation(float x, float y, float z)
{
	m_rotation = D3DXVECTOR3(x, y, z);
	return;
}

bool Models::Initialize(ID3D11Device* device,char* filename, WCHAR* textureFilename, ID3D11DeviceContext* deviceContext)
{
	bool result;

	//load the model data
	result = LoadModel(filename, device, deviceContext);
	if (!result)
	{
		return false;
	}

	//Initialize the vertex and index buffer that hold the geometry for the model
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//Load the texture for this model
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Models::Shutdown()
{
	//Release the model texture
	ReleaseTexture();

	//Release the vertex and index buffers
	ShutdownBuffers();

	return;
}

bool Models::Render(ID3D11DeviceContext* deviceContext, LightShaderClass* lightShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, LightClass* light)
{
	HRESULT result;
	D3DXMATRIX endMatrix, rotXMatrix, rotYMatrix, rotZMatrix, endRotation;

	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	//Render the model using the light shader
	result = lightShader->Render(deviceContext,GetVertexCount(), worldMatrix, viewMatrix, projectionMatrix, GetTexture(), light->GetDirection(),light->GetAmbientColor(),light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	return true;
}

int Models::GetIndexCount()
{
	return m_indexCount;
}

int Models::GetVertexCount()
{
	return m_vertexCount;
}

ID3D11ShaderResourceView* Models::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Models::InitializeBuffers(ID3D11Device* device)
{
	//VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//Create the vertex array
	/*vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}*/

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	//load the vertex array and index array with data
	for (i = 0; i < m_indexCount; i++)
	{
		//vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		//vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		//vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	//Set up the description of the static vertex buffer
	/*vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;*/

	//Give the subresource structure a pointer to the vertex data
	/*vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;*/

	//Now create the vertex Buffer
	/*result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}*/

	//set up the descrption of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release the arrays 
	/*delete[] vertices;
	vertices = 0;*/

	delete[] indices;
	indices = 0;

	return true;
}

void Models::ShutdownBuffers()
{
	//release the index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	//release the vertex uffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Models::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	//Set the vertex buffer to active in the input assembler so it can be rendered
	m_object->SetVertexBuffer();

	//set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Models::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	//Create the texture object
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	//initialize the texture object
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Models::ReleaseTexture()
{
	//release the texture
	if (m_Texture)
	{
		m_Texture->Shutdown();
	}

	return;
}

bool Models::LoadModel(char* filename, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//ifstream fin;
	//char input;
	//
	////open the model file
	//fin.open(filename);

	////if it could not open the file then exit
	//if (fin.fail())
	//{
	//	return false;
	//}

	////read up to the value of vertex count
	//fin.get(input);
	//while (input != ':')
	//{
	//	fin.get(input);
	//}

	////read the vertex count
	//fin >> m_vertexCount;

	////set the number of indices to be the same as the vertex count
	//m_indexCount = m_vertexCount;

	////create the model using the vertex count that was read in 
	//m_model = new ModelType[m_vertexCount];
	//if (!m_model)
	//{
	//	return false;
	//}

	////read up to the beginning of the data
	//fin.get(input);
	//while (input != ':')
	//{
	//	fin.get(input);
	//}

	//fin.get(input);
	//fin.get(input);

	////read the vertex data
	//for (int i = 0; i < m_vertexCount; i++)
	//{
	//	fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
	//	fin >> m_model[i].tu >> m_model[i].tv;
	//	fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	//}

	////close the model file
	//fin.close();

	m_object = new ObjFileModel(filename, device, deviceContext);
	if (m_object->filename == "FILE NOT LOADED") return false;

	m_vertexCount = m_object->numverts;
	m_indexCount = m_vertexCount;

	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

	return true;
}

void Models::CalculateModelCentrePoint()
{
	float minx = 0.0f, miny = 0.0f, minz = 0.0f, maxx = 0.0f, maxy = 0.0f, maxz = 0.0f;
	for (int i = 0; i < m_object->numverts; i++)
	{
		if (m_object->vertices[i].Pos.x < minx)
		{
			minx = m_object->vertices[i].Pos.x;
		}
		else
		{
			if (m_object->vertices[i].Pos.x > maxx)
			{
				maxx = m_object->vertices[i].Pos.x;
			}
		}
		if (m_object->vertices[i].Pos.y < miny)
		{
			miny = m_object->vertices[i].Pos.y;
		}
		else
		{
			if (m_object->vertices[i].Pos.y > maxy)
			{
				maxy = m_object->vertices[i].Pos.y;
			}
		}
		if (m_object->vertices[i].Pos.z < minz)
		{
			minz = m_object->vertices[i].Pos.z;
		}
		else
		{
			if (m_object->vertices[i].Pos.z > maxz)
			{
				maxz = m_object->vertices[i].Pos.z;
			}
		}
	}

	m_bounding_sphere_centre_x = (minx + maxx) / 2;
	m_bounding_sphere_centre_y = (miny + maxy) / 2;
	m_bounding_sphere_centre_z = (minz + maxz) / 2;
	return;
}

void Models::CalculateBoundingSphereRadius()
{
	float maxDistance = 0.0f, newDistance = 0.0f;
	for (int i = 0; i < m_object->numverts; i++)
	{
		newDistance = sqrt(m_object->vertices[i].Pos.x*m_object->vertices[i].Pos.x +
			m_object->vertices[i].Pos.y*m_object->vertices[i].Pos.y +
			m_object->vertices[i].Pos.z*m_object->vertices[i].Pos.z);
		if (newDistance > maxDistance)
		{
			maxDistance = newDistance;
		}
	}
	m_bounding_sphere_centre_radius = maxDistance;
	return;
}

float Models::GetBoundingSphereRadius()
{
	return m_bounding_sphere_centre_radius;
}

float Models::GetBoundingSphereX()
{
	return m_bounding_sphere_centre_x;
}

float Models::GetBoundingSphereY()
{
	return m_bounding_sphere_centre_y;
}

float Models::GetBoundingSphereZ()
{
	return m_bounding_sphere_centre_z;
}