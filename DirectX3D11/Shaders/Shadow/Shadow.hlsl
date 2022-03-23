#include "../Header.hlsli"

cbuffer LightView : register(b11)
{
	matrix lightView;
}

cbuffer LightProjection : register(b12)
{
	matrix lightProjection;
}

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewPos : POSITION0;
	float3 worldPos : POSITION1;
	float4 clipPos : POSITION2;
};

PixelInput VS(VertexUVNormalTangent input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	
	output.worldPos = output.pos;
	output.viewPos = invView._41_42_43;
	
	output.clipPos = mul(output.pos, lightView);
	output.clipPos = mul(output.clipPos, lightProjection);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

Texture2D depthMap : register(t10);

float4 PS(PixelInput input) : SV_TARGET
{
	LightPixelInput lightInput;
	lightInput.uv = input.uv;
	lightInput.tangent = input.tangent;
	lightInput.binormal = input.binormal;
	lightInput.normal = input.normal;
	lightInput.worldPos = input.worldPos;
	lightInput.viewPos = input.viewPos;
	
	Material material = GetMaterial(lightInput);
	
	float4 result = CalcLights(material);
	float4 ambient = CalcAmbient(material);
	float4 emissive = CalcEmissive(material);
	
	float4 color = result + ambient + emissive;
	
	float2 uv = input.clipPos.xy / input.clipPos.w;
	uv.y = -uv.y;
	uv = uv * 0.5f + 0.5f;
	
	if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
		return float4(color.rgb, material.diffuseColor.a);
	
	float shadowDepth = depthMap.Sample(samp, uv).r;
	
	float currentDepth = input.clipPos.z / input.clipPos.w;
	
	if (currentDepth > shadowDepth + 0.0001f)
		return color * 0.5f;
	
	return float4(color.rgb, material.diffuseColor.a);
}