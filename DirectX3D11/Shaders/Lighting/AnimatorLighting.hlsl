#include "../Header.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
	LightPixelInput output;
	
	matrix transform = mul(SkinWorld(input.indices, input.weights), world);
	
	output.pos = mul(input.pos, transform);
	
	output.worldPos = output.pos;
	output.viewPos = invView._41_42_43;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
	Material material = GetMaterial(input);
	
	float4 ambient = CalcAmbient(material);
	
	return ambient;
}