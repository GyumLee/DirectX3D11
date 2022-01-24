#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

using namespace std;

struct XMFLOAT4
{
	float x;
	float y;
	float z;
	float w;

	XMFLOAT4() : x(0), y(0), z(0), w(0) {}
	XMFLOAT4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}
};

/* * * * * * * * *
 * ImGui Manual  *
 * * * * * * * * */

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>
#pragma comment(lib, "ImGui.lib")

#define DELTA ImGui::GetIO().DeltaTime

#define KEY_DOWN(k) ImGui::IsKeyPressed(k, false)
#define KEY_UP(k) ImGui::IsKeyReleased(k) 
#define KEY_PRESS(k) ImGui::IsKeyDown(k)

// ImGuiMouseButton Enum: A mouse button identifier (0=left, 1=right, 2=middle)
#define MOUSE_PRESS(k) ImGui::IsMouseDown(k)
#define MOUSE_CLICK(k) ImGui::IsMouseClicked(k)
#define MOUSE_DBCLICK(k) ImGui::IsMouseDoubleClicked(k)
#define MOUSE_UP(k) ImGui::IsMouseReleased(k)

class ProgramManual
{
private:
	HWND hWnd;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

public:
	ProgramManual()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_TitleBgActive] = ImColor(0, 100, 200, 100);

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Hack-Bold.ttf", 15.0f);

		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(device, deviceContext);
	}
	~ProgramManual()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void GUIRender();
};

void ProgramManual::GUIRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static bool isActive = true;

	if (isActive)
	{
		ImGui::Begin("Inspector", &isActive);
		//ImGuiManual::GUIRender();
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

class ImGuiManual
{
private:
	int num;
	XMFLOAT4 color;
	bool isActive;
	string tag;
	ID3D11ShaderResourceView* srv;
	int type;

public:
	void GUIRender();
	void Save() {}
	void Load() {}
};

void ImGuiManual::GUIRender()
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		ImVec2 delta = ImGui::GetIO().MouseDelta;
		float wheel = ImGui::GetIO().MouseWheel;

		wstring fps = L"FPS : " + to_wstring((int)ImGui::GetIO().Framerate);

		if (ImGui::TreeNode("label"))
		{
			ImGui::Checkbox("label", &isActive);

			char buf[128];
			strcpy_s(buf, sizeof(buf), tag.c_str());
			ImGui::InputText("label", buf, sizeof(buf));
			tag = buf;

			ImGui::TextColored(ImVec4(0, 0, 1, 1), "fmt");

			const char* List[] = { "1st type", "2nd type", "3rd type" };
			ImGui::Combo("Label", &type, List, sizeof(List) / sizeof(const char*));

			ImGui::DragFloat("label", (float*)&num, 0.1f, -100.0f, 100.0f, "%.3");
			ImGui::SliderFloat("label", (float*)&num, -100.0f, 100.0f, "%.3");

			ImGui::ColorEdit3("label", (float*)&color, ImGuiColorEditFlags_PickerHueWheel);

			ImTextureID textureID = srv;
			if (ImGui::ImageButton(textureID, ImVec2(50, 50))) Save();
			ImGui::SameLine();
			if (ImGui::Button("label")) Load();

			ImGui::Spacing();
			ImGui::NewLine();

			ImGui::TreePop();
		}


		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		if (ImGui::TreeNodeEx("str_id", flags, "fmt"))
		{

			ImGui::TreePop();
		}
	}
}