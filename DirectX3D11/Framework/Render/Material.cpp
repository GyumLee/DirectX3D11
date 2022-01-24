#include "Framework.h"

Material::Material(string name)
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr),
	name(name)
{
	buffer = new MaterialBuffer();
}

Material::~Material()
{
	delete buffer;
}

void Material::Set()
{
	buffer->SetPSBuffer(1);

	if (diffuseMap != nullptr)
		diffuseMap->Set(0);

	if (specularMap != nullptr)
		specularMap->Set(1);

	if (normalMap != nullptr)
		normalMap->Set(2);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::GUIRender()
{
	if (ImGui::TreeNode((name + "_Material").c_str()))
	{
		char str[128];
		strcpy_s(str, sizeof(str), editName.c_str());
		ImGui::InputText("Name", str, sizeof(str));
		editName = str;

		ImGui::SameLine();
		if (ImGui::Button("Edit")) name = editName;

		ImGui::ColorEdit3("Diffuse", (float*)&buffer->data.diffuse, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorEdit3("Specular", (float*)&buffer->data.specular, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorEdit3("Ambient", (float*)&buffer->data.ambient, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::ColorEdit4("Emissive", (float*)&buffer->data.emissive, ImGuiColorEditFlags_PickerHueWheel);

		ImGui::DragFloat("Shininess", &buffer->data.shininess, 0.1f, 1.0f, 50.0f);

		for (int i = 0; i < 3; i++)
		{
			SelectMap(i);
			if (i < 3 - 1) ImGui::SameLine();
		}

		SaveDialog();
		ImGui::SameLine();
		LoadDialog();

		ImGui::TreePop();
	}
}

void Material::SetShader(wstring file)
{
	vertexShader = Shader::AddVS(file);
	pixelShader = Shader::AddPS(file);
}

void Material::SetDiffuseMap(string file)
{
	if (ExistFile(file))
	{
		diffuseMap = Texture::Add(file);
		buffer->data.hasDiffuseMap = 1;
	}
	else
	{
		diffuseMap = nullptr;
		buffer->data.hasDiffuseMap = 0;
	}
}

void Material::SetSpecularMap(string file)
{
	if (ExistFile(file))
	{
		specularMap = Texture::Add(file);
		buffer->data.hasSpecularMap = 1;
	}
	else
	{
		specularMap = nullptr;
		buffer->data.hasSpecularMap = 0;
	}
}

void Material::SetNormalMap(string file)
{
	if (ExistFile(file))
	{
		normalMap = Texture::Add(file);
		buffer->data.hasNormalMap = 1;
	}
	else
	{
		normalMap = nullptr;
		buffer->data.hasNormalMap = 0;
	}
}

void Material::Save(string file)
{
	BinaryWriter w(file);

	w.String(name);

	if (diffuseMap) w.String(diffuseMap->GetFile());
	else w.String("");
	if (specularMap) w.String(specularMap->GetFile());
	else w.String("");
	if (normalMap) w.String(normalMap->GetFile());
	else w.String("");

	w.XMFLOAT4(buffer->data.diffuse);
	w.XMFLOAT4(buffer->data.specular);
	w.XMFLOAT4(buffer->data.ambient);
	w.XMFLOAT4(buffer->data.emissive);

	w.Float(buffer->data.shininess);
}

void Material::Load(string file)
{
	BinaryReader r(file);

	if (r.IsFailed()) return;

	this->file = file;

	name = r.String();

	SetDiffuseMap(r.String());
	SetSpecularMap(r.String());
	SetNormalMap(r.String());

	buffer->data.diffuse = r.XMFLOAT4();
	buffer->data.specular = r.XMFLOAT4();
	buffer->data.ambient = r.XMFLOAT4();
	buffer->data.emissive = r.XMFLOAT4();

	buffer->data.shininess = r.Float();
}

void Material::SelectMap(int type)
{
	string key = this->name + to_string(type);
	ImTextureID textureID = Texture::Add("Textures/UI/none.png", key)->GetSRV();

	switch (type)
	{
	case 0:
		if (diffuseMap)
			textureID = diffuseMap->GetSRV();
		break;
	case 1:
		if (specularMap)
			textureID = specularMap->GetSRV();
		break;
	case 2:
		if (normalMap)
			textureID = normalMap->GetSRV();
		break;
	}

	if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
	{
		ImGuiFileDialog::Instance()->OpenDialog(key, key,
			".png,.jpg,.tga,.dds", ".");
	}

	if (ImGuiFileDialog::Instance()->Display(key))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			switch (type)
			{
			case 0:
				SetDiffuseMap(file);
				break;
			case 1:
				SetSpecularMap(file);
				break;
			case 2:
				SetNormalMap(file);
				break;
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void Material::SaveDialog()
{
	if (ImGui::Button("Save"))
	{
		if (file.empty())
			file = "TextData/Materials/" + name + ".mat";

		Save(file);
	}

	ImGui::SameLine();

	if (ImGui::Button("SaveAs"))
	{
		ImGuiFileDialog::Instance()->OpenDialog(name + "_SaveMaterial", name + "_SaveMaterial",
			".mat", ".");
	}

	if (ImGuiFileDialog::Instance()->Display(name + "_SaveMaterial"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			Save(file);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void Material::LoadDialog()
{
	if (ImGui::Button("Load"))
	{
		ImGuiFileDialog::Instance()->OpenDialog(name + "_LoadMaterial", name + "_LoadMaterial",
			".mat", ".");
	}

	if (ImGuiFileDialog::Instance()->Display(name + "_LoadMaterial"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();

			char path[128];
			GetCurrentDirectoryA(sizeof(path), path);

			file = file.substr(strlen(path) + 1, file.length());

			Load(file);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}
