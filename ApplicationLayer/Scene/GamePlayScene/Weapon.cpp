#include "Weapon.h"
#include "Player.h"
#include "TutorialPlayer.h"
#include "Enemy.h"
#include "TutorialEnemy.h"
#include "CollisionTypeIdDef.h"
#include "ImGuiManager.h"

#include <cmath>

/// -------------------------------------------------------------
///							コンストラクタ
/// -------------------------------------------------------------
Weapon::Weapon()
{
	// シリアルナンバーを振る
	serialNumber_ = nextSerialNumber_;
	// 次のシリアルナンバーに1を足す
	++nextSerialNumber_;
}


/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void Weapon::Initialize()
{
	// IDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kWeapon));

	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("Voxel_Weapon.gltf");

	if (player_) {
		object3D_->SetTranslate(player_->GetPosition());
		object3D_->SetRotate(player_->GetRotation());
		object3D_->SetScale(player_->GetScale());
	}
	else if (tutorialPlayer_) {
		object3D_->SetTranslate(tutorialPlayer_->GetPosition());
		object3D_->SetRotate(tutorialPlayer_->GetRotation());
		object3D_->SetScale(tutorialPlayer_->GetScale());
	}

	radius_ = 2.0f;

	attackTime_ = 0;
	attackRotationAngle_ = 0.0f;
}


/// -------------------------------------------------------------
///						　更新処理
/// -------------------------------------------------------------
void Weapon::Update()
{
	// プレイヤーが存在する場合
	if (player_) {
		// 位置
		position_ = player_->GetPosition();
		// 回転
		rotation_ = player_->GetRotation();
		// スケール
		scale_ = player_->GetScale();
	}
	//プレイヤーがチュートリアル用になっている時
	else if (tutorialPlayer_) {

		// プレイヤーの位置を取得
		position_ = tutorialPlayer_->GetPosition();
		// プレイヤーの回転を取得
		rotation_ = tutorialPlayer_->GetRotation();
		// プレイヤーのスケールを取得
		scale_ = tutorialPlayer_->GetScale();
	}

	// プレイヤーの向いている方向に武器を配置
	Vector3 offset = { distance_ * std::cos(attackRotationAngle_), 0.0f, distance_ * std::sin(attackRotationAngle_) };
	Vector3 weaponPosition = position_ - offset;

	/*------武器が回転するようにy軸だけ変更------*/
	object3D_->SetTranslate(weaponPosition);
	object3D_->SetRotate({ rotation_.x,attackRotationAngle_,rotation_.z });
	object3D_->SetScale(scale_);
	object3D_->Update();
	SetRadius(radius_);
}


/// -------------------------------------------------------------
///							描画処理
/// -------------------------------------------------------------
void Weapon::Draw()
{
	object3D_->Draw();


}


/// -------------------------------------------------------------
///							ImGui描画処理
/// -------------------------------------------------------------
void Weapon::DrawImGui()
{
	ImGui::Begin("Weapon");
	ImGui::Text("Weapon");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("Rotation Speed", &rotationSpeed_, 0.0f, 5.0f);
	ImGui::End();
}


/// -------------------------------------------------------------
///							攻撃処理
/// -------------------------------------------------------------
void Weapon::Attack()
{
	/*------攻撃時間をカウント------*/
	attackTime_++;
	/*------攻撃時間が最大値を超えたら攻撃を終了------*/
	if (attackTime_ > attackMaxTime_) {
		isAttack_ = false;
		attackTime_ = 0;
		attackRotationAngle_ = 0.0f;
	}
	else {
		/*------回転角度を更新------*/
		attackRotationAngle_ += (2.0f * std::numbers::pi_v<float> *rotationSpeed_) / attackMaxTime_;
	}
}


/// -------------------------------------------------------------
///							衝突判定
/// -------------------------------------------------------------
void Weapon::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();

	// 武器が敵に当たった場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy))
	{
		Enemy* enemy = static_cast<Enemy*>(other);
		TutorialEnemy* tutorialEnemy = static_cast<TutorialEnemy*>(other);

		uint32_t serialNumber = {};

		if (enemy) {
			enemy->GetSerialNumber();
		}
		else if (tutorialEnemy) {
			serialNumber = tutorialEnemy->GetSerialNumber();
		}

		// 敵が無敵状態でない場合
		if (!enemy->GetIsInvincible() || !tutorialEnemy->GetIsInvincible()) {
			isEnemyHit_ = true; // 敵に当たったフラグを立てる
		}

		// 接触記録があれば何もせずに抜ける
		if (contactRecord_.Check(serialNumber)) return;

		// 接触記録に追加
		contactRecord_.Add(serialNumber);

		// 敵にダメージを与える
		enemy->SetHp(enemy->GetHp() - 1);

		// 敵の位置にパーティクルを生成
	}
	else {
		isEnemyHit_ = false; // 敵に当たったフラグを解除
	}


	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet))
	{

	}
}


/// -------------------------------------------------------------
///				中心座標を取得する純粋仮想関数
/// -------------------------------------------------------------
Vector3 Weapon::GetCenterPosition() const
{
	// ローカル座標でのオフセット
	const Vector3 offset = { distance_ * std::cos(attackRotationAngle_), 0.0f, distance_ * std::sin(attackRotationAngle_) };
	// ワールド座標に変換
	Vector3 worldPosition = position_ - offset;

	return worldPosition;
}
