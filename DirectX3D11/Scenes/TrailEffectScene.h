#pragma once

class TrailEffectScene : public Scene
{
private:
	Transform start;
	Transform end;

	Trail* trail;

public:
	TrailEffectScene();
	~TrailEffectScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};