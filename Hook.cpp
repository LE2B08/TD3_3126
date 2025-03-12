#include "Hook.h"
#include "CollisionTypeIdDef.h"
#include "Input.h"
#include "Wireframe.h"

#include <imgui.h>

#undef max
#undef min

void Hook::Initialize() {

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kHook));
}

void Hook::Update() {

	// 状態がリクエストされていたら
	if (requestState_) {
		
		// 状態を変更
		state_ = requestState_.value();

		// 状態ごとの初期化
		switch (state_) {

		case State::Throw:
			ThrowInitialize();
			break;

		case State::Pull:
			PullInitialize();
			break;

		default:
			break;
		}

		// リクエストをクリア
		requestState_ = std::nullopt;
	}

	// 状態ごとの更新
	switch (state_) {

	case State::Throw:
		ThrowUpdate();
		break;

	case State::Pull:
		PullUpdate();
		break;

	default:
		break;
	}
}

void Hook::Draw() {

	// 描画
	Wireframe::GetInstance()->DrawLine(startPos_, endPos_, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void Hook::OnCollision(Collider* other) {

	// 種別IDを種別
	uint32_t typeID = other->GetTypeID();

	// フックがアクティブで、敵と衝突した場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {

		ImGui::Begin("Hook");
		ImGui::Text("EnemyHit");
		ImGui::End();
	}
}

void Hook::ShowImGui() {

	ImGui::Begin("Hook");

	ImGui::Checkbox("IsActive", &isActive_);

	ImGui::DragFloat3("StartPos", &startPos_.x, 0.1f);
	ImGui::DragFloat3("EndPos", &endPos_.x, 0.1f);
	ImGui::DragFloat3("PotentialEndPos", &potentialEndPos.x, 0.1f);
	ImGui::DragFloat3("PlayerRotation", &playerRotation_.x, 0.1f);
	ImGui::DragFloat3("PlayerPosition", &playerPosition_.x, 0.1f);
	ImGui::DragFloat3("Direction", &direction_.x, 0.1f);

	// 状態の表示
	switch (state_) {

	case State::Idle:
		ImGui::Text("State: Idle");
		break;

	case State::Throw:
		ImGui::Text("State: Throw");
		break;

	case State::Pull:
		ImGui::Text("State: Pull");
		break;

	default:
		break;
	}

	ImGui::End();
}

void Hook::ThrowInitialize() {

	// 始点をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	// フックの終点を視点の位置と同じにする
	endPos_ = startPos_;

	// フックの方向ベクトルを計算
	direction_ = Vector3{ cos(playerRotation_.y), 0.0f, sin(playerRotation_.y) };

	// フックの終了位置を計算（壁に当たるまでの数値にする）
	maxDistance_ = 50.0f;

	// フックの終了位置を計算
	potentialEndPos = startPos_ - direction_ * maxDistance_;

	// 壁に当たるまでの距離を計算
	if (potentialEndPos.x < minMoveLimit_.x) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.x - minMoveLimit_.x) / direction_.x);
	}
	else if (potentialEndPos.x > maxMoveLimit_.x) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.x - maxMoveLimit_.x) / direction_.x);
	}
	if (potentialEndPos.z < minMoveLimit_.z) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.z - minMoveLimit_.z) / direction_.z);
	}
	else if (potentialEndPos.z > maxMoveLimit_.z) {
		maxDistance_ = std::min(maxDistance_, (playerPosition_.z - maxMoveLimit_.z) / direction_.z);
	}

	// フックの終了位置を再計算
	potentialEndPos = playerPosition_ - direction_ * maxDistance_;

	// フックの現在位置を開始位置に設定
	currentPos_ = startPos_;
	// フックの開始時間を記録
	startTime_ = std::chrono::steady_clock::now();
	// フックを投げるフラグを設定
	isActive_ = false;
}

void Hook::ThrowUpdate() {

	// フックの方向ベクトルを計算
	Vector3 moveDirection = potentialEndPos - endPos_;
	float distance = Vector3::Length(moveDirection);

	// 終了位置に到達したらフラグを更新
	if (distance < speed_ * 0.016f) {
		endPos_ = potentialEndPos;
		isActive_ = true;
		requestState_ = State::Idle; // 待機状態にする
	}
	else {
		Vector3::Normalize(moveDirection);
		endPos_ += moveDirection * speed_ * 0.016f;
	}
}

void Hook::PullInitialize() {

}

void Hook::PullUpdate() {

	// 始点を終点へ徐々に近づける
	Vector3 moveDirection = endPos_ - startPos_;
	float distance = Vector3::Length(moveDirection);

	// 終了位置に到達したらフラグを更新
	if (distance < speed_ * 0.016f) {
		startPos_ = endPos_;
		isActive_ = false;
		requestState_ = State::Idle; // 待機状態にする
	}
	else {
		Vector3::Normalize(moveDirection);
		startPos_ += moveDirection * speed_ * 0.016f;
	}
}

Vector3 Hook::GetCenterPosition() const {
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // プレイヤーの中心を考慮
	Vector3 worldPosition = endPos_ + offset;
	return worldPosition;
}
