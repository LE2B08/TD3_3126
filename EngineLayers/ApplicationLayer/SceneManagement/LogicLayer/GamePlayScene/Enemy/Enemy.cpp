#include "Enemy.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "Vector3.h"
#include <algorithm>
#include <imgui.h>
#include <Wireframe.h>
#include "CollisionTypeIdDef.h"


Enemy::Enemy() {
	serialNumber_ = nextSerialNumber_;
	nextSerialNumber_++;
}

Enemy::~Enemy() {
}

void Enemy::Initialize()
{
	// オブジェクトの生成・初期化
	objectEnemy_ = std::make_unique<Object3D>();
	objectEnemy_->Initialize("sphere.gltf");

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
}

void Enemy::Update() {

	// 状態の変更がリクエストされている場合
	if (requestBehavior_) {

		// 状態を変更する
		behavior_ = requestBehavior_.value();

		// 状態ごとの初期化を行う
		switch (behavior_) {

		case Enemy::Behavior::Normal:
			BehaviorNormalInitialize();
			break;

		case Enemy::Behavior::Attack:
			BehaviorAttackInitialize();
			break;

		case Enemy::Behavior::Leave:
			BehaviorLeaveInitialize();
			break;

		default:
			break;
		}

		// リクエストをクリア
		requestBehavior_ = std::nullopt;
	}

	// 状態ごとの更新を行う
	switch (behavior_) {

	case Enemy::Behavior::Normal:
		BehaviorNormalUpdate();
		break;

	case Enemy::Behavior::Attack:
		BehaviorAttackUpdate();
		break;

	case Enemy::Behavior::Leave:
		BehaviorLeaveUpdate();
		break;

	default:
		break;
	}

	// Y軸回転のみ対応した向きを設定
	const float direction = atan2(worldTransform_.translate_.z - player_->GetPosition().z, worldTransform_.translate_.x - player_->GetPosition().x);

	// 向きを設定
	worldTransform_.rotate_.y = direction;

	// 弾の削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {

		// 弾が死んでいる場合
		if (!bullet->IsAlive()) {

			// リセット
			bullet.reset();
			return true;
		}

		return false;
		});

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

	// ワイヤーフレームの処理
	Wireframe::GetInstance()->DrawCircle(worldTransform_.translate_, 10.0f, 32, { 1.0f, 1.0f, 1.0f,1.0f });

	// Object3Dの更新
	objectEnemy_->SetRotate(worldTransform_.rotate_);
	objectEnemy_->SetTranslate(worldTransform_.translate_);
	objectEnemy_->Update();
}

void Enemy::Draw()
{
	// 描画
	objectEnemy_->Draw();

	// 弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

void Enemy::Move() {

	// 速度を位置に加算
	worldTransform_.translate_ += velocity_;

	// 移動制限
	worldTransform_.translate_.x = std::clamp(worldTransform_.translate_.x, minMoveLimit_.x, maxMoveLimit_.x);
	worldTransform_.translate_.z = std::clamp(worldTransform_.translate_.z, minMoveLimit_.z, maxMoveLimit_.z);
}

void Enemy::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::DragFloat3("Rotate", &worldTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
	ImGui::DragFloat3("Position", &worldTransform_.translate_.x, 0.01f);
	ImGui::DragInt("AttackCount", reinterpret_cast<int*>(&attackCount_));

	// 状態の表示
	switch (behavior_) {

	case Behavior::Normal:
		ImGui::Text("Behavior: Normal");
		break;

	case Behavior::Attack:
		ImGui::Text("Behavior: Attack");
		break;

	case Behavior::Leave:
		ImGui::Text("Behavior: Leave");
		break;

	default:
		break;
	}

	// ボタンを押したら攻撃状態にする
	if (ImGui::Button("Attack")) {
		requestBehavior_ = Behavior::Attack;
	}

	ImGui::End();
}

void Enemy::OnCollision(Collider* other)
{

}

Vector3 Enemy::GetCenterPosition() const
{
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // エネミーの中心を考慮
	Vector3 worldPosition = worldTransform_.translate_ + offset;
	return worldPosition;
}

void Enemy::BehaviorNormalInitialize() {
}

void Enemy::BehaviorNormalUpdate() {

	// プレイヤーとの距離を計算
	const float distance = Vector3::Length(player_->GetPosition() - worldTransform_.translate_);

	// 一定距離まで近づいたら離脱状態にする
	if (distance < maxDistance_) {
		requestBehavior_ = Behavior::Leave;
	}
}

void Enemy::BehaviorAttackInitialize() {

	// 攻撃間隔を設定
	attackInterval_ = 300;

	// 発射回数をリセット
	attackCount_ = 0;
}

void Enemy::BehaviorAttackUpdate() {

	// 射撃
	if (attackInterval_ <= 0 && attackInterval_ < 5) {

		// 弾を生成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();

		// 弾の初期化
		bullet->Initialize();

		// 弾の位置を設定
		bullet->SetPosition(worldTransform_.translate_);

		// プレイヤーの位置を向きを設定
		const Vector3 direction = player_->GetPosition() - worldTransform_.translate_;

		// 弾に向きを設定
		bullet->SetDirection(direction);

		// 弾をリストに追加
		bullets_.push_back(std::move(bullet));

		// 攻撃回数をカウント
		attackCount_++;

		// 攻撃間隔をリセット
		attackInterval_ = 300;
	}
	else {

		// 攻撃間隔を減らす
		attackInterval_--;
	}

	// 攻撃回数が5回になったら通常状態にする
	if (attackCount_ >= 5) {
		requestBehavior_ = Behavior::Normal;
	}
}

void Enemy::BehaviorLeaveInitialize() {

	// プレイヤーの位置を取得
	const Vector3 playerPosition = player_->GetPosition();

	// 向きを180度回転
	leaveDirection_ = -worldTransform_.rotate_;
}

void Enemy::BehaviorLeaveUpdate() {

	// 速度に向きを設定
	velocity_ = leaveDirection_ * 0.01f;

	// プレイヤーの位置を向きを設定
	const Vector3 direction = player_->GetPosition() - worldTransform_.translate_;

	// プレイヤーとの距離を計算
	const float distance = Vector3::Length(direction);

	// 一定距離まで離れたら攻撃状態にする
	if (distance > maxDistance_) {
		requestBehavior_ = Behavior::Attack;
	}
}
