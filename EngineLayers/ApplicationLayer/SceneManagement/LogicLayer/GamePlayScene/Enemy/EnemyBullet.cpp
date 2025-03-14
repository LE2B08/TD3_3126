#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() {
}

EnemyBullet::~EnemyBullet() {
}

void EnemyBullet::Initialize() {

	isAlive_ = true;	
	lifeFrame_ = 180;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// オブジェクトの生成・初期化
	objectBullet_ = std::make_unique<Object3D>();
	objectBullet_->Initialize("sphere.gltf");
	objectBullet_->SetScale({ 0.4f, 0.4f, 0.4f });

	SetRadius(0.4f);
}

void EnemyBullet::Update() {

	// 移動
	Move();

	if (lifeFrame_ > 0) {
		lifeFrame_--;
	}
	else {
		isAlive_ = false;
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
	velocity_ = direction_ * 0.01f;

	// 速度を位置に加算
	worldTransform_.translate_ += velocity_;
}

void EnemyBullet::OnCollision(Collider* other) {
}

Vector3 EnemyBullet::GetCenterPosition() const {
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // エネミーの中心を考慮
	Vector3 worldPosition = worldTransform_.translate_ + offset;
	return worldPosition;
}
