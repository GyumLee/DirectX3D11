#include "Framework.h"

Cursor::Cursor()
{
	ShowCursor(false);

	cursor = new Quad(50, 50);
	cursor->GetMaterial()->SetDiffuseMap("Textures/UI/Cursor.png");
}

Cursor::~Cursor()
{
	delete cursor;
}

void Cursor::Update()
{
	cursor->position.x = mousePos.x;
	cursor->position.y = WIN_HEIGHT - mousePos.y;

	cursor->UpdateWorld();
}

void Cursor::PostRender()
{
	cursor->Render();
}

void Cursor::HideCursor(bool bHide)
{
	cursor->isActive = !bHide;
}
