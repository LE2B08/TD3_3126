#include "Hook.h"
#include "CollisionTypeIdDef.h"
#include "Input.h"
#include "Wireframe.h"

#include <imgui.h>

#undef max
#undef min
void Hook::Initialize() {
	// フックの初期化
	isExtending_ = false;
	isThrowing_ = false;

	endPos_ = playerPosition_;

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kHook));
}
void Hook::Update() {
	// 開始位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	// フックが投げられている場合、終了位置を徐々に更新
	if (isThrowing_) {
		Vector3 direction = potentialEndPos - endPos_;
		float distance = Vector3::Length(direction);
		float moveStep = 1.0f; // 移動ステップ

		// 終了位置に到達したらフラグを更新
		if (distance < moveStep) {
			endPos_ = potentialEndPos;
			isActive_ = true;
			isThrowing_ = false;
		} else {
			direction = Vector3::Normalize(direction);
			endPos_ += direction * moveStep;
		}
	}
}

void Hook::Draw() {
		// 描画
		Wireframe::GetInstance()->DrawLine(startPos_, endPos_, {1.0f, 1.0f, 1.0f, 1.0f});
	
}
void Hook::Throw() {
	// フックの開始位置をプレイヤーの位置に設定
	endPos_ = playerPosition_;
	// フックの終了位置を計算（壁に当たるまでの数値にする）
	maxDistance_ = 50.0f;
	// プレイヤーの向きからフックの方向ベクトルを計算
	direction_ = Vector3{cos(playerRotation_.y), 0.0f, sin(playerRotation_.y)};
	// フックの終了位置を計算
	potentialEndPos = playerPosition_ - direction_ * maxDistance_;

	// 壁に当たるまでの距離を計算
	if (potentialEndPos.x < minMoveLimit_.x) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.x - minMoveLimit_.x) / direction_.x);
	} else if (potentialEndPos.x > maxMoveLimit_.x) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.x - maxMoveLimit_.x) / direction_.x);
	}
	if (potentialEndPos.z < minMoveLimit_.z) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.z - minMoveLimit_.z) / direction_.z);
	} else if (potentialEndPos.z > maxMoveLimit_.z) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.z - maxMoveLimit_.z) / direction_.z);
	}

	// フックの終了位置を再計算
	potentialEndPos = playerPosition_ - direction_ * maxDistance_;

	// フックの現在位置を開始位置に設定
	currentPos_ = startPos_;
	// フックの開始時間を記録
	startTime_ = std::chrono::steady_clock::now();
	// フックを投げるフラグを設定
	isThrowing_ = true;
	isActive_ = false;
}

void Hook::Move() {

	// フックの方向ベクトルを計算
	Vector3 direction = endPos_ - startPos_;
	float distance = Vector3::Length(direction);

	// フックの位置に到達したらフックを非アクティブにする
	if (distance < speed_ * 0.016f) { // 0.016fは1フレームの時間（約60FPS）
		isActive_ = false;
	} else {
		// フックの方向に向かって移動
		direction.Normalize(direction);
		Vector3 newPosition = playerPosition_ + direction * speed_ * 0.016f; // 0.016fは1フレームの時間（約60FPS）

		// 壁に触れたらそれ以上ポジションを追加しない
		if (newPosition.x < minMoveLimit_.x || newPosition.x > maxMoveLimit_.x || newPosition.z < minMoveLimit_.z || newPosition.z > maxMoveLimit_.z) {
			isActive_ = false;
		}
	}
}

void Hook::Extend() {

	// フックの方向ベクトルを計算
	direction_ = endPos_ - startPos_;

	// フックの現在位置を更新
	currentPos_ += direction_ * speed_ * 0.016f; // 0.016fは1フレームの時間（約60FPS）

	// フックが終了位置に到達したらフックの伸びを止める
	if ((currentPos_ - startPos_).Length((currentPos_ - startPos_)) >= (endPos_ - startPos_).Length((endPos_ - startPos_))) {
		currentPos_ = endPos_;
	}
}

void Hook::OnCollision(Collider* other) {

	// 種別IDを種別
	uint32_t typeID = other->GetTypeID();

	// フックがアクティブで、敵と衝突した場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		
	// フックの終了位置を計算
		//Vector3 enemyPosition = other->GetCenterPosition();
		//direction_ = Vector3::Normalize(enemyPosition - playerPosition_);
		//maxDistance_ = Vector3::Distance(playerPosition_, enemyPosition);
		//potentialEndPos = playerPosition_ + direction_ * maxDistance_;

		//// フックの終了位置を設定
		//endPos_ = potentialEndPos;
		//isActive_ = true;
		//isThrowing_ = false;

		ImGui::Begin("Hook");
		ImGui::Text("EnemyHit");
		ImGui::End();

	}
}

void Hook::ShowImGui() {

	ImGui::Begin("Hook");

	ImGui::Checkbox("IsActive", &isActive_);

	ImGui::DragFloat3("StartPos", &startPos_.x, 0.1f);
	ImGui::DragFloat3("CurrentPos", &currentPos_.x, 0.1f);
	ImGui::DragFloat3("EndPos", &endPos_.x, 0.1f);
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.1f);
	ImGui::DragFloat3("Acceleration", &acceleration_.x, 0.1f);
	ImGui::DragFloat3("AngularVelocity", &angularVelocity_.x, 0.1f);
	ImGui::DragFloat3("Direction", &direction_.x, 0.1f);
	ImGui::DragFloat("MaxDistance", &maxDistance_, 0.1f);

	ImGui::End();
}

Vector3 Hook::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f}; // プレイヤーの中心を考慮
	Vector3 worldPosition = endPos_ + offset;
	return worldPosition;
}
