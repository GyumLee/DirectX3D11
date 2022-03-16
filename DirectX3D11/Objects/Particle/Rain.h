#pragma once

class Rain : public Particle
{
private:
	WeatherBuffer* buffer;
	VertexSize* vertices;

public:
	Rain();
	~Rain();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;

private:
	void Create();
};