#include "GameClearScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include <SpriteManager.h>
#include <Object3DCommon.h>
#include <SkyBoxManager.h>
#include "SceneManager.h"

void GameClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();
}

void GameClearScene::Update()
{
	// 入力によるシーン切り替え
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(XButtons.A)) // Enterキーが押されたら
	{
		if (sceneManager_) {
			sceneManager_->ChangeScene("TitleScene"); // シーン名を指定して変更
		}

		wavLoader_->StopBGM();
	}
}

void GameClearScene::Draw()
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

void GameClearScene::Finalize()
{
}

void GameClearScene::DrawImGui()
{
}
