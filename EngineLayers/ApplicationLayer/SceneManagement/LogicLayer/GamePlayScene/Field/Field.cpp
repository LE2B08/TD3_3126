#include "Field.h"
#include <imgui.h>

void Field::Initialize() {

	objectField_ = std::make_unique<Object3D>();
	objectField_->Initialize("Field.obj");
}

void Field::Update() {

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
