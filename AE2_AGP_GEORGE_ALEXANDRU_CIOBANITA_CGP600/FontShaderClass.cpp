/////////////////////////////////////
// Filename: FontShaderClass.cpp
//////////////////////////////////////
#include "FontShaderClass.h"

FontShaderClass::FontShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
	m_pixelBuffer = 0;
}

FontShaderClass::FontShaderClass(const FontShaderClass& other)
{

}

FontShaderClass::~FontShaderClass()
{

}

bool FontShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"Font.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void FontShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders 
	ShutdownShader();

	return;
}

bool FontShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	bool result;

	//set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);
	if (!result)
	{
		return false;
	}

	//Now Render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool FontShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;

	//initialize the pointers
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Compile the vertex shader code
	result = D3DX11CompileFromFile(filename, NULL, NULL, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//if the shader failed to compile it should have written something to the message 
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

	//compile the pixel shader code
	result = D3DX11CompileFromFile(filename, NULL, NULL, "FontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
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
	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}
	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
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

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	//setup the description of the dynamic pixel constant buffer that is in the pixel shader
	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

	//create the pixel constant buffer pointer so we can access the pixel shader constant buffer from withing this class
	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pixelBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FontShaderClass::ShutdownShader()
{
	//release the pixel constant buffer
	if (m_pixelBuffer)
	{
		m_pixelBuffer->Release();
		m_pixelBuffer = 0;
	}

	//release the sampler state
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	//release the matrix constant buffer
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
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

void FontShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
	return;
}

bool FontShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	PixelBufferType* dataPtr2;
	unsigned int bufferNumber;

	//Tranpose the matrices to prepare them for the shader
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//lock the constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//copy the matrices
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//now set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//set the shader texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);

	//lock the pixel constant buffer so it can be wrriten to
	result = deviceContext->Map(m_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the pixel constant buffer
	dataPtr2 = (PixelBufferType*)mappedResource.pData;

	//copy the pixel color into the pixel constant buffer
	dataPtr2->pixelColor = pixelColor;

	//unlock the pixel constant buffer
	deviceContext->Unmap(m_pixelBuffer, 0);
	
	//set the position of the pixel constant buffer in the pixel shader
	bufferNumber = 0;

	//now set the pixel constant buffer in the pixel shader with the updated value
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelBuffer);

	return true;
}

void FontShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}