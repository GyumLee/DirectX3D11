
#define MAX_BONE 1024

//VertexLayout
struct Vertex
{
	float4 pos : POSITION;
};

struct VertexColor
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct VertexUV
{
	float4 pos : POSITION;
	float2 uv : UV;
};

struct VertexUVNormal
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct VertexUVNormalTangentBlend
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 indices : BLENDINDICES;
	float4 weights : BLENDWEIGHTS;
};

//VertexShader ConstBuffer
cbuffer WorldBuffer : register(b0)
{
	matrix world;
}

cbuffer ViewBuffer : register(b1)
{
	matrix view;
	matrix invView;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix projection;
}

struct Frame
{
	int clip;
	uint curFrame;
	float time;
	float speed;
};

struct Motion
{
	float takeTime;
	float tweenTime;
	float runningTime;
	float padding;
	
	Frame cur;
	Frame next;
};

cbuffer FrameBuffer : register(b3)
{
	Motion motion;
}

Texture2DArray transformMap : register(t0);

//PixelShader ConstBuffer
SamplerState samp : register(s0);

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

struct Light
{
	float4 color;
	float3 direction;
};

cbuffer LightBuffer : register(b0)
{
	Light light;

	float padding;
	
	float4 ambientLight;
}

cbuffer MaterialBuffer : register(b1)
{
	float4 mDiffuse;
	float4 mSpecular;
	float4 mAmbient;
	float4 mEmissive;
	
	float shininess;
	
	int hasDiffuseMap;
	int hasSpecularMap;
	int hasNormalMap;
}

//VertexShader Function
matrix SkinWorld(float4 indices, float4 weights)
{
	matrix transform = 0;
	matrix cur, next;
	matrix curAnim, nextAnim;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	for (int i = 0; i < 4; i++)
	{
		int curFrame = motion.cur.curFrame;
		int clip = motion.cur.clip;
		float time = motion.cur.time;
		
		c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
		c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
		c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
		c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		
		cur = matrix(c0, c1, c2, c3);
		
		n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
		n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
		n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
		n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
		
		next = matrix(n0, n1, n2, n3);
		
		curAnim = lerp(cur, next, time);
		
		curFrame = motion.next.curFrame;
		clip = motion.next.clip;
		time = motion.next.time;
		
		[flatten]
		if(clip > -1)
		{
			c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
			c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
			c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
			c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
		
			cur = matrix(c0, c1, c2, c3);
		
			n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0));
			n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0));
			n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0));
			n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0));
			
			next = matrix(n0, n1, n2, n3);
			
			nextAnim = lerp(cur, next, time);
			
			curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
		}
		
		transform += mul(weights[i], curAnim);
	}
	
	return transform;
}