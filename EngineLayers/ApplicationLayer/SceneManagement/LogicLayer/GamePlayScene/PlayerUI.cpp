#include "PlayerUI.h"
#include <imgui.h>

void PlayerUI::Initialize() {

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	//-----------------全体-----------------//
	position_ = {0.0f, 0.0f};
	scale_ = {1.0f, 1.0f};
	rotation_ = {0.0f, 0.0f};

	//-----------------Hp-----------------//
	hp_ = 100;
	maxHp_ = 100;
	minHp_ = 0;

	hpPosition_ = {0.0f, 0.0f};
	hpScale_ = {1.0f, 1.0f};
	hpRotation_ = {0.0f, 0.0f};

	hpSprite_ = std::make_unique<Sprite>();
	hpSprite_->Initialize("Resources/uvChecker.png");

	//-----------------Speed-----------------//
	speed_ = 1.0f;
	maxSpeed_ = 10.0f;
	minSpeed_ = 0.0f;

	speedPosition_ = {0.0f, 0.0f};
	speedScale_ = {1.0f, 1.0f};
	speedRotation_ = {0.0f, 0.0f};

	speedSprite_ = std::make_unique<Sprite>();
	speedSprite_->Initialize("Resources/uvChecker.png");
}

void PlayerUI::Update() {
	// Hpに応じてテクスチャを変更
	float hpRatio = static_cast<float>(hp_) / maxHp_;
	float textureHeight = 100.0f; // テクスチャの高さを設定
	float newY = -textureHeight * (1.0f - hpRatio);

	Vector4 uvRect(0.0f, 0.0f, 100.0f, (float)hp_);
	hpSprite_->SetTextureRect(uvRect);
	hpSprite_->SetPosition(hpPosition_);
	hpSprite_->Update();
}

void PlayerUI::Draw() { hpSprite_->Draw(); }

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
	ImGui::DragFloat2("HpPosition", &hpPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("HpScale", &hpScale_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("HpRotation", &hpRotation_.x, 0.1f, -100.0f, 100.0f);

	ImGui::Text("Speed");
	ImGui::DragFloat("Speed", &speed_, 0.1f, minSpeed_, maxSpeed_);
	ImGui::DragFloat("MaxSpeed", &maxSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("MinSpeed", &minSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat2("SpeedPosition", &speedPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("SpeedScale", &speedScale_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat2("SpeedRotation", &speedRotation_.x, 0.1f, -100.0f, 100.0f);

	ImGui::End();
}
