#include "../Header.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
};

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
	LightPixelInput output;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
	float depth = input.pos.z / input.pos.w; //0~1;
	
	return float4(depth.xxx, 1);
}