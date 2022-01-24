#pragma once

class TerrainEditor : public Transform
{
private:
	const float MIN_HEIGHT = 0.0f;
	const float MAX_HEIGHT = 30.0f;

	struct InputDesc
	{
		UINT index;
		Float3 v0, v1, v2;
	};
	struct OutputDesc
	{
		int picked;
		float distance;
	};

	struct VertexType
	{
		Float3 position;
		Float2 uv;
		Float3 normal = Float3(0, 0, 0);
		Float3 tangent = Float3(0, 0, 0);
		float alpha[4] = {};
	};

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	WorldBuffer* worldBuffer;
	BrushBuffer* brushBuffer;

	Texture* heightMap;

	Texture* secondMap;
	Texture* thirdMap;
	UINT selectMap;

	float adjustValue;

	int editType;

	ComputeShader* computeShader;
	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;
	InputDesc* input;
	OutputDesc* output;

	UINT size;

public:
	TerrainEditor(UINT width = 100, UINT height = 100);
	~TerrainEditor();

	void Update();
	void Render();
	void GUIRender();

	Vector3 Picking();
	Vector3 ComputePicking();

	Material* GetMaterial() { return material; }

private:
	void AdjustHeight();
	void AdjustAlpha();

	void CreateMesh();
	void CreateNormal();
	void CreateTangent();
	void CreateComputeData();

	void InitNormalTangent();

	void SaveHeightMap();
	void LoadHeightMap();

	void SaveAlphaMap();
	void LoadAlphaMap();
};