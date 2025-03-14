#include "Field.h"
#include <imgui.h>
#include "Wireframe.h"

void Field::Initialize() {

	scale_ = { 10.0f,1.0f,10.0f };
	rotate_ = { 0.0f,0.0f,0.0f };
	position_ = { 0.0f,0.0f,0.0f };
}

void Field::Update() {

	minPosition_ = position_ - scale_;
	maxPosition_ = position_ + scale_;

}

void Field::Draw() {

	// ワイヤーフレームの描画
	// Fieldの描画
	Wireframe::GetInstance()->DrawLine({maxPosition_.x, 0.0f, maxPosition_.z}, {minPosition_.x, 0.0f, maxPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
	Wireframe::GetInstance()->DrawLine({maxPosition_.x, 0.0f, minPosition_.z}, {minPosition_.x, 0.0f, minPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
	Wireframe::GetInstance()->DrawLine({maxPosition_.x, 0.0f, maxPosition_.z}, {maxPosition_.x, 0.0f, minPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
	Wireframe::GetInstance()->DrawLine({minPosition_.x, 0.0f, maxPosition_.z}, {minPosition_.x, 0.0f, minPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
}

void Field::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::DragFloat3("Scale", &scale_.x, 1.0f);

	ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);

	ImGui::DragFloat3("Position", &position_.x, 0.01f);

	ImGui::End();
}
