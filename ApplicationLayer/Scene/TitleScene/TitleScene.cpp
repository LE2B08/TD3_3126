#define NOMINMAX
#include "TitleScene.h"
#include "FadeManager.h"
#include "SceneManager.h"
#include <SpriteManager.h>
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Object3DCommon.h>
#include <SkyBoxManager.h>
#include <WinApp.h>
#include <AudioManager.h>


/// -------------------------------------------------------------
///				　			　初期化処理
/// -------------------------------------------------------------
void TitleScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input = Input::GetInstance();

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	// カメラの初期化
	camera_->SetRotate({ 1.57f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,50.0f, 0.0f });

	// フェードマネージャーの初期化
	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();

	fadeManager_->StartFadeFromWhite(0.02f); // シーン開始時に白からフェードアウトする（白 → 透明）

	// テクスチャのパスをリストで管理
	texturePaths_ = {
		"Resources/HowToPlayForTitle.png",
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

	// タイトルオブジェクトの初期化
	titleObject_ = std::make_unique<TitleObject>();
	titleObject_->Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/CameraShakeOnText.png");
	TextureManager::GetInstance()->LoadTexture("Resources/CameraShakeOffText.png");

	// カメラシェイクの初期化
	cameraShakeOnSprite_ = std::make_unique<Sprite>();
	cameraShakeOnSprite_->Initialize("Resources/CameraShakeOnText.png");

	cameraShakeOffSprite_ = std::make_unique<Sprite>();
	cameraShakeOffSprite_->Initialize("Resources/CameraShakeOffText.png");

	cameraShakeEnabled_ = sceneManager_->GetCameraShakeEnabled(); // カメラシェイクの状態を取得
}

/// -------------------------------------------------------------
///				　			　 更新処理
/// -------------------------------------------------------------
void TitleScene::Update()
{
#ifdef _DEBUG
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
#endif // _DEBUG

	// Updateの中に状態管理追加
	switch (titleState_) {
	case TitleState::Appear:
		// ※ ここで登場演出を入れたければ titleObject_->StartAppear() など
		titleState_ = TitleState::Idle;
		break;

	case TitleState::Idle:
		// Idle中の点滅演出など（今の titleObject_->Update() に書かれてるかも）

		// 入力によるシーン切り替え
		if (titleState_ == TitleState::Idle &&
			input->TriggerKey(DIK_RETURN) || input->TriggerButton(XButtons.A)) // Enterキーが押されたら
		{
			titleState_ = TitleState::Exit;
			exitTimer_ = 0.0f;

			titleObject_->StartExitAnimation(); // タイトルロゴの退場アニメーションを開始
		}

		// 操作方法の表示 Bボタンで表示
		if (input->TriggerButton(XButtons.B))
		{
			titleState_ = TitleState::Operate;
		}

		 // XボタンでカメラシェイクのON/OFF切り替え
    if (input->TriggerButton(XButtons.X)) {
        cameraShakeEnabled_ = !cameraShakeEnabled_;
    }

		break;

	case TitleState::Exit:
		exitTimer_ += 1.0f / 60.0f;
		{
			float t = std::min(exitTimer_ / 1.0f, 1.0f); // 1秒かけて退場

			// タイトルオブジェクトに退場アニメ命令
			titleObject_->SetAlpha(1.0f - t);
			titleObject_->SetOffsetY(-100.0f * t); // 少し落とす演出（任意）

			if (titleObject_->IsExitAnimationComplete()) {
				// 白フェードへ移行
				fadeManager_->StartFadeToWhite(0.02f, [this]() {
					sceneManager_->ChangeScene("GamePlayScene");
					});

				titleState_ = TitleState::Transition;
			}
		}
		break;

	case TitleState::Transition:
		// フェードアウト進行中（fadeManager_->Update() が処理してくれる）
		// フェードマネージャーの更新処理
		fadeManager_->Update();
		break;

	case TitleState::Operate:
		// 操作方法の表示 Bボタンで非表示
		if (input->TriggerButton(XButtons.B)) {
			titleState_ = TitleState::Idle;
		}
		break;
	}

	// スプライトの更新処理
	for (auto& sprite : sprites_) {
		sprite->Update();
	}
	cameraShakeOnSprite_->Update();

	cameraShakeOffSprite_->Update();
	// フェードマネージャーの更新処理
	fadeManager_->Update();

	// タイトルオブジェクトの更新処理
	titleObject_->Update();

	// カメラの更新処理
	camera_->Update();

	sceneManager_->SetCameraShakeEnabled(cameraShakeEnabled_);
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

#pragma endregion


#pragma region オブジェクト3Dの描画

	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// タイトルオブジェクトの描画
	titleObject_->Draw();

	if (cameraShakeEnabled_) {
		titleObject_->DrawCameraShakeOn();
	} else {
		titleObject_->DrawCameraShakeOff();
	}

#pragma endregion


#pragma region スプライトの描画（前面描画・UI用）

	// UIスプライトの描画設定（前面）
	SpriteManager::GetInstance()->SetRenderSetting_UI();

	// 操作方法を描画
	if (titleState_ == TitleState::Operate)
	{
		/// ----- スプライトの描画設定と描画 ----- ///
		for (auto& sprite : sprites_)
		{
			sprite->Draw();
		}
	}
	/*if (cameraShakeEnabled_) {
		cameraShakeOnSprite_->Draw();
	}else {
		cameraShakeOffSprite_->Draw();
	}*/
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

	AudioManager::GetInstance()->StopBGM(); // BGMを停止
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

	// タイトルオブジェクトのImGui描画
	if (titleObject_) {
		titleObject_->DrawImGui();
	}

	ImGui::End();
	ImGui::Begin("SceneManager");
	ImGui::Text("cameraShakeEnabled : %d", sceneManager_->GetCameraShakeEnabled());
	ImGui::End();
}
