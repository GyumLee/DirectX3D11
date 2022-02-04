#pragma once

class CollisionScene : public Scene
{
private:
	vector<Collider*> colliders;

public:
	CollisionScene();
	~CollisionScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};