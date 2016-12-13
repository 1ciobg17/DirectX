/////////////////
//Filename:Font.hlsl
/////////////////////

Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
	float4 pixelColor;
};

cbuffer PerFrameBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position:POSITION;
	float2 tex:TEXCOORD0;
};

struct PixelInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD0;
};

//Vertex Shader
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;

	//change the position vector to be 4 units for proper matrix calculations
	input.position.w = 1.0f;

	//calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//store the texture coordinates for the pixel shader
	output.tex = input.tex;

	return output;
}

//Pixel Shader
float4 FontPixelShader(PixelInputType input):SV_TARGEt
{
	float4 color;
	
	//sample the texture pixel at this location
	color = shaderTexture.Sample(SampleType, input.tex);

	//if the color if black on the texture then treat this pixel as transparent 
	if (color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	//if the color is other than black on the texture then this is a pixel on the font so draw it using the font pixel color
	else
	{
		color.a = 1.0f;
		color = color*pixelColor;
	}

	return color;
}