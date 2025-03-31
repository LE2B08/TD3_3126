#include "EnemyBullet.h"
#include "CollisionTypeIdDef.h"

EnemyBullet::EnemyBullet() {

	// シリアルナンバーを設定
	serialNumber_ = nextSerialNumber_;

	// 次のシリアルナンバーを設定
	nextSerialNumber_++;
}

EnemyBullet::~EnemyBullet() {
}

void EnemyBullet::Initialize() {

	// IDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet));

	isAlive_ = true;
	lifeFrame_ = 180;

	isMove_ = false;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// オブジェクトの生成・初期化
	objectBullet_ = std::make_unique<Object3D>();
	objectBullet_->Initialize("sphere.gltf");
	objectBullet_->SetScale({ 0.4f, 0.4f, 0.4f });

	SetRadius(0.4f);
}

void EnemyBullet::Update() {

	// 移動できるときだけ
	if (isMove_) {

		// 移動
		Move();

		// 生存フレームの更新
		if (lifeFrame_ > 0) {
			lifeFrame_--;
		}
		else {
			isAlive_ = false;
		}
	}

	// WorldTransformの更新
	worldTransform_.Update();

	// Object3Dの更新
	objectBullet_->SetTranslate(worldTransform_.translate_);
	objectBullet_->Update();
}

void EnemyBullet::Draw() {

	// 描画
	objectBullet_->Draw();
}

void EnemyBullet::Move() {

	// 速度に向きを設定
	velocity_ = direction_ * speed_;

	// 速度を位置に加算
	worldTransform_.translate_ += velocity_;
}

void EnemyBullet::OnCollision(Collider* other) {

	// Idを取得
	uint32_t typeID = other->GetTypeID();

	// 弾同時の衝突だった場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {

		// 何も処理せずに終了
		return;
	}

	// それ以外は死亡
	isAlive_ = false;
}

Vector3 EnemyBullet::GetCenterPosition() const {
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // エネミーの中心を考慮
	Vector3 worldPosition = worldTransform_.translate_ + offset;
	return worldPosition;
}
