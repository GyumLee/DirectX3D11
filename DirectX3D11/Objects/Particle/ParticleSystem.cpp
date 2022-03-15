#include "Framework.h"

ParticleSystem::ParticleSystem()
{
	quad = new Quad();
	quad->GetMaterial()->SetShader(L"Particle/ParticleSystem.hlsl");
	quad->GetMaterial()->SetDiffuseMap("Textures/Particle/star.png");

	transforms.resize(MAX_COUNT);
	quads.resize(MAX_COUNT);

	instanceBuffer = new VertexBuffer(transforms.data(), sizeof(Matrix), MAX_COUNT);
	spriteBuffer = new SpriteBuffer();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Additive();

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

ParticleSystem::~ParticleSystem()
{
	delete quad;
	delete instanceBuffer;
	delete spriteBuffer;

	delete blendState[0];
	delete blendState[1];
	delete depthState[0];
	delete depthState[1];
}

void ParticleSystem::Update()
{
	if (!quad->isActive) return;

	time += DELTA;

	UpdatePhysical();
	UpdateColor();
	UpdateSprite();

	quad->UpdateWorld();

	if (time > particleData.duration)
	{
		if (particleData.isLoop)
			Init();
		else
			Stop();
	}
}

void ParticleSystem::Render()
{
	if (!quad->isActive) return;

	instanceBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 1);

	spriteBuffer->SetPSBuffer(10);

	quad->SetRender();

	blendState[1]->SetState();
	depthState[1]->SetState();

	DC->DrawIndexedInstanced(6, drawCount, 0, 0, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void ParticleSystem::GUIRender()
{
	ImGui::Text("Particle Option");
	ImGui::Checkbox("Loop", &particleData.isLoop);
	ImGui::Checkbox("Additive", &particleData.isAdditive);
	ImGui::Checkbox("Billboard", &particleData.isBillboard);
	ImGui::SliderInt("Count", (int*)&particleData.count, 0, MAX_COUNT);
	ImGui::SliderFloat("Duration", &particleData.duration, 0, 10.0f);
	ImGui::DragFloat3("MinVelocity", (float*)&particleData.minVelocity, 0.1f);
	ImGui::DragFloat3("MaxVelocity", (float*)&particleData.maxVelocity, 0.1f);
	ImGui::DragFloat3("MinAccelation", (float*)&particleData.minAccelation, 0.1f);
	ImGui::DragFloat3("MaxAccelation", (float*)&particleData.maxAccelation, 0.1f);
	ImGui::DragFloat3("MinScale", (float*)&particleData.minScale, 0.1f);
	ImGui::DragFloat3("MaxScale", (float*)&particleData.maxScale, 0.1f);
	ImGui::SliderFloat("MinStartTime", &particleData.minStartTime, 0, particleData.duration);
	ImGui::SliderFloat("MaxStartTime", &particleData.maxStartTime, 0, particleData.duration);
	ImGui::SliderFloat("MinSpeed", &particleData.minSpeed, 0, 10.0f);
	ImGui::SliderFloat("MaxSpeed", &particleData.maxSpeed, 0, 10.0f);
	ImGui::SliderFloat("MinAngularVelocity", &particleData.minAngularVelocity, -10.0f, 10.0f);
	ImGui::SliderFloat("MaxAngularVelocity", &particleData.maxAngularVelocity, -10.0f, 10.0f);
	ImGui::SliderFloat("WorldRotationY", &particleData.worldRotationY, -XM_PI, XM_PI);

	ImGui::ColorEdit4("StartColor", (float*)&particleData.startColor);
	ImGui::ColorEdit4("EndColor", (float*)&particleData.endColor);

	ImGui::DragFloat2("Tiling", (float*)&particleData.tiling, 1.0f);
	ImGui::DragFloat("TilingSpeed", &particleData.spriteSpeed, 0.1f);

	quad->GetMaterial()->GUIRender();
}

void ParticleSystem::Play(Vector3 pos, Vector3 rot)
{
	quad->isActive = true;
	quad->position = pos;
	quad->rotation = rot;

	Init();
}

void ParticleSystem::Stop()
{
	quad->isActive = false;
}

void ParticleSystem::Save(string file)
{
	CreateFolders(file);

	BinaryWriter w(file);

	string diffuseFile = quad->GetMaterial()->GetDiffuseMap();
	w.String(diffuseFile);

	w.Byte(&particleData, sizeof(ParticleData));
}

void ParticleSystem::Load(string file)
{
	BinaryReader r(file);

	string diffuseFile = r.String();
	quad->GetMaterial()->SetDiffuseMap(diffuseFile);

	void* data = &particleData;
	r.Byte(&data, sizeof(ParticleData));
}

void ParticleSystem::UpdatePhysical()
{
	drawCount = 0;

	quad->rotation.y += particleData.worldRotationY * DELTA;

	for (UINT i = 0; i < particleData.count; i++)
	{
		if (quads[i].startTime > time) continue;

		quads[i].velocity += quads[i].accelation * DELTA;
		quads[i].transform.position += quads[i].velocity * quads[i].speed * DELTA;
		quads[i].transform.rotation.z += quads[i].angularVelocity * DELTA;
		if (particleData.isBillboard)
		{
			quads[i].transform.rotation.x = CAM->rotation.x;
			quads[i].transform.rotation.y = CAM->rotation.y - quad->rotation.y;
		}
		quads[i].transform.UpdateWorld();
		transforms[drawCount] = XMMatrixTranspose(quads[i].transform.GetWorld());

		drawCount++;
	}

	instanceBuffer->Update(transforms.data(), drawCount);
}

void ParticleSystem::UpdateColor()
{
	float t = time / particleData.duration;

	Float4 color;
	color.x = LERP(particleData.startColor.x, particleData.endColor.x, t);
	color.y = LERP(particleData.startColor.y, particleData.endColor.y, t);
	color.z = LERP(particleData.startColor.z, particleData.endColor.z, t);
	color.w = LERP(particleData.startColor.w, particleData.endColor.w, t);

	quad->GetMaterial()->GetData().diffuse = color;
}

void ParticleSystem::UpdateSprite()
{
	if (particleData.tiling.x == 1 && particleData.tiling.y == 1)
		return;

	spriteTime += particleData.spriteSpeed * DELTA;

	if (spriteTime > 0.1f)
	{
		spriteBuffer->data.curFrame.x++;

		if (spriteBuffer->data.curFrame.x >= particleData.tiling.x)
		{
			spriteBuffer->data.curFrame.x = 0;
			spriteBuffer->data.curFrame.y++;
			if (spriteBuffer->data.curFrame.y >= particleData.tiling.y)
				spriteBuffer->data.curFrame.y = 0;
		}

		spriteTime = 0.0f;
	}

	spriteBuffer->data.maxFrame = particleData.tiling;
}

void ParticleSystem::Init()
{
	if (particleData.isAdditive)
		blendState[1]->Additive();
	else
		blendState[1]->Alpha(true);

	time = 0.0f;
	spriteTime = 0.0f;
	drawCount = 0;

	spriteBuffer->data.curFrame = { 0, 0 };

	for (UINT i = 0; i < particleData.count; i++)
	{
		quads[i].transform = Transform();
		quads[i].transform.scale = Random(particleData.minScale, particleData.maxScale);

		quads[i].velocity = Random(particleData.minVelocity, particleData.maxVelocity);
		quads[i].velocity = quads[i].velocity.Normalize();

		quads[i].accelation = Random(particleData.minAccelation, particleData.maxAccelation);
		quads[i].startTime = Random(particleData.minStartTime, particleData.maxStartTime);
		quads[i].speed = Random(particleData.minSpeed, particleData.maxSpeed);
		quads[i].angularVelocity = Random(particleData.minAngularVelocity, particleData.maxAngularVelocity);
	}
}
