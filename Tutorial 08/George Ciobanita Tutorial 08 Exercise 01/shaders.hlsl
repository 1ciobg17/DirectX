Texture2D texture0;
SamplerState sampler0;

cbuffer CBuffer0
{
	matrix WVPMatrix;   //64 bytes
	float red_fraction; //4 bytes
	float scale;		//4 bytes
	float2 packing;		//2x4 bytes = 8 bytes
	
};//total of 80bytes

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord: TEXCOORD)
{
	VOut output;

	color.r *= red_fraction;
	output.position = mul(WVPMatrix, position);//Tutorial 5 Exercise 01

	//output.position.xy *= scale;//same as above
	//above was commented out for the purposes of Tutorial 5 Exercise 01

	output.color = color;

	output.texcoord = texcoord;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color* texture0.Sample(sampler0, texcoord);
}