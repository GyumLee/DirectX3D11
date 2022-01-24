#include "Framework.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (pair<string, Scene*> scene : scenes)
		delete scene.second;
}

void SceneManager::Update()
{
	for (Scene* scene : curScenes)
		scene->Update();
}

void SceneManager::PreRender()
{
	for (Scene* scene : curScenes)
		scene->PreRender();
}

void SceneManager::Render()
{
	for (Scene* scene : curScenes)
		scene->Render();
}

void SceneManager::PostRender()
{
	for (Scene* scene : curScenes)
		scene->PostRender();
}

void SceneManager::GUIRender()
{
	for (Scene* curScene : curScenes)
	{
		for (auto scene : scenes)
		{
			if (scene.second == curScene)
			{
				string key = scene.first + " Scene Component";
				ImGui::TextColored(ImVec4(0, 0.8f, 0.8f, 1), key.c_str());
				ImGui::Spacing();
			}
		}
		curScene->GUIRender();
		ImGui::Spacing(); ImGui::Spacing();
	}
}

Scene* SceneManager::Create(string key, Scene* scene)
{
	if (scenes.count(key) > 0)
		return scenes[key];

	scenes[key] = scene;

	return scene;
}

Scene* SceneManager::Add(string key)
{
	if (scenes.count(key) == 0) return nullptr;

	for (Scene*	scene : curScenes)
	{
		if (scene == scenes[key])
			return scenes[key];
	}

	curScenes.push_back(scenes[key]);
	scenes[key]->Start();

	return scenes[key];
}

void SceneManager::Remove(string key)
{
	if (scenes.count(key) == 0) return;

	for (UINT i = 0; i < curScenes.size(); i++)
	{
		if (curScenes[i] == scenes[key])
		{
			curScenes[i]->End();
			curScenes.erase(curScenes.begin() + i);
			return;
		}
	}
}
