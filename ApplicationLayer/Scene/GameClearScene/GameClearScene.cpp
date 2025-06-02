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
#include <AudioManager.h>


/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void GameClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input = Input::GetInstance();

	AudioManager::GetInstance()->PlayBGM("gameclear.mp3", 0.4f);

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	// カメラの初期化
	camera_->SetRotate({ 1.57f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,50.0f, 0.0f });

	// フェードマネージャーの初期化
	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();
	// シーン開始時に白からフェードアウトする（白 → 透明）
	fadeManager_->StartFadeFromWhite(0.02f);

	gameClearLogo_ = std::make_unique<GameClearLogo>();
	gameClearLogo_->Initialize();
}


/// -------------------------------------------------------------
///						　	更新処理
/// -------------------------------------------------------------
void GameClearScene::Update()
{
	// 入力によるシーン切り替え
	if (input->TriggerButton(XButtons.A)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			fadeManager_->StartFadeToWhite(0.02f, [this]() {
				// フェード完了後の処理
				sceneManager_->ChangeScene("TitleScene"); // シーン名を指定して変更
				});
		}
	}

	// カメラの更新処理
	camera_->Update();

	// フェードマネージャーの更新処理
	fadeManager_->Update();

	// ゲームクリアロゴの更新
	gameClearLogo_->Update();
}


/// -------------------------------------------------------------
///						　	描画処理
/// -------------------------------------------------------------
void GameClearScene::Draw()
{
#pragma region スカイボックスの描画

	// スカイボックスの描画設定
	SkyBoxManager::GetInstance()->SetRenderSetting();

#pragma endregion

#pragma region スプライトの描画（後面描画・背景用）

	// スプライトの描画設定（後面）
	SpriteManager::GetInstance()->SetRenderSetting_Background();


	// フェードマネージャーの描画
	fadeManager_->Draw();

#pragma endregion
	

#pragma region オブジェクト3Dの描画

	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// ゲームクリアロゴの描画
	gameClearLogo_->Draw();

#pragma endregion


#pragma region スプライトの描画（前面描画・UI用）

	// スプライトの描画設定（前面）
	SpriteManager::GetInstance()->SetRenderSetting_UI();

#pragma endregion
}


/// -------------------------------------------------------------
///						　終了処理
/// -------------------------------------------------------------
void GameClearScene::Finalize()
{
	AudioManager::GetInstance()->StopBGM(); // BGMを停止
}


/// -------------------------------------------------------------
///					　	ImGui描画処理
/// -------------------------------------------------------------
void GameClearScene::DrawImGui()
{
}
