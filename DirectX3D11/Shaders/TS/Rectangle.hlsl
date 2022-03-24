#include "../Header.hlsli"

Vertex VS(Vertex input)
{
	return input;
}

struct CHullOutput
{
	float edgeTessFactor[4] : SV_TessFactor;
	float insideTessFactor[2] : SV_InsideTessFactor;
};

cbuffer EdgeInfo : register(b10)
{
	float edge0;
	float edge1;
	float edge2;
	float edge3;
}

cbuffer InsideInfo : register(b11)
{
	float inside0;
	float inside1;
}

#define NUM_CONTROL_POINTS 4

CHullOutput CHS(InputPatch<Vertex, NUM_CONTROL_POINTS> input)
{
	CHullOutput output;

	output.edgeTessFactor[0] = edge0;
	output.edgeTessFactor[1] = edge1;
	output.edgeTessFactor[2] = edge2;
	output.edgeTessFactor[3] = edge3;
	
	output.insideTessFactor[0] = inside0;
	output.insideTessFactor[1] = inside1;

	return output;
}

struct HullOutput
{
	float4 pos : POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<Vertex, NUM_CONTROL_POINTS> input, uint i : SV_OutputControlPointID)
{
	HullOutput output;

	output.pos = input[i].pos;

	return output;
}

struct DomainOutput
{
	float4 pos : SV_Position;
};

[domain("quad")]
DomainOutput DS(CHullOutput input, float2 uv : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
	DomainOutput output;
	
	float4 v1 = lerp(patch[0].pos, patch[2].pos, uv.x);
	float4 v2 = lerp(patch[1].pos, patch[3].pos, uv.x);
	
	float4 position = lerp(v1, v2, 1 - uv.y);
	output.pos = float4(position.xyz, 1.0f);

	return output;
}

float4 PS(DomainOutput output) : SV_TARGET
{
	return float4(1, 1, 0, 1);
}