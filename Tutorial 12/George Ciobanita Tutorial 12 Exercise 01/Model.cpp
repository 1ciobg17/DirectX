#include"Model.h"
#include<math.h>

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; //64 bytes(4x4=16x4 bytes)
};//total is 64 bytes

Model::Model(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext)
{
	m_pD3DDevice = pD3DDevice;
	m_pImmediateContext = pImmediateContext;

	m_x = m_y = m_z = 0.0f;
	m_xangle = m_yangle = m_zangle = 0.0f;
	m_scale = 1.0f;
}

Model::~Model()
{
	if (m_pObject)
	{
		delete m_pObject;
		m_pObject = NULL;
	}
	if (m_pVShader) m_pVShader->Release();
	if (m_pPShader) m_pPShader->Release();
	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pConstantBuffer) m_pConstantBuffer->Release();
}

int Model::LoadObjModel(char* filename)
{
	HRESULT hr = S_OK;

	ID3DBlob *VS, *PS, *error;
	D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//don't fail if error is just a warning
		{
			return hr;
		}
	}

	D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//don't fail if error is just a warning
		{
			return hr;
		}
	}

	m_pObject = new ObjFileModel(filename, m_pD3DDevice, m_pImmediateContext);

	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	if (FAILED(hr))
	{
		return hr;
	}
	
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);

	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; //can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = 64; //MUST be a multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //use as a constant buffer

	m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	
	//Tutorial 12
	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

	return S_OK;
}

void Model::Draw(XMMATRIX* view, XMMATRIX* projection)
{
	XMMATRIX world;

	XMMATRIX rotX = XMMatrixRotationX(m_xangle*XM_PI/180);//look into why im multiplying this by that
	XMMATRIX rotY = XMMatrixRotationY(m_yangle*XM_PI/180);
	XMMATRIX rotZ = XMMatrixRotationZ(m_zangle*XM_PI/180);

	XMMATRIX scale = XMMatrixScaling(m_scale, m_scale, m_scale);

	XMMATRIX translation = XMMatrixTranslation(m_x, m_y, m_z);

	world =scale*(rotX*rotY*rotZ)*translation;

	MODEL_CONSTANT_BUFFER model_cb_values;
	model_cb_values.WorldViewProjection = world*(*view)*(*projection);

	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pObject->Draw();
}

void Model::SetXPos(float x)
{
	m_x = x;
}
void Model::SetYPos(float y)
{
	m_y = y;
}

void Model::SetZPos(float z)
{
	m_z = z;
}

void Model::SetXRot(float x)
{
	m_xangle = x;
}

void Model::SetYRot(float y)
{
	m_yangle = y;
}

void Model::SetZRot(float z)
{
	m_zangle = z;
}

void Model::SetScale(float scale)
{
	m_scale = scale;
}

float Model::GetXPos()
{
	return m_x;
}

float Model::GetYPos()
{
	return m_y;
}

float Model::GetZPos()
{
	return m_z;
}

float Model::GetXRot()
{
	return m_xangle;
}

float Model::GetYRot()
{
	return m_yangle;
}

float Model::GetZRot()
{
	return m_zangle;
}

float Model::GetScale()
{
	return m_scale;
}

void Model::ChangeXPos(float x)
{
	m_x += x;
}

void Model::ChangeYPos(float y)
{
	m_y += y;
}

void Model::ChangeZPos(float z)
{
	m_z += z;
}

void Model::ChangeXRot(float x)
{
	m_xangle += x;
}

void Model::ChangeYRot(float y)
{
	m_yangle += y;
}

void Model::ChangeZRot(float z)
{
	m_zangle += z;
}

void Model::ChangeScale(float scale)
{
	m_scale += scale;
}

void Model::LookAt_XZ(float x, float z)
{
	float dx, dz;
	dx = x - m_x;
	dz = z - m_z;
	m_yangle = atan2(dx, dz)*(180.0 / XM_PI);//part after * is for degree conversion, its degrees
}

void Model::MoveForward(float distance)
{
	m_x += sin(m_yangle * (XM_PI / 180.0))*distance;
	m_z += cos(m_yangle * (XM_PI / 180.0))*distance;
}

void Model::CalculateModelCentrePoint()
{
	float minx=0.0f, miny=0.0f, minz=0.0f, maxx=0.0f, maxy=0.0f, maxz=0.0f;
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		if (m_pObject->vertices[i].Pos.x <= minx)
		{
			minx = m_pObject->vertices[i].Pos.x;
		}
		else
		{
			if (m_pObject->vertices[i].Pos.x >= maxx)
			{
				maxx = m_pObject->vertices[i].Pos.x;
			}
		}
		if (m_pObject->vertices[i].Pos.y <= miny)
		{
			miny = m_pObject->vertices[i].Pos.y;
		}
		else
		{
			if (m_pObject->vertices[i].Pos.y >= maxy)
			{
				maxy = m_pObject->vertices[i].Pos.y;
			}
		}
		if (m_pObject->vertices[i].Pos.z <= minz)
		{
			minz = m_pObject->vertices[i].Pos.z;
		}
		else
		{
			if (m_pObject->vertices[i].Pos.z >= maxz)
			{
				maxz = m_pObject->vertices[i].Pos.z;
			}
		}
	}
	m_bounding_sphere_centre_x=(minx+maxx)/2;
	m_bounding_sphere_centre_y=(miny+maxy)/2;
	m_bounding_sphere_centre_z=(minz+maxz)/2;
}

void Model::CalculateBoundingSphereRadius()
{
	float maxDistance = 0, newDistance=0;
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		newDistance = sqrt(m_pObject->vertices[i].Pos.x*m_pObject->vertices[i].Pos.x + m_pObject->vertices[i].Pos.y*m_pObject->vertices[i].Pos.y + m_pObject->vertices[i].Pos.z*m_pObject->vertices[i].Pos.z);
		if (newDistance>maxDistance)
		{
			maxDistance = newDistance;
		}
	}
	m_bounding_sphere_centre_radius = maxDistance;
}