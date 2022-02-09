#include "Framework.h"

#include "Scenes/TutorialScene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/PlaneScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimatorRenderScene.h"
#include "Scenes/CharacterScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/ShootingScene.h"

Program::Program()
{
	Create();

	//SceneManager::Get()->Create("Start", new CubeScene());
	//SceneManager::Get()->Create("Start", new PlaneScene());
	SceneManager::Get()->Create("Grid", new GridScene());
	//SceneManager::Get()->Create("Start", new TerrainScene());
	//SceneManager::Get()->Create("Start", new TerrainEditorScene());
	//SceneManager::Get()->Create("ModelExport", new ModelExportScene());
	//SceneManager::Get()->Create("Start", new ModelRenderScene());
	//SceneManager::Get()->Create("Start", new ModelAnimatorRenderScene());
	//SceneManager::Get()->Create("Start", new CharacterScene());
	//SceneManager::Get()->Create("Start", new CollisionScene());
	SceneManager::Get()->Create("Start", new ShootingScene());

	SceneManager::Get()->Add("Grid");
	SceneManager::Get()->Add("Start");
}

Program::~Program()
{
	Delete();
}

void Program::Update()
{
	SceneManager::Get()->Update();

	CAM->Update();

	ETC::Get()->Update();
}

void Program::PreRender()
{
	SceneManager::Get()->PreRender();
}

void Program::Render()
{
	Environment::Get()->SetRender();

	Device::Get()->SetRenderTarget();

	Environment::Get()->Render();
	SceneManager::Get()->Render();
}

void Program::PostRender()
{
	Environment::Get()->SetPostRender();

	Font::Get()->GetDC()->BeginDraw();

	wstring fps = L"FPS : " + to_wstring((int)ImGui::GetIO().Framerate);
	Font::Get()->RenderText(fps, "default", Float2(60, 10));

	SceneManager::Get()->PostRender();

	Font::Get()->GetDC()->EndDraw();
}

void Program::GUIRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool isActive = true;

	if (isActive)
	{
		ImGui::Begin("Inspector", &isActive);
		Environment::Get()->GUIRender();
		SceneManager::Get()->GUIRender();
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Program::Create()
{
	Device::Get();
	Font::Get();
	Environment::Get();
	SceneManager::Get();
	ETC::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(0, 100, 200, 100);

	ImGuiIO& io = ImGui::GetIO();
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Hack-Bold.ttf", 15.0f);

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);

	Font::Get()->Add("default", L"Consolas");
}

void Program::Delete()
{
	Device::Delete();
	Shader::Delete();
	Font::Delete();
	Environment::Delete();
	SceneManager::Delete();
	ETC::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
