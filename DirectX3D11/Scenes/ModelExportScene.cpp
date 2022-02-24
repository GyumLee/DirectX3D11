#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	ModelExporter* exporter;
	string name = "Character";
	string file;
	string clip;

	//file = "ModelData/FBX/" + name + ".fbx";
	//exporter = new ModelExporter(name, file);
	//exporter->ExportMaterial();
	//exporter->ExportMesh();
	//delete exporter;

	clip = "idle";
	file = "ModelData/Animations/" + name + "/" + clip + ".fbx";
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clip);
	delete exporter;
	
	clip = "jump";
	file = "ModelData/Animations/" + name + "/" + clip + ".fbx";
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clip);
	delete exporter;
	
	clip = "run";
	file = "ModelData/Animations/" + name + "/" + clip + ".fbx";
	exporter = new ModelExporter(name, file);
	exporter->ExportClip(clip);
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
