#include "../Header.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
	LightPixelInput output;
	
	output.pos = mul(input.pos, world);
	
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
	
	float4 result = CalcLights(material);
	float4 ambient = CalcAmbient(material);
	float4 emissive = CalcEmissive(material);
	
	//return result + ambient + emissive;
	
	float4 color = result + ambient + emissive;
	
	return float4(color.rgb, material.diffuseColor.a);
}