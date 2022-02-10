#include "Framework.h"

ProgressBar::ProgressBar(string frontImage, string backImage, float maxValue)
{
	valueBuffer = new ValueBuffer();
	valueBuffer->values[0] = maxValue;
	valueBuffer->values[1] = maxValue;
	valueBuffer->values[2] = maxValue;

	Texture* texture = Texture::Add(frontImage);

	frontBar = new Quad(texture->Width(), texture->Height());
	frontBar->GetMaterial()->SetShader(L"UI/ProgressBar.hlsl");
	frontBar->GetMaterial()->SetDiffuseMap(frontImage);
	frontBar->SetParent(this);

	texture = Texture::Add(backImage);

	backBar = new Quad(texture->Width(), texture->Height());
	backBar->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	backBar->GetMaterial()->SetDiffuseMap(backImage);
	backBar->SetParent(this);
}

ProgressBar::~ProgressBar()
{
	delete valueBuffer;

	delete frontBar;
	delete backBar;
}

void ProgressBar::Update()
{
	UpdateWorld();

	frontBar->UpdateWorld();
	backBar->UpdateWorld();
}

void ProgressBar::Render()
{
	backBar->Render();
	valueBuffer->SetPSBuffer(10);
	frontBar->Render();
}
