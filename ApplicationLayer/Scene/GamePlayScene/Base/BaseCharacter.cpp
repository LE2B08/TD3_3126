#include "BaseCharacter.h"
#include "Input.h"
#include "Camera.h"
#include "Object3DCommon.h"


/// -------------------------------------------------------------
///					　		初期化処理
/// -------------------------------------------------------------
void BaseCharacter::Initialize()
{
	input_ = Input::GetInstance();
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	// ワールド変換を初期化
	worldTransform_.Initialize();
}


/// -------------------------------------------------------------
///					　		更新処理
/// -------------------------------------------------------------
void BaseCharacter::Update()
{
	// ワールド変換の更新
	object3D_->SetCamera(camera_);
	object3D_->SetTranslate(worldTransform_.translate_);
	object3D_->SetRotate(worldTransform_.rotate_);
	object3D_->SetScale(worldTransform_.scale_);
	worldTransform_.Update();
}


/// -------------------------------------------------------------
///					　		描画処理
/// -------------------------------------------------------------
void BaseCharacter::Draw()
{
	object3D_->Draw();
}


/// -------------------------------------------------------------
///					　		衝突判定処理
/// -------------------------------------------------------------
void BaseCharacter::OnCollision(Collider* other)
{

}


/// -------------------------------------------------------------
///					　	中心座標を取得
/// -------------------------------------------------------------
Vector3 BaseCharacter::GetCenterPosition() const
{
	return Vector3();
}
