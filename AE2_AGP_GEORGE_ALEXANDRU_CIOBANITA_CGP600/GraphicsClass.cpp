//////////////////////////
// Filename:: GraphicsClass.cpp
////////////////////////////////

#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Model = 0;
	m_ColorShader = 0;
	//m_textureShader = 0;
	m_lightShader = 0;
	m_light = 0;
	m_text = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd, D3DXMATRIX viewMatrix)
{
	bool result;
	D3DXMATRIX baseViewMatrix=viewMatrix;

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

	//create the text object
	m_text = new TextClass;
	if (!m_text)
	{
		return false;
	}

	//initialize the text object
	result = m_text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object", L"Error", MB_OK);
		return false;
	}

	//create the model
	m_Model = new Models;
	if (!m_Model)
	{
		return false;
	}

	//initialize the model
	result = m_Model->Initialize(m_D3D->GetDevice(),"assets/cube.obj",L"assets/seafloor.dds", m_D3D->GetDeviceContext());
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	//create the light shader object
	m_lightShader = new LightShaderClass;
	if (!m_lightShader)
	{
		return false;
	}

	//initialize the light shader object
	result = m_lightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	//create the light object
	m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}

	//initialize the light object
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);
	 
	return true;
}

void GraphicsClass::Shutdown()
{
	//Release the texture shader object
	/*if (m_textureShader)
	{
		m_textureShader->Shutdown();
		delete m_textureShader;
		m_textureShader = 0;
	}*/

	//release the text object
	if (!m_text)
	{
		m_text->Shutdown();
		delete m_text;
		m_text = 0;
	}

	//release the light object
	if (m_light)
	{
		delete m_light;
		m_light = 0;
	}

	//release the light shader object
	if (m_lightShader)
	{
		m_lightShader->Shutdown();
		m_lightShader = 0;
	}

	//release the model
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, int mouseX, int mouseY, D3DXMATRIX viewMatrix)
{
	bool result;

	static float rotation = 0.0f;

	//set the frames per second
	result = m_text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//set the cpu usage
	result = m_text->SetCPU(cpu, m_D3D->GetDeviceContext());

	//update the rotation variable each frame
	rotation += (float)D3DX_PI*0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	//set the location of the mouse
	result = m_text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//Render the graphics scene
	result = Render(rotation, viewMatrix);
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(float rotation, D3DXMATRIX viewmatrix)
{
	D3DXMATRIX viewMatrix=viewmatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;

	//Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Get the world , view and projection matrices from the camera and d3d objects
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//turn on the alpha blending before rendering the text
	m_D3D->TurnOnAlphaBlending();

	//render the text strings
	result = m_text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	//turn off alpha blending after rendering the text
	m_D3D->TurnOffAlphaBlending();

	//rotate the world matrix by the rotation value so that the cube will spin
	D3DXMatrixRotationY(&worldMatrix, rotation);

	//put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

	//Render the model using the light shader
	result = m_lightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetVertexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	//Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}