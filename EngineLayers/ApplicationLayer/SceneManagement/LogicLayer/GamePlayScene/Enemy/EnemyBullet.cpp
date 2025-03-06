#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() {
}

EnemyBullet::~EnemyBullet() {
}

void EnemyBullet::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// オブジェクトの生成・初期化
	objectBullet_ = std::make_unique<Object3D>();
	objectBullet_->Initialize("sphere.gltf");
}

void EnemyBullet::Update() {

	// 移動
	Move();

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
	velocity_ = direction_ * 0.01f;

	// 速度を位置に加算
	worldTransform_.translate_ += velocity_;
}

void EnemyBullet::OnCollision(Collider* other) {
}

Vector3 EnemyBullet::GetCenterPosition() const {
	return Vector3();
}
