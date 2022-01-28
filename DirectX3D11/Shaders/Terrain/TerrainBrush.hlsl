#include "../Header.hlsli"

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : VIEWDIR;
	float3 worldPos : POSITION;
};

PixelInput VS(VertexUVNormalTangent input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	
	float3 camPos = invView._41_42_43;
	output.worldPos = output.pos.xyz;
	output.viewDir = normalize(output.worldPos - camPos);
	
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3) world);
	output.tangent = mul(input.tangent, (float3x3) world);
	output.binormal = cross(output.normal, output.tangent);
	
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
	
	float4 brushColor = float4(BrushColor(input.worldPos), 1.0f);
	
	return albedo * diffuse + specular + ambient + emissive + brushColor;
}