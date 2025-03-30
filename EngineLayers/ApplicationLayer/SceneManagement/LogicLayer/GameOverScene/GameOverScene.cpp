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


void GameOverScene::Initialize()
{
	FadeManager::GetInstance()->StartFadeOut();

	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

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

void GameOverScene::Update()
{
	// 入力によるシーン切り替え
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(XButtons.A)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TitleScene"); // シーン名を指定して変更
		}

		wavLoader_->StopBGM();
	}

	if (input->TriggerKey(DIK_F1))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	if (input->TriggerKey(DIK_F2))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	if (input->TriggerKey(DIK_F3))
	{

	}

	// スプライトの更新処理
	for (auto& sprite : sprites_)
	{
		sprite->Update();
	}
}

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

	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();


}

void GameOverScene::Finalize()
{
}

void GameOverScene::DrawImGui()
{
}
