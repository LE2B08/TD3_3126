#include "TitleScene.h"
#include "FadeManager.h"
#include "SceneManager.h"
#include <SpriteManager.h>
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Object3DCommon.h>
#include <SkyBoxManager.h>
#include <WinApp.h>


/// -------------------------------------------------------------
///				　			　初期化処理
/// -------------------------------------------------------------
void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// フェードマネージャーの初期化
	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();

	fadeManager_->StartFadeFromWhite(0.02f); // シーン開始時に白からフェードアウトする（白 → 透明）

	// テクスチャのパスをリストで管理
	texturePaths_ = {
		"Resources/titleSceneUI.png ",
		//"Resources/uvChecker.png",
	};

	/// ---------- TextureManagerの初期化 ----------///
	for (auto& texture : texturePaths_) {
		textureManager->LoadTexture(texture);
	}

	/// ---------- Spriteの初期化 ---------- ///
	for (uint32_t i = 0; i < 1; i++) {
		sprites_.push_back(std::make_unique<Sprite>());

		// テクスチャの範囲をチェック
		if (!texturePaths_.empty()) {
			sprites_[i]->Initialize(texturePaths_[i % texturePaths_.size()]);
		}
		else {
			throw std::runtime_error("Texture paths list is empty!");
		}

		sprites_[i]->SetPosition(Vector2(100.0f * i, 100.0f * i));
	}
}

/// -------------------------------------------------------------
///				　			　 更新処理
/// -------------------------------------------------------------
void TitleScene::Update()
{
	// 入力によるシーン切り替え
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(XButtons.A)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			fadeManager_->StartFadeToWhite(0.02f, [this]() {
				// フェード完了後の処理
				sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
				});
		}

		wavLoader_->StopBGM();
	}

	if (input->TriggerKey(DIK_F1)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	if (input->TriggerKey(DIK_F2)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	if (input->TriggerKey(DIK_F3)) {
	}

	// スプライトの更新処理
	for (auto& sprite : sprites_) {
		sprite->Update();
	}

	// フェードマネージャーの更新処理
	fadeManager_->Update();
}

/// -------------------------------------------------------------
///				　			　 描画処理
/// -------------------------------------------------------------
void TitleScene::Draw()
{
#pragma region スカイボックスの描画

	SkyBoxManager::GetInstance()->SetRenderSetting();

#pragma endregion


#pragma region スプライトの描画（後面描画・背景用）

	// スプライトの描画設定（後面）
	SpriteManager::GetInstance()->SetRenderSetting_Background();

	/// ----- スプライトの描画設定と描画 ----- ///
	for (auto& sprite : sprites_)
	{
		sprite->Draw();
	}

#pragma endregion


#pragma region オブジェクト3Dの描画

	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

#pragma endregion


#pragma region スプライトの描画（前面描画・UI用）

	// UIスプライトの描画設定（前面）
	SpriteManager::GetInstance()->SetRenderSetting_UI();


	// フェードの描画（最後尾に置く）
	fadeManager_->Draw();

#pragma endregion
}

/// -------------------------------------------------------------
///				　			　 終了処理
/// -------------------------------------------------------------
void TitleScene::Finalize()
{
	if (!sprites_.empty()) {
		sprites_.clear();
	}

	if (wavLoader_) {
		wavLoader_.reset();
	}
}

/// -------------------------------------------------------------
///				　		　ImGui描画処理
/// -------------------------------------------------------------
void TitleScene::DrawImGui()
{
	ImGui::Begin("Test Window");

	for (uint32_t i = 0; i < sprites_.size(); i++) {
		ImGui::PushID(i); // スプライトごとに異なるIDを設定
		if (ImGui::TreeNode(("Sprite" + std::to_string(i)).c_str())) {
			Vector2 position = sprites_[i]->GetPosition();
			ImGui::DragFloat2("Position", &position.x, 1.0f);
			sprites_[i]->SetPosition(position);

			float rotation = sprites_[i]->GetRotation();
			ImGui::SliderAngle("Rotation", &rotation);
			sprites_[i]->SetRotation(rotation);

			Vector2 size = sprites_[i]->GetSize();
			ImGui::DragFloat2("Size", &size.x, 1.0f);
			sprites_[i]->SetSize(size);

			ImGui::TreePop();
		}
		ImGui::PopID(); // IDを元に戻す
	}

	ImGui::End();
}
