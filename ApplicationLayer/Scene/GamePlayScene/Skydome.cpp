#include "Skydome.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void Skydome::Initialize() {


	position_ = {0.0f, 0.0f, 0.0f};
	scale_ = {1.0f, 1.0f, 1.0f};
	rotation_ = {0.0f, 0.0f, 0.0f};


	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("Skydome.obj");

	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);


}

void Skydome::Update() {

	rotation_.x+=0.001f;
	rotation_.z+=0.001f;
	// 位置
	object3D_->SetTranslate(position_);
	// 回転
	object3D_->SetRotate(rotation_);
	// スケール
	object3D_->SetScale(scale_);

	// カメラの位置をスカイドームの位置に設定
	object3D_->SetCamera(camera_);

	// スカイドームの色を設定
	float alpha = 0.5f;
	object3D_->SetAlpha(alpha);
	

	object3D_->Update();
}

void Skydome::Draw() { object3D_->Draw(); }

void Skydome::DrawImGui() {
	ImGui::Begin("Skydome");
	ImGui::Text("Skydome");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::End();
}

