//////////////////////////
// Filename:: GraphicsClass.cpp
////////////////////////////////

#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
	m_textureShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//Create the Direct3D object
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	//Initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create the camera
	m_Camera = new Camera();
	if (!m_Camera)
	{
		return false;
	}

    //set the initial position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	//create the model
	m_Model = new Models;
	if (!m_Model)
	{
		return false;
	}

	//initialize the model
	result = m_Model->Initialize(m_D3D->GetDevice(), L"assets/seafloor.dds");
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	//Create the texture shader object
	m_textureShader = new TextureShaderClass;
	if (!m_textureShader)
	{
		return false;
	}

	//initialize the texture shader object
	result = m_textureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}


	return true;
}

void GraphicsClass::Shutdown()
{
	//Release the color shader object
	if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}

	//release the model
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	//release the camera
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	//Render the graphics scene
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	//Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Generate the view matrix based on the camera's position
	m_Camera->Render();

	//Get the world , view and projection matrices from the camera and d3d objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//ut the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

	//Render the model using the color shader
	result = m_textureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	//Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}