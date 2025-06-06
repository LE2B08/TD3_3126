#include "PlayerUI.h"
#include <Player.h>
#include <cmath>
#include <corecrt_math_defines.h>
#include <imgui.h>

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void PlayerUI::Initialize() {
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/playerHpUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/hpGauge.png");
	TextureManager::GetInstance()->LoadTexture("Resources/PlayerHead.png");

	//  テクスチャを180度回転
	hpRotation_ = static_cast<float>(M_PI); // ここを変更

	// HPのスプライトの生成と初期化
	hpSprite_ = std::make_unique<Sprite>();
	hpSprite_->Initialize("Resources/playerHpUI.png");

	// Hpのゲージのスプライト
	hpGaugeSprite_ = std::make_unique<Sprite>();
	hpGaugeSprite_->Initialize("Resources/hpGauge.png");

	// 顔のスプライトの生成と初期化
	headSprite_ = std::make_unique<Sprite>();
	headSprite_->Initialize("Resources/PlayerHead.png");
}

/// -------------------------------------------------------------
///						　	　更新処理
/// -------------------------------------------------------------
void PlayerUI::Update() {

	//-------------------------------------------
	//				HPゲージの更新処理
	//-------------------------------------------

	// 緑から赤への線形補間
	float t = static_cast<float>(player_->GetHp()) / 10.0f;
	hpGaugeColor.x = (1.0f - t) * 1.0f + t * 0.0f; // 赤の成分
	hpGaugeColor.y = (1.0f - t) * 0.0f + t * 1.0f; // 緑の成分
	hpGaugeColor.z = 0.0f;                         // 青の成分
	hpGaugeColor.w = 1.0f;                         // アルファ値

	// HPゲージのスプライトの更新
	hp_ = player_->GetHp();
	float hpRatio = hp_ / 10.0f;         // HPの割合 (0.0f ～ 1.0f)
	float newHeight = 320.0f * hpRatio; // HPバーの高さを割合で変える
	float yOffset = 320.0f - newHeight; // 上から下に減るように調整

	//------------------------------------------
	//				Speedの更新処理
	//------------------------------------------

	// プレイヤーの速度を取得
	Vector3 playerVelocity = player_->GetVelocity();
	// プレイヤーの速度をfloatに変換
	speed_ = static_cast<float>(sqrt(playerVelocity.x * playerVelocity.x + playerVelocity.z * playerVelocity.z));

	// HPバーの位置とサイズを設定
	hpGaugeSprite_->SetHPBar(static_cast<float>(hp_), { hpPosition_.x, hpPosition_.y + yOffset }, { 0.0f, yOffset, 32.0f, newHeight }, DecreaseHpDirection::TopToBottom);
	hpGaugeSprite_->SetColor(hpGaugeColor); // HPバーの色を設定
	hpGaugeSprite_->Update();               // 更新

	// HPのスプライトの更新
	hpSprite_->SetTextureRect({ hpPosition_.x, hpPosition_.y }, { 0.0f, 0.0f, 32.0f, 320.0f });
	hpSprite_->Update();

	// 顔のスプライトの更新
	headSprite_->SetPosition(headPosition_);
	headSprite_->Update();
}

/// -------------------------------------------------------------
///						　	　描画処理
/// -------------------------------------------------------------
void PlayerUI::Draw() {
	hpGaugeSprite_->Draw();
	hpSprite_->Draw();
	headSprite_->Draw();
}

/// -------------------------------------------------------------
///						　ImGuiの描画処理
/// -------------------------------------------------------------
void PlayerUI::DrawImGui() {
	hp_ = player_->GetHp();
	ImGui::Begin("PlayerUI");
	ImGui::DragInt("HP", &hp_, 1, 0, 10);
	ImGui::DragFloat("Speed", &speed_, 0.1f, 0.0f, 10.0f);

	ImGui::DragFloat2("headPos", &headPosition_.x, 0.1f);
	ImGui::End();
}
