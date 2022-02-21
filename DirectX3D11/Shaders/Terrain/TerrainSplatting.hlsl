#include "../Header.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangent input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	
	float3 camPos = invView._41_42_43;
	output.viewDir = normalize(output.pos.xyz - camPos);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

Texture2D alphaMap : register(t10);
Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = float4(1, 1, 1, 1);
	if (hasDiffuseMap)
		albedo = diffuseMap.Sample(samp, input.uv);
	
	float4 alpha = alphaMap.Sample(samp, input.uv);
	float4 second = secondMap.Sample(samp, input.uv);
	float4 third = thirdMap.Sample(samp, input.uv);
	
	albedo = lerp(albedo, second, alpha.r);
	albedo = lerp(albedo, third, alpha.g);
	
	return albedo;
}