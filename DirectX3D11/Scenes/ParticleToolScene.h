#pragma once

class ParticleToolScene : public Scene
{
private:
	ParticleSystem* particleSystem;

public:
	ParticleToolScene();
	~ParticleToolScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};