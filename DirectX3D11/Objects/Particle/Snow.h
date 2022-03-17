#pragma once

class Snow : public Particle
{
private:
	struct VertexSnow
	{
		Float3 position;
		Float2 size;
		Float2 random;
	};

	WeatherBuffer* buffer;
	VertexSnow* vertices;

public:
	Snow();
	~Snow();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

private:
	void Create();
};