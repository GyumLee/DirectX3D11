#pragma once

class Spark : public Particle
{
private:
	struct VertexParticle
	{
		Float3 position = { 0,0,0 };
		Float2 size = { 1,1 };
		Float3 velocity = { 0,0,0 };
	};

	VertexParticle* vertices;

	ValueBuffer* sparkBuffer;
	ColorBuffer* startColorBuffer;
	ColorBuffer* endColorBuffer;

	float minRadius = 5.0f;
	float maxRadius = 10.0f;

	float minSize = 0.1f;
	float maxSize = 1.0f;

public:
	Spark(string imageFile, bool isAdditive);
	~Spark();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

	virtual void Play(Vector3 position) override;

private:
	void Create();
	void UpdateParticle();
};