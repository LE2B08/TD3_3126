#include "Weapon.h"
#include "CollisionTypeIdDef.h"
#include "Enemy.h"
#include "ImGuiManager.h"
#include "Player.h"

#include <cmath>
#undef max
#undef min
/// -------------------------------------------------------------
///							コンストラクタ
/// -------------------------------------------------------------
Weapon::Weapon() {
	// シリアルナンバーを振る
	serialNumber_ = nextSerialNumber_;
	// 次のシリアルナンバーに1を足す
	++nextSerialNumber_;
}

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void Weapon::Initialize() {
	// IDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kWeapon));

	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("Voxel_Weapon.gltf");

	object3D_->SetTranslate(player_->GetPosition());
	object3D_->SetRotate(player_->GetRotation());
	object3D_->SetScale(player_->GetScale());

	// エフェクトスプライト
	// 三枚用意する

	
	
	// スプライトのワールドトランスフォームを初期化
	effectSpritesWorldTransform_.Initialize();
	effectSpritesWorldTransform_.translate_ = {0.0f, 0.0f, 0.0f};
	effectSpritesWorldTransform_.scale_ = {0.005f, 0.1f, 0.1f};
	effectSpritesWorldTransform_.rotate_ = {0.0f, 0.0f, 0.0f};

	

	attackTime_ = 0;
	attackRotationAngle_ = 0.0f;
}

/// -------------------------------------------------------------
///						　更新処理
/// -------------------------------------------------------------

void Weapon::Update() {
	// 位置
	position_ = player_->GetPosition();
	// 回転
	rotation_ = player_->GetRotation();
	// スケール
	scale_ = player_->GetScale();

	// プレイヤーの向いている方向に武器を配置
	Vector3 offset = {distance_ * std::cos(attackRotationAngle_), 0.0f, distance_ * std::sin(attackRotationAngle_)};
	Vector3 weaponPosition = position_ - offset;

	/*------武器が回転するようにy軸だけ変更------*/
	object3D_->SetTranslate(weaponPosition);
	object3D_->SetRotate({rotation_.x, attackRotationAngle_, rotation_.z});
	object3D_->SetScale(scale_);
	object3D_->SetCamera(camera_);
	object3D_->Update();

	// ワールド座標をスクリーン座標に変換
	Vector3 screenPosition = WorldToScreen(effectSpritesWorldTransform_.translate_, camera_);

	
	// effectSprite
	if (isHitEnemy_) {
		
	}

	
}

/// -------------------------------------------------------------
///							描画処理
/// -------------------------------------------------------------
void Weapon::Draw() { object3D_->Draw(); }

///--------------------------------------------------------------
///						Effect描画処理
/// -------------------------------------------------------------

void Weapon::DrawEffect() {
	// スプライトの描画

	
}
/// -------------------------------------------------------------
///							ImGui描画処理
/// -------------------------------------------------------------
void Weapon::DrawImGui() {
	ImGui::Begin("Weapon");
	ImGui::Text("Weapon");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("Rotation Speed", &rotationSpeed_, 0.0f, 5.0f);
	ImGui::End();

	ImGui::Begin("WeaponEffect");
	ImGui::Text("WeaponEffect");
	ImGui::Text("IsHitEnemy: %d", isHitEnemy_);
	ImGui::SliderFloat2("Position", &effectSpritesWorldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat("Rotation", &effectSpritesWorldTransform_.rotate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat2("Scale", &effectSpritesWorldTransform_.scale_.x, 0.0f, 10.0f);

	
}

/// -------------------------------------------------------------
///							攻撃処理
/// -------------------------------------------------------------
void Weapon::Attack() {
	/*------攻撃時間をカウント------*/
	attackTime_++;
	/*------攻撃時間が最大値を超えたら攻撃を終了------*/
	if (attackTime_ > attackMaxTime_) {
		isAttack_ = false;
		attackTime_ = 0;
		attackRotationAngle_ = 0.0f;
	} else {
		/*------回転角度を更新------*/
		attackRotationAngle_ += (2.0f * std::numbers::pi_v<float> * rotationSpeed_) / attackMaxTime_;
	}
}

/// -------------------------------------------------------------
///							衝突判定
/// -------------------------------------------------------------
void Weapon::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	Enemy* enemy = static_cast<Enemy*>(other);

	// 武器が敵に当たった場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		// 敵が無敵時間でなければ
		if (!enemy->GetIsInvincible()) {
			// フラグを立てる
			isHitEnemy_ = true;
		}

		// 接触記録があれば何もせずに抜ける
		if (contactRecord_.Check(enemy->GetSerialNumber())) {
			return;
		}

		// 接触記録に追加
		contactRecord_.Add(enemy->GetSerialNumber());

		// 敵にダメージを与える
		enemy->SetHp(enemy->GetHp() - 1);
	}

	// 武器が敵の弾に当たった場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {
	}
}

/// -------------------------------------------------------------
///				中心座標を取得する純粋仮想関数
/// -------------------------------------------------------------
Vector3 Weapon::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = {distance_ * std::cos(attackRotationAngle_), 0.0f, distance_ * std::sin(attackRotationAngle_)};
	// ワールド座標に変換
	Vector3 worldPosition = position_ - offset;

	return worldPosition;
}



/// -------------------------------------------------------------
/// ワールド座標をスクリーン座標に変換する関数
/// -------------------------------------------------------------
Vector3 Weapon::WorldToScreen(const Vector3& worldPosition, Camera* camera) {
	// カメラのビュー行列とプロジェクション行列を取得
	const Matrix4x4& viewMatrix = camera->GetViewMatrix();
	const Matrix4x4& projectionMatrix = camera->GetProjectionMatrix();

	// ワールド座標をビュー空間に変換
	Vector4 viewSpacePosition = viewMatrix * Vector4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);

	// ビュー空間をプロジェクション空間に変換
	Vector4 clipSpacePosition = projectionMatrix * viewSpacePosition;

	// 正規化デバイス座標系（NDC）に変換
	Vector3 ndcPosition = {clipSpacePosition.x / clipSpacePosition.w, clipSpacePosition.y / clipSpacePosition.w, clipSpacePosition.z / clipSpacePosition.w};

	// スクリーン座標に変換
	Vector3 screenPosition = {
	    (ndcPosition.x * 0.5f + 0.5f) * 1280,   // スクリーン幅にスケール
	    (-ndcPosition.y * 0.5f + 0.5f) * 720, // スクリーン高さにスケール（Y軸反転）
	    ndcPosition.z};

	return screenPosition;
}