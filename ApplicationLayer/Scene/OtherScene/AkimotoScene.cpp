#include "AkimotoScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"
#include "Object3DCommon.h"
#include "Wireframe.h"
#include <SkyBoxManager.h>
#include <SpriteManager.h>
#include "FadeManager.h"

void AkimotoScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	Object3DCommon::GetInstance()->SetDefaultCamera(camera_);

	textureManager->LoadTexture("Resources/Green.png"); // テクスチャの読み込み

	// スプライトの初期化
	progressGauge_ = std::make_unique<Sprite>();
	progressGauge_->Initialize("Resources/Green.png");
}

void AkimotoScene::Update() {
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_) {
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_F1)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_F3)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("SatouScene");
		}
	}

	// スペースキーを押したら
	if (input_->TriggerKey(DIK_SPACE)) {

		// 進行度が最大値に達していない場合
		if (progressCount_ < maxCount_) {

			progressCount_++; // 進行度を1増やす
		}
		// 最大値に達したら
		else {
			progressCount_ = 0; // リセット
		}
	}

	// カメラの更新
	camera_->Update();

	UpdateGauge(); // ゲージの更新
}

void AkimotoScene::Draw() {
	/// ------------------------------------------ ///
	/// ---------- スカイボックスの描画 ---------- ///
	/// ------------------------------------------ ///
	SkyBoxManager::GetInstance()->SetRenderSetting();



	/// ---------------------------------------- ///
	/// ----------  スプライトの描画  ---------- ///
	/// ---------------------------------------- ///
	// スプライトの共通描画設定
	SpriteManager::GetInstance()->SetRenderSetting_Background();

	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();


	// スプライトの共通描画設定
	SpriteManager::GetInstance()->SetRenderSetting_UI();

	// ゲージの描画
	progressGauge_->Draw();
}

void AkimotoScene::Finalize() {
}

void AkimotoScene::DrawImGui() {

	ImGui::Begin("SettingGauge");

	Vector2 pos = progressGauge_->GetPosition();
	ImGui::SliderFloat2("Position", &pos.x, 0.0f, 1280.0f);
	progressGauge_->SetPosition(pos);

	Vector2 size = progressGauge_->GetSize();
	ImGui::SliderFloat2("Size", &size.x, 0.0f, 1280.0f);
	progressGauge_->SetSize(size);

	ImGui::SliderInt("Count", &progressCount_, 0, maxCount_); // 進行度のスライダー
	ImGui::SliderFloat("Ratio", &progressRatio_, 0.0f, 1.0f); // 進行度の割合のスライダー

	ImGui::End();
}

void AkimotoScene::UpdateGauge() {

	// 進行度の割合を計算
	progressRatio_ = static_cast<float>(progressCount_) / maxCount_;

	// ゲージのサイズを設定
	Vector2 size = { gaugeDefaultSize_.x * progressRatio_, gaugeDefaultSize_.y };

	progressGauge_->SetSize(size);

	progressGauge_->Update();
}
