#include "../Header.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
};

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
	LightPixelInput output;
	
	matrix transform = mul(SkinWorld(input.indices, input.weights), world);
	
	output.pos = mul(input.pos, transform);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
	float depth = input.pos.z / input.pos.w; //0~1;
	
	return float4(depth.xxx, 1);
}