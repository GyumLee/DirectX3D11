#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <assert.h>
#define V(hr) assert(SUCCEEDED(hr))

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define CENTER_X (WIN_WIDTH * 0.5f)
#define CENTER_Y (WIN_HEIGHT * 0.5f)

#define MAX_BONE 1024
#define MAX_FRAME 1024
#define MAX_LIGHT 10
#define MAX_INSTANCE 100

#define LERP(s, e, t) (s + (e - s) * t)

#define DELTA ImGui::GetIO().DeltaTime

#define KEY_DOWN(k) ImGui::IsKeyPressed(k, false)
#define KEY_UP(k) ImGui::IsKeyReleased(k)
#define KEY_PRESS(k) ImGui::IsKeyDown(k)

#define MOUSE_PRESS(k) ImGui::IsMouseDown(k)
#define MOUSE_CLICK(k) ImGui::IsMouseClicked(k)
#define MOUSE_DBCLICK(k) ImGui::IsMouseDoubleClicked(k)
#define MOUSE_UP(k) ImGui::IsMouseReleased(k)

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define CAM Environment::Get()->GetMainCamera()
#define FRUSTUM Environment::Get()->GetFrustum()

#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <functional>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>
#pragma comment(lib, "ImGui.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

//DirectWrite
#include <d2d1_2.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#pragma comment(lib, "assimp-vc142-mtd.lib")

using namespace std;
using namespace DirectX;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;

typedef function<void()> Event;

//Framework Header
#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"
#include "Framework/Utility/Font.h"
using namespace Utility;

#include "Framework/System/Device.h"
#include "Framework/System/DepthStencil.h"
#include "Framework/System/RenderTarget.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"
#include "Framework/Shader/HullShader.h"
#include "Framework/Shader/DomainShader.h"

#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/RawBuffer.h"
#include "Framework/Buffer/StructuredBuffer.h"

#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/GameMath.h"
using namespace GameMath;

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"
#include "Framework/Collision/BoxCollider.h"

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Frustum.h"
#include "Framework/Environment/Environment.h"
#include "Framework/Environment/ETC.h"
#include "Framework/Environment/Reflection.h"
#include "Framework/Environment/Refraction.h"
#include "Framework/Environment/Shadow.h"

#include "Framework/Model/ModelType.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelReader.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelBone.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

//Algorithm
#include "Algorithm/Node.h"
#include "Algorithm/Heap.h"
#include "Algorithm/AStar.h"

//Object Header
#include "Objects/Basic/RenderTransform.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"

#include "Objects/UI/ProgressBar.h"
#include "Objects/UI/RenderTexture.h"

#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/SkyBox.h"
#include "Objects/Landscape/Water.h"
#include "Objects/Landscape/TerrainData.h"
#include "Objects/Landscape/QuadTreeTerrain.h"
#include "Objects/Landscape/TerrainLOD.h"
#include "Objects/Landscape/Scattering.h"

#include "Objects/Model/Ninja.h"
#include "Objects/Model/Sword.h"
#include "Objects/Model/Hinata.h"
#include "Objects/Model/Bullet.h"
#include "Objects/Model/Character.h"
#include "Objects/Model/Obstacle.h"

#include "Objects/Particle/ParticleSystem.h"
#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/Rain.h"
#include "Objects/Particle/Snow.h"
#include "Objects/Particle/Trail.h"

#include "Objects/Manager/BulletManager.h"
#include "Objects/Manager/MonsterManager.h"

#include "Objects/Deferred/GBuffer.h"

//Program
#include "Scenes/Scene.h"
#include "Program/SceneManager.h"
#include "Program/Program.h"

extern HWND hWnd;
extern Vector3 mousePos;