#include "TuboScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"

#include "Object3DCommon.h"

void TuboScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// Playerクラスの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();

	//camera

}

void TuboScene::Update()
{
#pragma region SceneChange
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
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

	if (input_->TriggerKey(DIK_3))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("SatouScene");
		}
	}
#pragma endregion

	player_->Update();
}

void TuboScene::Draw()
{ 
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	player_->Draw();
}

void TuboScene::Finalize()
{ 
	player_->Finalize(); 
}

void TuboScene::DrawImGui()
{ 
	player_->DrawImGui(); 
}
