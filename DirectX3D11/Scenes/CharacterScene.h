#pragma once

class CharacterScene : public Scene
{
private:
	Ninja* ninja;

public:
	CharacterScene();
	~CharacterScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};