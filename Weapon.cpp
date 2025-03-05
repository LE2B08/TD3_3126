#include "Weapon.h"

void Weapon::Initialize() { 
	
	//位置
	position_ = player_->GetPosition();
	// 回転
	rotation_ = player_->GetRotation();
	// スケール
	scale_ = player_->GetScale();

	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("sphere.gltf");
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);


}

void Weapon::Update() {
	// 位置
	position_ = player_->GetPosition();
	// 回転
	rotation_ = player_->GetRotation();
	// スケール
	scale_ = player_->GetScale();

	// Transform更新処理
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();

}

void Weapon::Draw() { object3D_->Draw(); }

void Weapon::Finalize() {}

void Weapon::DrawImGui() {}

void Weapon::Attack() {}
