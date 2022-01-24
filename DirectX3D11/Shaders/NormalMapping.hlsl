#include "Header.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangent input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	
	float3 camPos = invView._41_42_43;
	output.viewDir = normalize(output.pos.xyz - camPos);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 albedo = float4(1, 1, 1, 1);
	if (hasDiffuseMap)
		albedo = diffuseMap.Sample(samp, input.uv);
	
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);
	
	float3 normal = N;
	
	if (hasNormalMap)
	{
		float4 normalMapping = normalMap.Sample(samp, input.uv);
	
		normal = normalMapping * 2.0f - 1.0f; // 0~1 -> -1~1
		float3x3 TBN = float3x3(T, B, N);
		normal = normalize(mul(normal, TBN));
	}
	
	float3 lightDirection = normalize(light.direction);
	
	float diffuseIntensity = saturate(dot(normal, -lightDirection));
	
	float4 specular = 0;
	if (diffuseIntensity > 0)
	{
		//Blinn Phong Shading
		float3 halfWay = normalize(input.viewDir + lightDirection);
		specular = saturate(dot(normal, -halfWay));
		
		float4 specularIntensity = 1.0f;
		
		if (hasSpecularMap)
			specularIntensity = specularMap.Sample(samp, input.uv);
		
		specular = pow(specular, shininess) * specularIntensity * mSpecular * light.color;
	}
	
	float emissiveIntensity = 0.0f;
	
	if (mEmissive.a > 0.0f)
	{
		float t = saturate(dot(normal, -input.viewDir));
		emissiveIntensity = smoothstep(1.0f - mEmissive.a, 1.0f, 1.0f - t);
	}
	
	float4 diffuse = albedo * diffuseIntensity * light.color * mDiffuse;
	float4 ambient = albedo * ambientLight * mAmbient;
	float4 emissive = mEmissive * emissiveIntensity;
	
	return albedo * diffuse + specular + ambient + emissive;
}