#include "Hook.h"
#include "Input.h"
#include "Wireframe.h"

#undef max
#undef min

void Hook::Initialize() {

	// フックの初期化
	isExtending_ = false;
}

void Hook::Update() {

	// 開始位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;
}

void Hook::Draw() {

	if (isActive_) {
		// 描画
		Wireframe::GetInstance()->DrawLine(startPos_, endPos_, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Hook::Throw() {

	// フックの終了位置を計算（壁に当たるまでの数値にする）
	maxDistance_ = 50.0f;
	// プレイヤーの向きからフックの方向ベクトルを計算
	direction_ = Vector3{ cos(playerRotation_.y), 0.0f, sin(playerRotation_.y) };
	// フックの終了位置を計算
	Vector3 potentialEndPos = playerPosition_ - direction_ * maxDistance_;

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

	// フックの終了位置を設定
	endPos_ = potentialEndPos;
	// フックの現在位置を開始位置に設定
	currentPos_ = startPos_;
	// フックの開始時間を記録
	startTime_ = std::chrono::steady_clock::now();
	// フックをアクティブにする
	isActive_ = true;
}

void Hook::Move() {

	// フックの方向ベクトルを計算
	Vector3 direction = endPos_ - startPos_;
	float distance = Vector3::Length(direction);

	// フックの位置に到達したらフックを非アクティブにする
	if (distance < speed_ * 0.016f) { // 0.016fは1フレームの時間（約60FPS）
		isActive_ = false;
	}
	else {
		// フックの方向に向かって移動
		direction.Normalize(direction);
		Vector3 newPosition = playerPosition_ + direction * speed_ * 0.016f; // 0.016fは1フレームの時間（約60FPS）

		// 壁に触れたらそれ以上ポジションを追加しない
		if (newPosition.x < minMoveLimit_.x || newPosition.x > maxMoveLimit_.x || newPosition.z < minMoveLimit_.z || newPosition.z > maxMoveLimit_.z) {
			isActive_ = false;
		}

		///===================================
		/// フック使用時の弧の移動
		///

		///
		/// 壁の上辺
		///
		if (endPos_.z >= maxMoveLimit_.z) {
			// フックの終点が上辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = playerPosition_ - endPos_;
			// フックの終点から中心までの距離を計算
			float radius = toCenter.Length(toCenter);
			// フックの終点から中心までの角度を計算
			float angle = atan2(toCenter.z, toCenter.x);
			float angularSpeed = 3.0f; // 角速度（調整可能）

			// 左スティックの入力を取得
			Vector2 leftStick = Input::GetInstance()->GetLeftStick();

			// 左スティックの入力に応じて角度を変更
			if (leftStick.x < -0.1f) {
				// 右に移動
				angle -= angularSpeed * 0.016f;

			}
			else if (leftStick.x > 0.1f) {
				// 左に移動
				angle += angularSpeed * 0.016f;
			}

			// 新しい位置を計算
			playerPosition_.x = endPos_.x + radius * cos(angle);
			playerPosition_.z = endPos_.z + radius * sin(angle);
		}

		///
		/// 壁の下辺
		///
		if (endPos_.z <= minMoveLimit_.z) {
			// フックの終点が下辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = playerPosition_ - endPos_;
			// フックの終点から中心までの距離を計算
			float radius = toCenter.Length(toCenter);
			// フックの終点から中心までの角度を計算
			float angle = atan2(toCenter.z, toCenter.x);
			float angularSpeed = 3.0f; // 角速度（調整可能）

			// 左スティックの入力を取得
			Vector2 leftStick = Input::GetInstance()->GetLeftStick();

			// 左スティックの入力に応じて角度を変更
			if (leftStick.x < -0.1f) {
				// 右に移動
				angle += angularSpeed * 0.016f;

			}
			else if (leftStick.x > 0.1f) {
				// 左に移動
				angle -= angularSpeed * 0.016f;
			}

			// 新しい位置を計算
			playerPosition_.x = endPos_.x + radius * cos(angle);
			playerPosition_.z = endPos_.z + radius * sin(angle);
		}

		///
		/// 壁の左辺
		///
		if (endPos_.x <= minMoveLimit_.x) {
			// フックの終点が左辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = playerPosition_ - endPos_;
			// フックの終点から中心までの距離を計算
			float radius = toCenter.Length(toCenter);
			// フックの終点から中心までの角度を計算
			float angle = atan2(toCenter.z, toCenter.x);
			float angularSpeed = 3.0f; // 角速度（調整可能）
			// 左スティックの入力を取得
			Vector2 leftStick = Input::GetInstance()->GetLeftStick();
			// 左スティックの入力に応じて角度を変更
			if (leftStick.x < -0.1f) {
				// 右に移動
				angle += angularSpeed * 0.016f;
			}
			else if (leftStick.x > 0.1f) {
				// 左に移動
				angle -= angularSpeed * 0.016f;
			}
			// 新しい位置を計算
			playerPosition_.x = endPos_.x + radius * cos(angle);
			playerPosition_.z = endPos_.z + radius * sin(angle);
		}

		///
		/// 壁の右辺
		///
		if (endPos_.x >= maxMoveLimit_.x) {
			// フックの終点が右辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = playerPosition_ - endPos_;
			// フックの終点から中心までの距離を計算
			float radius = toCenter.Length(toCenter);
			// フックの終点から中心までの角度を計算
			float angle = atan2(toCenter.z, toCenter.x);
			float angularSpeed = 3.0f; // 角速度（調整可能）
			// 左スティックの入力を取得
			Vector2 leftStick = Input::GetInstance()->GetLeftStick();
			// 左スティックの入力に応じて角度を変更
			if (leftStick.x < -0.1f) {
				// 右に移動
				angle -= angularSpeed * 0.016f;
			}
			else if (leftStick.x > 0.1f) {
				// 左に移動
				angle += angularSpeed * 0.016f;
			}
			// 新しい位置を計算
			playerPosition_.x = endPos_.x + radius * cos(angle);
			playerPosition_.z = endPos_.z + radius * sin(angle);
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
}

Vector3 Hook::GetCenterPosition() const {
	return startPos_;
}
