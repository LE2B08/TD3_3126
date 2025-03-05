#include "Field.h"
#include <imgui.h>

void Field::Initialize() {

	scale_ = { 8.0f,1.0f,8.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	position_ = { 0.0f,0.0f,0.0f };

	objectField_ = std::make_unique<Object3D>();
	objectField_->Initialize("Field.obj");
}

void Field::Update() {

	minPosition_ = position_ - scale_;
	maxPosition_ = position_ + scale_;

	objectField_->SetScale(scale_);
	objectField_->SetRotate(rotate_);
	objectField_->SetTranslate(position_);

	objectField_->Update();
}

void Field::Draw() {

	objectField_->Draw();
}

void Field::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::DragFloat3("Scale", &scale_.x, 0.01f);

	ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);

	ImGui::DragFloat3("Position", &position_.x, 0.01f);

	ImGui::End();
}
