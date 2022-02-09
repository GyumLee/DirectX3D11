#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	ModelExporter* exporter;
	string name = "Ninja";
	string file;
	string clip;

	//file = "ModelData/FBX/" + name + ".obj";
	//exporter = new ModelExporter(name, file);
	//exporter->ExportMaterial();
	//exporter->ExportMesh();
	//delete exporter;

	clip = "Hit";
	file = "ModelData/Animations/" + name + "/" + clip + ".fbx";
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clip);
	delete exporter;
	
	clip = "Dying";
	file = "ModelData/Animations/" + name + "/" + clip + ".fbx";
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clip);
	delete exporter;
	//
	//clip = "Fire";
	//file = "ModelData/Animations/" + name + "/" + clip + ".fbx";
	//exporter = new ModelExporter(name, file);
	//exporter->ExportClip(clip);
	//delete exporter;
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
