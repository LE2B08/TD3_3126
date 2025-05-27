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
	gauge_ = std::make_unique<Sprite>();
	gauge_->Initialize("Resources/Green.png");
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
		if (count_ < maxCount_) {

			count_++; // 進行度を1増やす
		}
		// 最大値に達したら
		else {
			count_ = 0; // リセット
		}
	}

	// カメラの更新
	camera_->Update();

	// ゲージの進行度を計算
	GaugeProgress(count_, maxCount_);
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
	gauge_->Draw();
}

void AkimotoScene::Finalize() {
}

void AkimotoScene::DrawImGui() {

	ImGui::Begin("SettingGauge");

	Vector2 pos = gauge_->GetPosition();
	ImGui::SliderFloat2("Position", &pos.x, 0.0f, 1280.0f);
	gauge_->SetPosition(pos);

	Vector2 size = gauge_->GetSize();
	ImGui::SliderFloat2("Size", &size.x, 0.0f, 1280.0f);
	gauge_->SetSize(size);

	ImGui::SliderInt("Count", &count_, 0, maxCount_); // 進行度のスライダー
	ImGui::SliderInt("Max Count", &maxCount_, 1, 100); // 最大進行度のスライダー

	ImGui::End();
}

void AkimotoScene::GaugeProgress(int count, int maxCount) {

	// 進行度の割合を計算
	const float progressRatio_ = static_cast<float>(count) / maxCount;

	// ゲージのサイズを設定
	Vector2 size = { gaugeDefaultSize_.x * progressRatio_, gaugeDefaultSize_.y };

	gauge_->SetSize(size);

	gauge_->Update();
}
