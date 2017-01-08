/////////////////////////////////
// Filename: LightShaderClass.cpp
/////////////////////////////////
#include "LightShaderClass.h"

LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
	m_lightBuffer=0;
}

LightShaderClass::LightShaderClass(const LightShaderClass& other)
{

}

LightShaderClass::~LightShaderClass()
{

}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"Light.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void LightShaderClass::Shutdown()
{
	//shutdown the vertex and pixel shaders as well as other related objets
	ShutdownShader();

	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection,D3DXVECTOR4 ambientColor ,D3DXVECTOR4 diffuseColor)
{
	bool result;

	//set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor);
	if (!result)
	{
		return false;
	}

	//now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	//initialize the pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//compile the vertex shader code
	result = D3DX11CompileFromFile(filename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//if the shader failed to compile it should have written something to the error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		//if there was nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//compile the pixel shader code
	result = D3DX11CompileFromFile(filename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, filename);
		}
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	//create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//create the pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//create the vertex input layout description
	//this setup has to match the vertextype structure in Models class and the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result))
	{
		return false;
	}

	//release the vertex and pixel buffers
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//create a texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	//setup the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//setup the description of the light dynamic constant buffer that is in the pixel shader
	//note that the bytewidth always needs to be a multiple of 16 
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//create the constant buffer pointer so we can access the vertex shader constant buffer from withing this class
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	//release the light constant buffer
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer=0;
	}

	//release the matrix constant buffer
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//release the sampler state
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	//release the layout
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	//release the pixel shader
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	//release the vertex shader
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename)
{
	char* compileErrors;
	unsigned long bufferSize;
	ofstream fout;

	//get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//get the length of the message 
	bufferSize = errorMessage->GetBufferSize();

	//open a file to write the error message to
	fout.open("shader-error.txt");

	//write out the error message
	for (int i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	//close the file 
	fout.close();

	//release the error message
	errorMessage->Release();
	errorMessage = 0;

	//pop a message
	MessageBox(hwnd, L"Error compiling shaer. Check shader-error.txt for message.", filename, MB_OK);

	return;
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection,D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;
	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	
	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;
	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);
	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;
	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this object
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the object
	deviceContext->Draw(vertexCount,0);

	return;
}