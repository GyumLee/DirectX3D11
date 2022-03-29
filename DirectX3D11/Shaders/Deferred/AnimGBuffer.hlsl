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

struct PixelOutput
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1;
	float4 normal : SV_Target2;
};

PixelOutput PackGBuffer(float3 baseColor, float3 normal, float specIntensity)
{
	PixelOutput output;
	
	float specPowNorm = (shininess - shininessRange.x) / shininessRange.y;
	
	output.diffuse = float4(baseColor, 1);
	output.specular = float4(specPowNorm, specIntensity, 0, 1);
	output.normal = float4(normal * 0.5f + 0.5f, 1);
	
	return output;
}

PixelOutput PS(LightPixelInput input)
{
	float3 albedo = float3(1, 1, 1);
	if (hasDiffuseMap)
		albedo = diffuseMap.Sample(samp, input.uv).rgb;
	albedo *= albedo;
	
	float specIntensity = 1.0f;
	
	if (hasSpecularMap)
		specIntensity = specularMap.Sample(samp, input.uv).r;
	
	float3 normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
	
	return PackGBuffer(albedo, normal, specIntensity);
}