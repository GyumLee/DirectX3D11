#include "../Header.hlsli"

VertexUV VS(VertexUV input)
{
	return input;
}

struct CHullOutput
{
	float edgeTessFactor[4] : SV_TessFactor;
	float insideTessFactor[2] : SV_InsideTessFactor;
};

cbuffer TerrainBuffer : register(b10)
{
	float minDistance;
	float maxDistance;
	float minQuality;
	float maxQuality;
}

float CalcTessFactor(float3 position)
{
	float d = distance(position, invView._41_42_43);
	float f = saturate((d - maxDistance) / (minDistance - maxDistance));
	
	return lerp(minQuality, maxQuality, f);
}

#define NUM_CONTROL_POINTS 4

CHullOutput CHS(InputPatch<VertexUV, NUM_CONTROL_POINTS> input)
{
	CHullOutput output;

	float4 position[4];
	position[0] = mul(input[0].pos, world);
	position[1] = mul(input[1].pos, world);
	position[2] = mul(input[2].pos, world);
	position[3] = mul(input[3].pos, world);

	float3 e0 = (position[0] + position[2]).xyz * 0.5f;
	float3 e1 = (position[0] + position[1]).xyz * 0.5f;
	float3 e2 = (position[1] + position[3]).xyz * 0.5f;
	float3 e3 = (position[2] + position[3]).xyz * 0.5f;
	
	output.edgeTessFactor[0] = CalcTessFactor(e0);
	output.edgeTessFactor[1] = CalcTessFactor(e1);
	output.edgeTessFactor[2] = CalcTessFactor(e2);
	output.edgeTessFactor[3] = CalcTessFactor(e3);
	
	float3 center = (e0 + e2) * 0.5f;
	
	output.insideTessFactor[0] = CalcTessFactor(center);
	output.insideTessFactor[1] = CalcTessFactor(center);
	
	return output;
}

struct HullOutput
{
	float4 pos : POSITION;
	float2 uv : UV;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<VertexUV, NUM_CONTROL_POINTS> input, uint i : SV_OutputControlPointID)
{
	HullOutput output;

	output.pos = input[i].pos;
	output.uv = input[i].uv;

	return output;
}

struct DomainOutput
{
	float4 pos : SV_Position;
	float2 uv : UV;
};

cbuffer HeightBuffer : register(b10)
{
	float heightScale;
}

Texture2D heightMap : register(t0);

[domain("quad")]
DomainOutput DS(CHullOutput input, float2 uv : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
	DomainOutput output;
	
	float4 v1 = lerp(patch[0].pos, patch[1].pos, uv.x);
	float4 v2 = lerp(patch[2].pos, patch[3].pos, uv.x);
	float4 position = lerp(v1, v2, uv.y);
	
	float2 uv1 = lerp(patch[0].uv, patch[1].uv, uv.x);
	float2 uv2 = lerp(patch[2].uv, patch[3].uv, uv.x);
	float2 texCoord = lerp(uv1, uv2, uv.y);

	position.y = heightMap.SampleLevel(samp, texCoord, 0).r * heightScale;
	
	output.pos = float4(position.xyz, 1.0f);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = texCoord;
	
	return output;
}

float4 PS(DomainOutput input) : SV_TARGET
{
	return diffuseMap.Sample(samp, input.uv);
}