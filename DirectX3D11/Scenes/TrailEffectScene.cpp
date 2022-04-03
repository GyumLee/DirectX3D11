#include "Framework.h"
#include "TrailEffectScene.h"

TrailEffectScene::TrailEffectScene()
{
    start.tag = "Start";
    end.tag = "End";

    start.position = { 0, 10, 0 };
    end.position = { 0, 5, 0 };

    trail = new Trail("Textures/UI/Cursor.png", &start.position, &end.position, 10);
}

TrailEffectScene::~TrailEffectScene()
{
    delete trail;
}

void TrailEffectScene::Update()
{
    trail->Update();
}

void TrailEffectScene::PreRender()
{
}

void TrailEffectScene::Render()
{
    trail->Render();
}

void TrailEffectScene::PostRender()
{
}

void TrailEffectScene::GUIRender()
{
    start.GUIRender();
    end.GUIRender();
    trail->GUIRender();
}
