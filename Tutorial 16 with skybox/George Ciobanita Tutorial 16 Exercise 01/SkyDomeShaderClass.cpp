#include "SkyDomeShaderClass.h"

SkyDomeShaderClass::SkyDomeShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_gradientBuffer = 0;
}

SkyDomeShaderClass::SkyDomeShaderClass(const SkyDomeShaderClass& other)
{
}

SkyDomeShaderClass::~SkyDomeShaderClass()
{
}

bool SkyDomeShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = InitializeShader(device, hwnd, L"SkyDomeVS.hlsl", L"SkyDomePS.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void SkyDomeShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel shaders as well as the related objects
	ShutdownShader();
}

bool SkyDomeShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	bool result;

	//set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, apexColor, centerColor);
	if (!result)
	{
		return false;
	}

	//now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);
}

bool SkyDomeShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC gradientBufferDesc;

	//Initialize the pointers this function will use to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DX11CompileFromFile("SkydomeVS.hlsl", NULL, NULL, "SkyDomeVertexShader", "vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		//if the shader failed to compile it should have written something to the error message
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		//if there was nothing in the error message then it simple could not find the shader file itself
		else
		{
			MessageBox(hwnd, "SkydomeVS.hlsl", "Missing Shader File", MB_OK);
		}

		return false;
	}

	//compile the pixel shader code
	result = D3DX11CompileFromFile("SkyDomePS.hlsl", NULL, NULL, "SkyDomePixelShader", "ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		//if the shader failed to compile it should have written something to the error message
		if (errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		else
		{
			MessageBox(hwnd, "SkyDomePS.hlsl", "Missing Shader File", MB_OK);
		}
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
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	//get a count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0; 

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

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

	//setup the description of the gradient constant buffer that is in the pixel shader
	gradientBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gradientBufferDesc.ByteWidth = sizeof(GradientBufferType);
	gradientBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gradientBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gradientBufferDesc.MiscFlags = 0;
	gradientBufferDesc.StructureByteStride = 0;

	//create the constant buffer pointer so we can access the pixel shader constant buffer from within this class 
	result = device->CreateBuffer(&gradientBufferDesc, NULL, &m_gradientBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SkyDomeShaderClass::ShutdownShader()
{
	//release the gradient constant buffer
	if (m_gradientBuffer)
	{
		m_gradientBuffer->Release();
		m_gradientBuffer = 0;
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
		m_layout=0;
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

void SkyDomeShaderClass::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	//get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	//open a file to write the error message to
	fout.open("shader-error.txt");
	//write out the error message
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	//close the file
	fout.close();

	//release the error message
	errorMessage->Release();
	errorMessage = 0;

	//pop a message up on the screen to notify the user to check the text file for compile errors
	MessageBox(hwnd, "Error compiling shader. Check shader-error.txt for message.", "shader file name here", MB_OK);

	return;
}

bool SkyDomeShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	GradientBufferType* dataPtr2;

	//Tranpose the matrices to prepare them for the shader
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	//lock the matix constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the matrix constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//copy the matrices into the matrix constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//unlock the matrix constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//now set the matrix constant buffer in the vertex shader with the update values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//following section sets the apex and center colors in the pixel shader

	//lock the color constant buffer so it can be written to
	result = deviceContext->Map(m_gradientBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//get a pointer to the data in the color constant buffer
	dataPtr2 = (GradientBufferType*)mappedResource.pData;

	//copy the color data into the color constant buffer
	dataPtr2->apexColor = apexColor;
	dataPtr2->centerColor = centerColor;

	//unlock the color constant buffer
	deviceContext->Unmap(m_gradientBuffer, 0);

	//set the position of the color constant buffer in the pixel shader with the update color values
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_gradientBuffer);

	return true;
}

void SkyDomeShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//set the vertex and pixel shaders that will be used to render the triangles
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//render the triangles
	deviceContext->DrawIndexed(indexCount, 0, 0);
	
	return;
}