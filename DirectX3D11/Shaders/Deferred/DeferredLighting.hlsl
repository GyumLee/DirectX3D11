#include "../Header.hlsli"

struct PixelInput
{
	float4 pos : SV_Position;
	float2 uv : UV;
};

static const float2 arrBasePos[4] =
{
	float2(-1.0f, +1.0f),
	float2(+1.0f, +1.0f),
	float2(-1.0f, -1.0f),
	float2(+1.0f, -1.0f)
};

PixelInput VS(uint vertexID : SV_VertexID)
{
	PixelInput output;
	output.pos = float4(arrBasePos[vertexID], 0.0f, 1.0f);
	output.uv = output.pos.xy;
	
	return output;
}

cbuffer ViewBuffer : register(b10)
{
	matrix pView;
	matrix pInView;
}

cbuffer ProjectionBuffer : register(b11)
{
	matrix pProjection;
}

Texture2D depthTexture : register(t10);
Texture2D diffuseTexture : register(t11);
Texture2D specularTexture : register(t12);
Texture2D normalTexture : register(t13);

float ConvertDepthToLinear(float depth)
{
	return projection._43 / (depth - pProjection._33);
}

struct SurfaceData
{
	float linearDepth;
	float3 color;
	float3 normal;
	float specInt;
	float specPow;
};

SurfaceData UnPackGBuffer(int2 location)
{
	SurfaceData output;
	
	int3 location3 = int3(location, 0);
	
	float depth = depthTexture.Load(location3).x;
	output.linearDepth = ConvertDepthToLinear(depth);
	
	output.color = diffuseTexture.Load(location3).rgb;
	
	float4 specular = specularTexture.Load(location3);
	output.specPow = shininessRange.x + specular.x * shininessRange.y;
	output.specInt = specular.y;
	
	output.normal = normalTexture.Load(location3).xyz;
	output.normal = normalize(output.normal * 2.0f - 1.0f);
	
	return output;
}

float3 CalcWorldPos(float2 csPos, float linearDepth)
{
	float4 position;
	
	float2 temp;
	temp.x = 1 / pProjection._11;
	temp.y = 1 / pProjection._22;
	position.xy = csPos.xy * temp * linearDepth;
	position.z = linearDepth;
	position.w = 1.0f;
	
	return mul(position, pInView).xyz;
}

float4 PS(PixelInput input) : SV_TARGET
{
	SurfaceData data = UnPackGBuffer(input.pos.xy);
	
	Material material;
	material.normal = data.normal;
	material.diffuseColor = float4(data.color, 1.0f);
	material.viewPos = pInView._41_42_43;;
	material.specularIntensity = data.specInt;
	material.shininess = data.specPow;
	material.worldPos = CalcWorldPos(input.uv, data.linearDepth);
	
	float4 result = CalcLights(material);
	float4 ambient = CalcAmbient(material);
	
	return result + ambient;
}