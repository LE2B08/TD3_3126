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


/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void GameClearScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// フェードマネージャーの初期化
	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();
	// シーン開始時に白からフェードアウトする（白 → 透明）
	fadeManager_->StartFadeFromWhite(0.02f);
}


/// -------------------------------------------------------------
///						　	更新処理
/// -------------------------------------------------------------
void GameClearScene::Update()
{
	// 入力によるシーン切り替え
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(XButtons.A)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			fadeManager_->StartFadeToWhite(0.02f, [this]() {
				// フェード完了後の処理
				sceneManager_->ChangeScene("TitleScene"); // シーン名を指定して変更
				});
		}
	}

	// フェードマネージャーの更新処理
	fadeManager_->Update();
}


/// -------------------------------------------------------------
///						　	描画処理
/// -------------------------------------------------------------
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


	// フェードマネージャーの描画
	fadeManager_->Draw();

	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();




}


/// -------------------------------------------------------------
///						　終了処理
/// -------------------------------------------------------------
void GameClearScene::Finalize()
{
}


/// -------------------------------------------------------------
///					　	ImGui描画処理
/// -------------------------------------------------------------
void GameClearScene::DrawImGui()
{
}
