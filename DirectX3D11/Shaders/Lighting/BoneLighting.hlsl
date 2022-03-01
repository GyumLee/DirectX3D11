#include "../Header.hlsli"

cbuffer BoneBuffer : register(b10)
{
	matrix bones[MAX_BONE];
}

cbuffer BoneIndexBuffer : register(b11)
{
	int boneIndex;
}

LightPixelInput VS(VertexUVNormalTangent input)
{
	LightPixelInput output;
	
	matrix transform = mul(bones[boneIndex], world);
	
	output.pos = mul(input.pos, transform);
	
	output.worldPos = output.pos;
	output.viewPos = invView._41_42_43;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) transform);
	output.tangent = mul(input.tangent, (float3x3) transform);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
	Material material = GetMaterial(input);
	
	float4 result = CalcLights(material);
	float4 ambient = CalcAmbient(material);
	float4 emissive = CalcEmissive(material);
	
	return result + ambient + emissive;
}