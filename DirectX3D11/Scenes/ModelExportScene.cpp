#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	ModelExporter* exporter;
	string name = "Ninja";
	string file = "ModelData/FBX/" + name + ".fbx";

	exporter = new ModelExporter(name, file);
	exporter->ExportMaterial();

	delete exporter;
}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
