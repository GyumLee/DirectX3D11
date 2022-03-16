#include "../Header.hlsli"

cbuffer WeatherBuffer : register(b10)
{
	float3 velocity;
	float drawDistance;
	
	float4 color;
	
	float3 origin;
	float time;
	
	float3 size;
}

struct VertexOutput
{
	float3 pos : POSITION;
	float2 size : SIZE;
	float distance : DISTANCE;
	float4 color : COLOR;
	float3 velocity : VELOCITY;
};

struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
	float4 color : COLOR;
	float alpha : ALPHA;
};

VertexOutput VS(VertexSize input)
{
	VertexOutput output;
	
	float3 v = velocity;
	v.xz /= input.size.y * 0.1f;
	output.velocity = velocity;
	
	float3 displace = time * v;
	input.pos.xyz = origin + (size + (input.pos.xyz + displace) % size) % size - (size * 0.5f);
	
	output.distance = drawDistance;
	output.color = color;
	
	output.pos = input.pos.xyz;
	output.size = input.size;
	
	return output;
}

static const float2 TEXCOORD[4] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> output)
{
	float3 up = normalize(-input[0].velocity);
	float3 forward = input[0].pos - invView._41_42_43;
	forward = normalize(forward);
	float3 right = normalize(cross(up, forward));
	
	float2 halfSize = input[0].size * 0.5f;
	
	float4 vertices[4];
	vertices[0] = float4(input[0].pos - halfSize.x * right - halfSize.y * up, 1.0f);
	vertices[1] = float4(input[0].pos - halfSize.x * right + halfSize.y * up, 1.0f);
	vertices[2] = float4(input[0].pos + halfSize.x * right - halfSize.y * up, 1.0f);
	vertices[3] = float4(input[0].pos + halfSize.x * right + halfSize.y * up, 1.0f);

	PixelInput pixelInput;
	pixelInput.color = input[0].color;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		pixelInput.pos = mul(vertices[i], view);
		pixelInput.pos = mul(pixelInput.pos, projection);

		pixelInput.uv = TEXCOORD[i];
		
		pixelInput.alpha = saturate(1 - pixelInput.pos.z / input[0].distance) * 0.5f;
		
		output.Append(pixelInput);
	}
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 result = diffuseMap.Sample(samp, input.uv);
	
	result.a *= (1.0f - input.alpha) * 1.5f;
	
	return result * input.color;
}