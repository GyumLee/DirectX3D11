#include "../Header.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	return output;
}

cbuffer ValueBuffer : register(b10)
{
	float maxValue;
	float curValue;
	float lerpValue;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = diffuseMap.Sample(samp, input.uv) * mDiffuse;
	
	if (input.uv.x < curValue / maxValue)
		return color;
	else if (input.uv.x < lerpValue / maxValue)
		return float4(color.rgb, 0.5f);
	
	return float4(0, 0, 0, 0);
}