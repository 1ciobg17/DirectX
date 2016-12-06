///////////////////////////////
// Filename: GraphicsClass.h
///////////////////////////////
#pragma once

#include "D3DClass.h"
#include "Camera.h"
#include "Models.h"
#include "Colorshaderclass.h"
#include "TextureShaderClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
private:
	D3DClass* m_D3D;
	Camera* m_Camera;
	Models* m_Model;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_textureShader;

	bool Render();
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
};

