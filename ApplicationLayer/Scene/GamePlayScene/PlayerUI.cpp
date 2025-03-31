#include "PlayerUI.h"
#include <cmath>
#include <corecrt_math_defines.h>
#include <imgui.h>

void PlayerUI::Initialize() {

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/playerHpUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/hpGauge.png");

	//-----------------全体-----------------//
	position_ = {0.0f, 0.0f};
	scale_ = {1.0f, 1.0f};
	rotation_ = {0.0f, 0.0f};

	//-----------------Hp-----------------//
	hp_ = 10;
	maxHp_ = 10;
	minHp_ = 0;

	hpPosition_ = {100.0f, 320.0f};
	// hpScale_ = {100.0f, 100.0f};
	//  テクスチャを180度回転
	hpRotation_ = static_cast<float>(M_PI); // ここを変更

	hpSprite_ = std::make_unique<Sprite>();
	hpSprite_->Initialize("Resources/playerHpUI.png");

	// Hpのゲージのスプライト
	hpGaugeSprite_ = std::make_unique<Sprite>();
	hpGaugeSprite_->Initialize("Resources/hpGauge.png");

	//-----------------Speed-----------------//
	speed_ = 1.0f;
	maxSpeed_ = 10.0f;
	minSpeed_ = 0.0f;

	speedPosition_ = {100.0f, 100.0f};
	speedScale_ = {1.0f, 1.0f};
	speedRotation_ = {};

	speedSprite_ = std::make_unique<Sprite>();
	speedSprite_->Initialize("Resources/uvChecker.png");
}
void PlayerUI::Update() {

	hpScale_ = {hpSprite_->GetTextureSize().x, hpSprite_->GetTextureSize().y};

	hpSprite_->SetPosition(hpPosition_);
	hpSprite_->SetRotation(hpRotation_);
	hpSprite_->SetSize(hpScale_);
	hpSprite_->Update();

	Vector4 hpGaugeColor;

	// 緑から赤への線形補間
	float t = static_cast<float>(hp_) / maxHp_;
	hpGaugeColor.x = (1.0f - t) * 1.0f + t * 0.0f; // 赤の成分
	hpGaugeColor.y = (1.0f - t) * 0.0f + t * 1.0f; // 緑の成分
	hpGaugeColor.z = 0.0f;                         // 青の成分
	hpGaugeColor.w = 1.0f;                         // アルファ値

	hpGaugeSprite_->SetPosition(hpPosition_);
	hpGaugeSprite_->SetRotation(hpRotation_);
	hpGaugeSprite_->SetSize({hpScale_.x, (float)hp_ * (hpScale_.y / maxHp_)});
	hpGaugeSprite_->SetColor(hpGaugeColor);
	hpGaugeSprite_->Update();
}


void PlayerUI::Draw() {

	hpGaugeSprite_->Draw();
	hpSprite_->Draw();
}

void PlayerUI::Finalize() {}

void PlayerUI::DrawImGui() {

	ImGui::Begin("PlayerUI");

	ImGui::DragFloat2("Position", &position_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("Scale", &scale_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("Rotation", &rotation_.x, 0.1f, -100.0f, 100.0f);

	ImGui::Text("HP");
	ImGui::DragInt("HP", &hp_, 1, minHp_, maxHp_);
	ImGui::DragInt("MaxHP", &maxHp_, 1, 0, 100);
	ImGui::DragInt("MinHP", &minHp_, 1, 0, 100);
	ImGui::DragFloat2("HpPosition", &hpPosition_.x, 0.1f, 0.0f, 1280.0f);
	ImGui::DragFloat2("HpScale", &hpScale_.x, 0.1f, 0.0f, 300.0f);
	ImGui::DragFloat("HpRotation", &hpRotation_, 0.1f, -100.0f, 100.0f);

	ImGui::Text("Speed");
	ImGui::DragFloat("Speed", &speed_, 0.1f, minSpeed_, maxSpeed_);
	ImGui::DragFloat("MaxSpeed", &maxSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("MinSpeed", &minSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat2("SpeedPosition", &speedPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("SpeedScale", &speedScale_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("SpeedRotation", &speedRotation_, 0.1f, -100.0f, 100.0f);

	ImGui::End();
}
