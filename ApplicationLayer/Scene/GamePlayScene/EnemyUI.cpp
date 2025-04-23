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

	//  テクスチャを180度回転
	hpRotation_ = static_cast<float>(M_PI)/2; // ここを変更

	// HPのスプライトの生成と初期化
	hpSprite_ = std::make_unique<Sprite>();
	hpSprite_->Initialize("Resources/playerHpUI.png");

	// HPのスプライトの生成と初期化
	hpSprite2_ = std::make_unique<Sprite>();
	hpSprite2_->Initialize("Resources/playerHpUI.png");

	// Hpのゲージのスプライト
	hpGaugeSprite_ = std::make_unique<Sprite>();
	hpGaugeSprite_->Initialize("Resources/hpGauge.png");
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
	hpGaugeSprite_->SetHPBar(static_cast<float>(hp_), {hpPosition_.x, hpPosition_.y + yOffset}, {0.0f, yOffset, 32.0f, newHeight}, DecreaseHpDirection::RightToLeft);
	hpGaugeSprite_->SetColor(hpGaugeColor); // HPバーの色を設定
	hpGaugeSprite_->SetRotation(hpRotation_); // HPバーの回転を設定
	hpGaugeSprite_->SetAnchorPoint({0.0f, 0.5f}); // アンカーポイントを左上に設定
	hpGaugeSprite_->Update();               // 更新

	// HPのスプライトの更新
	//hpSprite_->SetTextureRect({hpPosition_.x, hpPosition_.y}, {0.0f, 0.0f, 32.0f, 320.0f});
	hpSprite_->SetPosition({hpPosition_.x, hpPosition_.y + yOffset}); // HPバーの位置を設定
	hpSprite_->SetRotation(hpRotation_); // HPバーの回転を設定
	hpSprite_->SetAnchorPoint({0.0f, 1.0f});                          // アンカーポイントを左上に設定
	hpSprite_->Update();

	hpSprite2_->SetPosition({hpPosition_.x+xOffset, hpPosition_.y + yOffset}); // HPバーの位置を設定
	hpSprite2_->SetRotation(hpRotation_);                              // HPバーの回転を設定
	hpSprite2_->SetAnchorPoint({0.0f, 1.0f});                                  // アンカーポイントを左上に設定
	hpSprite2_->Update();
}

/// -------------------------------------------------------------
///						　	　描画処理
/// -------------------------------------------------------------
void EnemyUI::Draw() {
	hpGaugeSprite_->Draw();
	hpSprite_->Draw();
	hpSprite2_->Draw();
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
	
	ImGui::End();
}
