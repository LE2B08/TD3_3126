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
#include <AudioManager.h>


/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void GameOverScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	// カメラの初期化
	camera_->SetRotate({ 1.57f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,50.0f, 0.0f });

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

	// タイトルオブジェクトの初期化
	gameOverLogo_ = std::make_unique<GameOverLogo>();
	gameOverLogo_->Initialize();
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
	}
#ifdef _DEBUG
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
#endif // _DEBUG

	// スプライトの更新処理
	for (auto& sprite : sprites_)
	{
		sprite->Update();
	}

	// カメラの更新処理
	camera_->Update();

	// フェードの更新
	fadeManager_->Update();

	// ゲームオーバーロゴの更新
	gameOverLogo_->Update();
}


/// -------------------------------------------------------------
///						　描画処理
/// -------------------------------------------------------------
void GameOverScene::Draw()
{
#pragma region スカイボックスの描画

	// スカイボックスの描画設定
	SkyBoxManager::GetInstance()->SetRenderSetting();

#pragma endregion


#pragma region スプライトの描画（後面描画・背景用）

	// スプライトの描画設定（後面）
	SpriteManager::GetInstance()->SetRenderSetting_Background();
	// スプライトの更新処理
	for (auto& sprite : sprites_)
	{
	//	sprite->Draw();
	}

#pragma endregion


#pragma region オブジェクト3Dの描画

	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// ゲームオーバーロゴの描画
	gameOverLogo_->Draw();

#pragma endregion


#pragma region スプライトの描画（前面描画・UI用）

	// スプライトの描画設定（前面）
	SpriteManager::GetInstance()->SetRenderSetting_UI();

	// フェードの描画（最後尾に置く）
	fadeManager_->Draw();

#pragma endregion
}


/// -------------------------------------------------------------
///						　終了処理
/// -------------------------------------------------------------
void GameOverScene::Finalize()
{
	AudioManager::GetInstance()->StopBGM(); // BGMを停止
}


/// -------------------------------------------------------------
///						　ImGui描画処理
/// -------------------------------------------------------------
void GameOverScene::DrawImGui()
{

}
