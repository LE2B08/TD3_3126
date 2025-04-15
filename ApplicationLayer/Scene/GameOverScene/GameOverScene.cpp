#include "GameOverScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include <SpriteManager.h>
#include <Object3DCommon.h>
#include <SkyBoxManager.h>
#include "SceneManager.h"
#include "FadeManager.h"


/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// フェードマネージャーの初期化
	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();
	// シーン開始時に白からフェードアウトする（白 → 透明）
	fadeManager_->StartFadeFromWhite(0.02f);

	// テクスチャのパスをリストで管理
	texturePaths_ = {
		"Resources/monsterBall.png",
		//"Resources/uvChecker.png",
	};

	/// ---------- TextureManagerの初期化 ----------///
	for (auto& texture : texturePaths_)
	{
		textureManager->LoadTexture(texture);
	}

	/// ---------- Spriteの初期化 ---------- ///
	for (uint32_t i = 0; i < 1; i++)
	{
		sprites_.push_back(std::make_unique<Sprite>());

		// テクスチャの範囲をチェック
		if (!texturePaths_.empty())
		{
			sprites_[i]->Initialize(texturePaths_[i % texturePaths_.size()]);
		}
		else
		{
			throw std::runtime_error("Texture paths list is empty!");
		}

		sprites_[i]->SetPosition(Vector2(100.0f * i, 100.0f * i));
	}
}


/// -------------------------------------------------------------
///						　更新処理
/// -------------------------------------------------------------
void GameOverScene::Update()
{
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(XButtons.A)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			fadeManager_->StartFadeToWhite(0.02f, [this]() {
				// フェード完了後の処理
				sceneManager_->ChangeScene("TitleScene"); // シーン名を指定して変更
				});
		}

		wavLoader_->StopBGM();
	}

	if (input_->TriggerKey(DIK_F1))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	if (input_->TriggerKey(DIK_F2))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	if (input_->TriggerKey(DIK_F3))
	{

	}

	// スプライトの更新処理
	for (auto& sprite : sprites_)
	{
		sprite->Update();
	}

	// フェードの更新
	fadeManager_->Update();
}


/// -------------------------------------------------------------
///						　描画処理
/// -------------------------------------------------------------
void GameOverScene::Draw()
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
	// スプライトの更新処理
	for (auto& sprite : sprites_)
	{
		sprite->Draw();
	}

	// フェードの描画（最後尾に置く）
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
void GameOverScene::Finalize()
{

}


/// -------------------------------------------------------------
///						　ImGui描画処理
/// -------------------------------------------------------------
void GameOverScene::DrawImGui()
{

}
