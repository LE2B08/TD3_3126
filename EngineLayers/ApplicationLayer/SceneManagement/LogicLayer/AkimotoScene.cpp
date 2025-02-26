#include "AkimotoScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"
#include "Object3DCommon.h"

void AkimotoScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
}

void AkimotoScene::Update()
{
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_1))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_3))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("SatouScene");
		}
	}

	enemy_->Update();

	enemy_->ShowImGui("Enemy");
}

void AkimotoScene::Draw()
{
	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	enemy_->Draw();
}

void AkimotoScene::Finalize()
{
}

void AkimotoScene::DrawImGui()
{
}
