#pragma once

class RenderTargetScene : public Scene
{
private:
	SkyBox* sky;
	Terrain* terrain;
	Hinata* hinata;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	RenderTexture* renderTexture;
	RenderTexture* renderTexture2;

public:
	RenderTargetScene();
	~RenderTargetScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateModels();
};