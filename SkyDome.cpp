#include "SkyDome.h"
#include"imgui.h"
void SkyDome::Initialize() {

	// オブジェクト3D
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("Skydome.obj");
	// 位置
	position_ = {0.0f, 0.0f, 0.0f};
	// 回転
	rotation_ = {0.0f, 0.0f, 0.0f};
	// スケール
	scale_ = {1.0f, 1.0f, 1.0f};


}

void SkyDome::Update() {

	// Transform更新処理
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();

}

void SkyDome::Draw() {

	object3D_->Draw(); }

void SkyDome::Finalize() {}

void SkyDome::DrawImGui() { 
	ImGui::Begin("SkyDome");
	ImGui::Text("Position");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::Text("Rotation");
	ImGui::SliderFloat3("Rotation", &rotation_.x, -10.0f, 10.0f);
	ImGui::Text("Scale");
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::End();
}
