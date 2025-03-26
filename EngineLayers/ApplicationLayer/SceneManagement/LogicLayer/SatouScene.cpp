#include "SatouScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"
#include <SpriteManager.h>
#include <Object3DCommon.h>
#include <SkyBoxManager.h>


void SatouScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();



}

void SatouScene::Update()
{
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
	}

	if (input_->TriggerKey(DIK_F1))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	// シーン切り替え（必要なら）
	if (input_->TriggerKey(DIK_F2))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}
}

void SatouScene::Draw()
{
	/// ------------------------------------------ ///
	/// ---------- スカイボックスの描画 ---------- ///
	/// ------------------------------------------ ///
	SkyBoxManager::GetInstance()->SetRenderSetting();




	/// ---------------------------------------- ///
	/// ----------  スプライトの描画  ---------- ///
	/// ---------------------------------------- ///
	// スプライトの共通描画設定
	SpriteManager::GetInstance()->SetRenderSetting();



	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();




}

void SatouScene::Finalize()
{
}

void SatouScene::DrawImGui()
{
}
