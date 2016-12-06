TextureCube cube0;
SamplerState sampler0;

cbuffer CBuffer0
{
	matrix WVPMatrix;   //64 bytes
	float4 directional_light_vector; //16 bytes
	float4 directional_light_colour; //16 bytes
	float4 ambient_light_colour; //16 bytes

};//total of 112 bytes

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 texcoord: TEXCOORD, float3 normal: NORMAL)
{
	VOut output;

	// output.position = mul(WVPMatrix, position);//Tutorial 5 Exercise 01

	// unit cube so just place it in the middle of the screen for testing
	output.position = position;

	float diffuse_amount = dot(directional_light_vector, float4(0,normal));

	diffuse_amount = saturate(diffuse_amount);

	output.color = ambient_light_colour + (directional_light_colour*diffuse_amount);

	output.texcoord = position.xyz;

	// why are we doing the position twice?
	//output.position = mul(WVPMatrix, position);

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
	return cube0.Sample(sampler0, texcoord);
}