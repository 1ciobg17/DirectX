cbuffer CB0
{
	matrix WVPMatrix;
};//total size 64 bytes

Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position :SV_POSITION;
	float4 color: COLOR;
	float2 texcoord: TEXCOORD;
};

VOut ModelVS(float4 position: POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	float4 default_color = { 1, 1, 1, 1 };

	output.position = mul(WVPMatrix, position);

	output.texcoord = texcoord;

	output.color = default_color;

	return output;
}

float4 ModelPS(float4 position: SV_POSITION, float4 color:COLOR, float2 texcoord: TEXCOORD):SV_TARGET
{
	return texture0.Sample(sampler0, texcoord)*color;
}