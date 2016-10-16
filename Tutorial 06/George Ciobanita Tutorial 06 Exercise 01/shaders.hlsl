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
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	color.r *= red_fraction;
	output.position = mul(WVPMatrix, position);//Tutorial 5 Exercise 01

	//output.position.xy *= scale;//same as above
	//above was commented out for the purposes of Tutorial 5 Exercise 01

	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}