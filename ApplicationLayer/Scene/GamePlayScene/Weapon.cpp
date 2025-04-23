#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"
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

	object3D_->SetTranslate(player_->GetPosition());
	object3D_->SetRotate(player_->GetRotation());
	object3D_->SetScale(player_->GetScale());

	attackTime_ = 0;
	attackRotationAngle_ = 0.0f;

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	// パーティクルグループの追加
	particleManager_->CreateParticleGroup("WeaponHitParticles", "Resources/uvChecker.png");

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(particleManager_, "WeaponHitParticles");
}


/// -------------------------------------------------------------
///						　更新処理
/// -------------------------------------------------------------
void Weapon::Update()
{
	// 位置
	position_ = player_->GetPosition();
	// 回転
	rotation_ = player_->GetRotation();
	// スケール
	scale_ = player_->GetScale();

	// プレイヤーの向いている方向に武器を配置
	Vector3 offset = { distance_ * std::cos(attackRotationAngle_), 0.0f, distance_ * std::sin(attackRotationAngle_) };
	Vector3 weaponPosition = position_ - offset;

	/*------武器が回転するようにy軸だけ変更------*/
	object3D_->SetTranslate(weaponPosition);
	object3D_->SetRotate({ rotation_.x,attackRotationAngle_,rotation_.z });
	object3D_->SetScale(scale_);
	object3D_->Update();

	//// 攻撃中の処理
	if (isEnemyHit_) {
		// パーティクルエミッターの位置を武器の位置に設定
		particleEmitter_->SetPosition(weaponPosition);
		// 斬撃のパーティクルを生成
		particleEmitter_->Update(1.0f, ParticleEffectType::Slash); // deltaTime は 0 で呼び出し
	}

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
		uint32_t serialNumber = enemy->GetSerialNumber();

		// 敵が無敵状態でない場合
		if (!enemy->GetIsInvincible()) {
			isEnemyHit_ = true; // 敵に当たったフラグを立てる
		}

		// 接触記録があれば何もせずに抜ける
		if (contactRecord_.Check(serialNumber)) return;

		// 接触記録に追加
		contactRecord_.Add(serialNumber);

		// 敵にダメージを与える
		enemy->SetHp(enemy->GetHp() - 1);

		// 敵の位置にパーティクルを生成
	} else {
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
