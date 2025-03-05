#include "SatouScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"
#include "Object3DCommon.h"
#include <Wireframe.h>
#include "Camera.h"
#include <CameraManager.h>
#ifdef _DEBUG
#include <DebugCamera.h>
#endif // _DEBUG


void SatouScene::Initialize()
{
	cameraTransform_.Initialize();
	cameraTransform_.scale_ = { 1.0f,1.0f,1.0f };
	cameraTransform_.rotate_ = { 0.3f,0.0f,0.0f };
	cameraTransform_.translate_ = { 0.0f,15.0f,-50.0f };
	
	CameraManager::GetInstance()->GetCamera("DefaultCamera")->SetWorldTransform(cameraTransform_);
	
#ifdef _DEBUG
	// デバッグカメラの初期化
	DebugCamera::GetInstance()->Initialize();
#endif // _DEBUG
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// terrainの生成と初期化
	objectTerrain_ = std::make_unique<Object3D>();
	objectTerrain_->Initialize("terrain.obj");

	objectBall_ = std::make_unique<Object3D>();
	objectBall_->Initialize("sphere.gltf");
}

void SatouScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F12))
	{
		Object3DCommon::GetInstance()->SetDebugCamera(!Object3DCommon::GetInstance()->GetDebugCamera());
		Wireframe::GetInstance()->SetDebugCamera(!Wireframe::GetInstance()->GetDebugCamera());
		isDebugCamera_ = !isDebugCamera_;
	}
#endif // _DEBUG
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
	}

	if (input_->TriggerKey(DIK_1))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	// シーン切り替え（必要なら）
	if (input_->TriggerKey(DIK_2))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}
	// オブジェクトの更新処理
	objectTerrain_->Update();
	objectBall_->Update();

	/*------カメラの更新------*/
	CameraManager::GetInstance()->GetCamera("DefaultCamera")->SetWorldTransform(cameraTransform_);
}

void SatouScene::Draw()
{
	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// Terrain.obj の描画
	objectTerrain_->Draw();
	objectBall_->Draw();

	// ワイヤーフレームの描画
	Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, { 0.25f, 0.25f, 0.25f,1.0f });

	// 衝突判定と応答
	//CheckAllCollisions();

	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///



}

void SatouScene::Finalize()
{
}

void SatouScene::DrawImGui()
{
	ImGui::Begin("SatouScene");
	ImGui::SliderFloat3("Translate", &cameraTransform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotate", &cameraTransform_.rotate_.x, -3.14f, 3.14f);
#ifdef _DEBUG
	DebugCamera::GetInstance()->DrawImGui();
#endif // _DEBUG
	ImGui::End();
}
