#include "../Header.hlsli"

struct VertexInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 alpha : ALPHA;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewPos : POSITION0;
	float3 worldPos : POSITION1;
	float4 alpha : ALPHA;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	
	output.viewPos = invView._41_42_43;
	output.worldPos = output.pos.xyz;
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	output.alpha = input.alpha;
	
	return output;
}

cbuffer BrushBuffer : register(b10)
{
	int type;
	float3 pickingPos;
	
	float3 color;
	float range;
}

float3 BrushColor(float3 pos)
{
	float x = pos.x - pickingPos.x;
	float z = pos.z - pickingPos.z;
	
	switch (type)
	{
		case 0:
			float distance = sqrt(x * x + z * z);
		
			if (distance <= range) 
				return color;
			break;
		case 1:
			if (abs(x) < range && abs(z) < range)
				return color;
			break;
		case 2:
			float3 P = float3(pickingPos.xz, 0.0f);
			float3 A = P + float3(cos(radians(90)) * range, sin(radians(90)) * range, 0);
			float3 B = P + float3(cos(radians(210)) * range, sin(radians(210)) * range, 0);
			float3 C = P + float3(cos(radians(330)) * range, sin(radians(330)) * range, 0);
		
			float3 T = float3(pos.xz, 0.0f);
		
			if (cross(B - A, T - A).z * cross(C - A, T - A).z > 0)
				return float3(0, 0, 0);
		
			if (cross(A - B, T - B).z * cross(C - B, T - B).z > 0)
				return float3(0, 0, 0);
		
			return color;
			break;
	}
	
	return float3(0, 0, 0);
}

Texture2D secondDiffuseMap : register(t11);
Texture2D thirdDiffuseMap : register(t12);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = float4(1, 1, 1, 1);
	if (hasDiffuseMap)
		albedo = diffuseMap.Sample(samp, input.uv);
	
	float4 second = secondDiffuseMap.Sample(samp, input.uv);
	float4 third = thirdDiffuseMap.Sample(samp, input.uv);
	
	albedo = lerp(albedo, second, input.alpha.x);
	albedo = lerp(albedo, third, input.alpha.y);
	
	Material material;
	material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
	material.diffuseColor = albedo;
	material.viewPos = input.viewPos;
	material.specularIntensity = SpecularMapping(input.uv);
	material.worldPos = input.worldPos;
	
	float4 result = CalcLights(material);
	float4 ambient = CalcAmbient(material);
	
	float4 brushColor = float4(BrushColor(input.worldPos), 1.0f);
	
	return result + ambient + brushColor;
}