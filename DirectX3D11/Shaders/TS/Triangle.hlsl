#include "../Header.hlsli"

Vertex VS( Vertex input )
{
	return input;
}

struct CHullOutput
{
	float edgeTessFactor[3] : SV_TessFactor;
	float insideTessFactor : SV_InsideTessFactor;
};

cbuffer EdgeInfo : register(b10)
{
	float edge0;
	float edge1;
	float edge2;
	float inside;
}

#define NUM_CONTROL_POINTS 3

CHullOutput CHS(InputPatch<Vertex, NUM_CONTROL_POINTS> input)
{
	CHullOutput output;

	output.edgeTessFactor[0] = edge0;
	output.edgeTessFactor[1] = edge1;
	output.edgeTessFactor[2] = edge2;
	output.insideTessFactor = inside;

	return output;
}

struct HullOutput
{
	float4 pos : POSITION;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
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

[domain("tri")]
DomainOutput DS(CHullOutput input, float3 uvw : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
	DomainOutput output;
	
	float4 position = patch[0].pos * uvw.x + patch[1].pos * uvw.y + patch[2].pos * uvw.z;
	output.pos = float4(position.xyz, 1.0f);

	return output;
}

float4 PS(DomainOutput output) : SV_TARGET
{
	return float4(1, 1, 0, 1);
}