////////////////////////////////
// Filename: Light.hlsl
////////////////////////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding
};

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position:POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL;
};

struct PixelInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD0;
	float3 normal:NORMAL:
};

//Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
	VertexInputType output;

	//change the vector position to be 4 units for proper maths calculations
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	//Calculate the normal against the world matrix
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//normalize the vector
	output.normal = normalize(output.normal);

	return output;
}

float4 LightPixelShader(PixelInputType input):SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	//sample the pixel color from the texture using the sampler at this texture coordinate location
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	//invert the light direction for calculations
	lightDir = -lightDirection;

	//calculate the amount of light on this pixel
	lightIntensity = saturate(dot(input.normal, lightDir));

	//determine the final amount of diffuse color based on the diffuse color combined with the light intensity
	color = saturate(diffuseColor*lightIntensity);

	//Multiply the texture pixel and the final diffuse color to get the final pixel color result
	color = color*textureColor;

	return color;
}