///////////////////////////////
// Filename: GraphicsClass.h
///////////////////////////////
#pragma once

#include "D3DClass.h"
#include "Models.h"
#include "Colorshaderclass.h"
//#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "TextClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
private:
	D3DClass* m_D3D;
	Models* m_Model;
	ColorShaderClass* m_ColorShader;
	//TextureShaderClass* m_textureShader;
	LightShaderClass* m_lightShader;
	LightClass* m_light;
	TextClass* m_text;

	bool Render(float, D3DXMATRIX);
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND, D3DXMATRIX);
	void Shutdown();
	bool Frame(int, int, float, int, int, D3DXMATRIX);
};

