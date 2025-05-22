#include "TutorialUI.h"
#include "TextureManager.h"
#include <imgui.h>

void TutorialUI::Initialize() {

	// スプライトの初期化
	startUI_ = std::make_unique<Sprite>();
	rotationUI_ = std::make_unique<Sprite>();
	throwUI_ = std::make_unique<Sprite>();
	arcUI_ = std::make_unique<Sprite>();
	moveUI_ = std::make_unique<Sprite>();
	attackUI_ = std::make_unique<Sprite>();
	endUI_ = std::make_unique<Sprite>();

	// スプライトのテクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	// スプライトの位置の初期化
	startUIPos_ = {0.0f, 0.0f};
	rotationUIPos_ = {0.0f, 0.0f};
	throwUIPos_ = {0.0f, 0.0f};
	arcUIPos_ = {0.0f, 0.0f};
	moveUIPos_ = {0.0f, 0.0f};
	attackUIPos_ = {0.0f, 0.0f};
	endUIPos_ = {0.0f, 0.0f};

	// スプライトの設定
	startUI_->Initialize("Resources/uvChecker.png");
	rotationUI_->Initialize("Resources/uvChecker.png");
	throwUI_->Initialize("Resources/uvChecker.png");
	arcUI_->Initialize("Resources/uvChecker.png");
	moveUI_->Initialize("Resources/uvChecker.png");
	attackUI_->Initialize("Resources/uvChecker.png");
	endUI_->Initialize("Resources/uvChecker.png");

	// スプライトの位置の設定
	startUI_->SetPosition(startUIPos_);
	rotationUI_->SetPosition(rotationUIPos_);
	throwUI_->SetPosition(throwUIPos_);
	arcUI_->SetPosition(arcUIPos_);
	moveUI_->SetPosition(moveUIPos_);
	attackUI_->SetPosition(attackUIPos_);
	endUI_->SetPosition(endUIPos_);
}

void TutorialUI::Update() {

	// スプライトの位置の更新
	startUI_->SetPosition(startUIPos_);
	rotationUI_->SetPosition(rotationUIPos_);
	throwUI_->SetPosition(throwUIPos_);
	arcUI_->SetPosition(arcUIPos_);
	moveUI_->SetPosition(moveUIPos_);
	attackUI_->SetPosition(attackUIPos_);
	endUI_->SetPosition(endUIPos_);

	// スプライトの更新
	startUI_->Update();
	rotationUI_->Update();
	throwUI_->Update();
	arcUI_->Update();
	moveUI_->Update();
	attackUI_->Update();
	endUI_->Update();
}

void TutorialUI::StartDraw() { startUI_->Draw(); }

void TutorialUI::RotationDraw() { rotationUI_->Draw(); }

void TutorialUI::ThrowDraw() { throwUI_->Draw(); }

void TutorialUI::ArcDraw() { arcUI_->Draw(); }

void TutorialUI::MoveDraw() { moveUI_->Draw(); }

void TutorialUI::AttackDraw() { attackUI_->Draw(); }

void TutorialUI::EndDraw() { endUI_->Draw(); }

void TutorialUI::DrawImGui() {

	ImGui::Begin("TutorialUI");
	ImGui::SliderFloat2("StartUIPos", &startUIPos_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("RotationUIPos", &rotationUIPos_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("ThrowUIPos", &throwUIPos_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("ArcUIPos", &arcUIPos_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("MoveUIPos", &moveUIPos_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("AttackUIPos", &attackUIPos_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("EndUIPos", &endUIPos_.x, -10.0f, 10.0f);
	ImGui::End();
}
