#include "EnemyUI.h"
#include <Enemy.h>
#include <cmath>
#include <corecrt_math_defines.h>
#include <imgui.h>

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void EnemyUI::Initialize() {
	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/playerHpUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/hpGauge.png");
	TextureManager::GetInstance()->LoadTexture("Resources/EnemyHead.png");

	//  テクスチャを180度回転
	hpRotation_ = static_cast<float>(M_PI) / 2; // ここを変更

	// HPのスプライトの生成と初期化
	hpFrameSprite_ = std::make_unique<Sprite>();
	hpFrameSprite_->Initialize("Resources/playerHpUI.png");

	// Hpのゲージのスプライト
	hpGaugeSprite_ = std::make_unique<Sprite>();
	hpGaugeSprite_->Initialize("Resources/hpGauge.png");

	// 顔のスプライトの生成と初期化
	headSprite_ = std::make_unique<Sprite>();
	headSprite_->Initialize("Resources/EnemyHead.png");
}

/// -------------------------------------------------------------
///						　	　更新処理
/// -------------------------------------------------------------
void EnemyUI::Update() {

	//-------------------------------------------
	//				HPゲージの更新処理
	//-------------------------------------------

	// 緑から赤への線形補間
	float t = static_cast<float>(enemy_->GetHp()) / 10.0f;
	hpGaugeColor.x = (1.0f - t) * 1.0f + t * 0.0f; // 赤の成分
	hpGaugeColor.y = (1.0f - t) * 0.0f + t * 1.0f; // 緑の成分
	hpGaugeColor.z = 0.0f;                         // 青の成分
	hpGaugeColor.w = 1.0f;                         // アルファ値

	// HPゲージのスプライトの更新
	hp_ = enemy_->GetHp();
	float hpRatio = hp_ / 10.0f;        // HPの割合 (0.0f ～ 1.0f)
	float newHeight = 320.0f * hpRatio; // HPバーの高さを割合で変える
	float yOffset = 320.0f - newHeight; // 上から下に減るように調整
	float xOffset = -320.0f;               // 左から右に減るように調整


	// HPバーの位置とサイズを設定
	hpGaugeSprite_->SetHPBar(static_cast<float>(hp_), { hpPosition_.x, hpPosition_.y - 280.0f }, { 0.0f, yOffset, 32.0f, newHeight }, DecreaseHpDirection::RightToLeft);
	hpGaugeSprite_->SetColor(hpGaugeColor); // HPバーの色を設定
	hpGaugeSprite_->SetRotation(hpRotation_); // HPバーの回転を設定
	hpGaugeSprite_->SetAnchorPoint({ 0.0f, 0.5f }); // アンカーポイントを左上に設定
	hpGaugeSprite_->Update();               // 更新

	
	hpFrameSprite_->SetPosition({ hpPosition_.x + xOffset, hpPosition_.y - 280.0f }); // HPバーの位置を設定
	hpFrameSprite_->SetRotation(hpRotation_);                              // HPバーの回転を設定
	hpFrameSprite_->SetAnchorPoint({ 0.0f, 1.0f });                                  // アンカーポイントを左上に設定
	hpFrameSprite_->SetSize({32.0f, 320.0f*2.0f});                              // サイズを設定
	hpFrameSprite_->Update();

	// 顔のスプライトの更新
	headSprite_->SetPosition(headPosition_);
	headSprite_->Update();
}

/// -------------------------------------------------------------
///						　	　描画処理
/// -------------------------------------------------------------
void EnemyUI::Draw() {
	hpGaugeSprite_->Draw();
	
	hpFrameSprite_->Draw();

	// 顔のスプライトの描画
	headSprite_->Draw();
}

/// -------------------------------------------------------------
///						　ImGuiの描画処理
/// -------------------------------------------------------------
void EnemyUI::DrawImGui() {
	int HP = enemy_->GetHp();

	hp_ = enemy_->GetHp();
	ImGui::Begin("EnemyUI");
	ImGui::DragInt("HP", &hp_, 1, 0, 10);
	ImGui::DragFloat3("Position", &hpPosition_.x);

	ImGui::DragFloat2("headPos", &headPosition_.x, 0.1f);

	ImGui::End();
}
