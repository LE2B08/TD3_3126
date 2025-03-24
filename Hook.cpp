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
	// フックの位置を初期化
	endPos_ = playerPosition_;

	// フックのコライダーの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kHook));
}
void Hook::Update() {

	// 状態の変更がリクエストされている場合
	if (requestBehavior_) {

		// 状態を変更する
		behavior_ = requestBehavior_.value();

		// 状態ごとの初期化を行う
		switch (behavior_) {
		case Hook::Behavior::None:
			BehaviorNoneInitialize();
			break;
		case Hook::Behavior::Throw:
			BehaviorThrowInitialize();
			break;
		case Hook::Behavior::Extend:
			BehaviorExtendInitialize();
			break;
		case Hook::Behavior::Move:
			BehaviorMoveInitialize();
			break;
		case Hook::Behavior::Back:
			BehaviorBackInitialize();
			break;
		default:
			break;
		}

		// リクエストをクリア
		requestBehavior_ = std::nullopt;
	}

	// 状態ごとの更新を行う
	switch (behavior_) {
	case Hook::Behavior::None:
		BehaviorNoneUpdate();
		break;
	case Hook::Behavior::Throw:
		BehaviorThrowUpdate();
		break;
	case Hook::Behavior::Extend:
		BehaviorExtendUpdate();
		break;
	case Hook::Behavior::Move:
		BehaviorMoveUpdate();
		break;
	case Hook::Behavior::Back:
		BehaviorBackUpdate();
		break;
	default:
		break;
	}
}

void Hook::Draw() {
	// 描画
	Wireframe::GetInstance()->DrawLine(startPos_, endPos_, {1.0f, 1.0f, 1.0f, 1.0f});
}

void Hook::BehaviorNoneInitialize() {
	// フックの初期化
	isExtending_ = false;
	isThrowing_ = false;
	isActive_ = false;

	// フックの位置を初期化
	startPos_ = playerPosition_;
	endPos_ = playerPosition_;
}

void Hook::BehaviorNoneUpdate() {
	///===========================================
	/// 状態を変更
	///
	startPos_ = playerPosition_;
	endPos_ = playerPosition_;
	if (Input::GetInstance()->TriggerButton(9)) {
		requestBehavior_ = Behavior::Throw;
	}
}

void Hook::BehaviorThrowInitialize() {

	isActive_ = false;
	isThrowing_ = false;
	playerVelocity_ = {0.0f, 0.0f, 0.0f};
	playerAcceleration_ = {0.0f, 0.0f, 0.0f};
}

void Hook::BehaviorThrowUpdate() {

	///===========================================
	/// 事前確認
	///

	// フックがアクティブなら何もしない
	if (isActive_) {
		return;
	}
	// フックが敵に当たっている場合
	if (enemyHit_) {
		// フックが敵に当たる前に投げられたフラグを立てる
		hookToEnemyHitBeforeThrow_ = true;
	} else {
		// あたってなければフラグを解除
		hookToEnemyHitBeforeThrow_ = false;
	}

	///===========================================
	/// フックの投げる処理
	///

	// フックの開始位置をプレイヤーの位置に設定
	endPos_ = playerPosition_;
	// プレイヤーとフックの長さ
	maxDistance_ = 100.0f;
	// プレイヤーの向きからフックの方向ベクトルを計算
	direction_ = Vector3{cos(playerRotation_.y), 0.0f, sin(playerRotation_.y)};

	// フックの終了位置を計算
	potentialEndPos = playerPosition_ - direction_ * maxDistance_;

	// プレイヤーとフックの長さ
	// 壁に当たる距離を計算
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

	///===========================================
	/// 状態を変更
	///

	// 伸ばす状態に変更
	requestBehavior_ = Behavior::Extend;
}

void Hook::BehaviorExtendInitialize() {}

void Hook::BehaviorExtendUpdate() {

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
			isThrowing_ = false;
			isActive_ = true;

			// フックの状態を移動に変更
			requestBehavior_ = Behavior::Move;

		} else {
			direction = Vector3::Normalize(direction);
			endPos_ += direction * moveStep;
		}
	}
}

void Hook::BehaviorMoveInitialize() {
	// フックの初期化
	isPulling_ = false;
}

void Hook::BehaviorMoveUpdate() {

	// フックの開始位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	///===========================================
	/// 　キー入力
	///
	if (isPulling_) {
		// フックがアクティブのときにボタンを押すとフックを非アクティブにする
		if (Input::GetInstance()->TriggerButton(15)) {
			// フックを非アクティブにする
			isPulling_ = false;
		}
	} else {
		// フックが非アクティブのときにボタンを押すとフックをアクティブにする
		if (Input::GetInstance()->TriggerButton(15)) {
			// フックを非アクティブにする
			isPulling_ = true;
		}
	}

	if (Input::GetInstance()->TriggerButton(9)) {
		// フックを戻す状態に変更
		requestBehavior_ = Behavior::Back;
	}

	///============================================================================================================
	/// フック使用時の弧の移動

	// 弧の動きは外積を使って計算を一度やってみて

	// 右スティックの入力を取得
	rightStick_ = Input::GetInstance()->GetRightStick();
	// フックの終点から中心へのベクトルを計算
	Vector3 toCenter = playerPosition_ - endPos_;
	// フックの終点から中心までの距離を計算
	float radius = toCenter.Length(toCenter);

	if (isActive_) {
		if (!enemyHit_) {

			///===================================
			/// 壁
			///

			// フックの終点から中心までの角度を計算
			angle = atan2(toCenter.z, toCenter.x);

			// 右スティックの小数点三桁の四捨五入
			rightStick_.x = round(rightStick_.x * 1000) / 1000;
			rightStick_.y = round(rightStick_.y * 1000) / 1000;

			// 右スティックの入力に応じて角度を変更
			// あたっている壁の方向ごとに角度を変更

			/////
			///// 壁の上辺
			/////

			if (endPos_.z >= maxMoveLimit_.z) {
				if (rightStick_.x < -0.1f) {
					angle -= angularSpeed * 0.016f;
					isRightStickLeft = true;
					isRightStickRight = false;
				} else if (rightStick_.x > 0.1f) {
					angle += angularSpeed * 0.016f;
					isRightStickRight = true;
					isRightStickLeft = false;
				}
			}

			/////
			///// 壁の下辺
			/////

			if (endPos_.z <= minMoveLimit_.z) {
				if (rightStick_.x < -0.1f) {
					angle += angularSpeed * 0.016f;
					isRightStickRight = true;
					isRightStickLeft = false;
				} else if (rightStick_.x > 0.1f) {
					angle -= angularSpeed * 0.016f;
					isRightStickLeft = true;
					isRightStickRight = false;
				}
			}

			/////
			///// 壁の左辺
			/////

			if (endPos_.x <= minMoveLimit_.x) {
				if (rightStick_.x < -0.1f) {
					angle += angularSpeed * 0.016f;
					isRightStickRight = true;
					isRightStickLeft = false;
				} else if (rightStick_.x > 0.1f) {
					angle -= angularSpeed * 0.016f;
					isRightStickLeft = true;
					isRightStickRight = false;
				}
			}

			/////
			///// 壁の右辺
			/////

			if (endPos_.x >= maxMoveLimit_.x) {
				if (rightStick_.x < -0.1f) {
					angle -= angularSpeed * 0.016f;
					isRightStickLeft = true;
					isRightStickRight = false;
				} else if (rightStick_.x > 0.1f) {
					angle += angularSpeed * 0.016f;
					isRightStickRight = true;
					isRightStickLeft = false;
				}
			}

			// 右スティックの入力がある場合のみ弧の動きを計算
			if (!Input::GetInstance()->RStickInDeadZone()) {
				angularSpeed = 3.0f; // 角速度（調整可能）

				// 新しい位置を計算
				Vector3 newPosition;
				newPosition.x = endPos_.x + radius * cos(angle);
				newPosition.z = endPos_.z + radius * sin(angle);

				// 外積を使って速度を計算
				Vector3 tangent = Vector3::Cross(Vector3(0, 1, 0), toCenter);
				playerVelocity_ = Vector3::Normalize(tangent) * speed_ * 0.016f;
				playerVelocity_.x = newPosition.x - playerPosition_.x;
				playerVelocity_.z = newPosition.z - playerPosition_.z;

			} else {

				// 右スティックの入力がない場合は
				// フックの長さは固定
				// 先程まで入力していた方向に弧を描くが
				// 少しずつ弧の動きが収まっていく

				// 右スティックが入力されていない場合
				// 先程まで移動していた方向に弧の動きをし、少しずつ減速する

				// 角速度を減速
				angularSpeed *= decelerationRate;

				// 角度を更新
				if (isRightStickRight) {
					angle += angularSpeed * 0.016f;
				} else if (isRightStickLeft) {
					angle -= angularSpeed * 0.016f;
				}

				// 新しい位置を計算
				Vector3 newPosition;
				newPosition.x = endPos_.x + radius * cos(angle);
				newPosition.z = endPos_.z + radius * sin(angle);

				// 外積を使って速度を計算
				Vector3 tangent = Vector3::Cross(Vector3(0, 1, 0), toCenter);
				playerVelocity_ = Vector3::Normalize(tangent) * speed_ * 0.016f;
				playerVelocity_.x = newPosition.x - playerPosition_.x;
				playerVelocity_.z = newPosition.z - playerPosition_.z;

				// 速度が非常に小さくなったら停止
				if (Vector3::Length(playerVelocity_) < 0.01f) {
					playerVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
				}
			}

		} else {
			///===================================
			/// Enemy
			///

			if (enemyHit_) {
				// 右スティックの入力を取得
				rightStick_ = Input::GetInstance()->GetRightStick();
				// フックの終点から中心へのベクトルを計算
				Vector3 toCenter = playerPosition_ - endPos_;
				// フックの終点から中心までの距離を計算
				float radius = Vector3::Length(toCenter);
				// フックの終点から中心までの角度を計算
				angle = atan2(toCenter.z, toCenter.x);

				// 右スティックの小数点三桁の四捨五入
				rightStick_.x = round(rightStick_.x * 1000) / 1000;
				rightStick_.y = round(rightStick_.y * 1000) / 1000;

				// 右スティックの入力に応じて角度を変更
				if (rightStick_.x < -0.1f) {
					angle -= angularSpeed * 0.016f;
					isRightStickLeft = true;
					isRightStickRight = false;
				} else if (rightStick_.x > 0.1f) {
					angle += angularSpeed * 0.016f;
					isRightStickRight = true;
					isRightStickLeft = false;
				}
				// 右スティックの入力がある場合のみ弧の動きを計算
				if (!Input::GetInstance()->RStickInDeadZone()) {
					angularSpeed = 3.0f; // 角速度（調整可能）

					// 新しい位置を計算
					Vector3 newPosition;
					newPosition.x = endPos_.x + radius * cos(angle);
					newPosition.z = endPos_.z + radius * sin(angle);

					// 外積を使って速度を計算
					Vector3 tangent = Vector3::Cross(Vector3(0, 1, 0), toCenter);
					playerVelocity_ = Vector3::Normalize(tangent) * speed_ * 0.016f;
					playerVelocity_.x = newPosition.x - playerPosition_.x;
					playerVelocity_.z = newPosition.z - playerPosition_.z;

				} else {

					// 右スティックの入力がない場合は
					// フックの長さは固定
					// 先程まで入力していた方向に弧を描くが
					// 少しずつ弧の動きが収まっていく

					// 右スティックが入力されていない場合
					// 先程まで移動していた方向に弧の動きをし、少しずつ減速する

					// 角速度を減速
					angularSpeed *= decelerationRate;

					// 角度を更新
					if (isRightStickRight) {
						angle += angularSpeed * 0.016f;
					} else if (isRightStickLeft) {
						angle -= angularSpeed * 0.016f;
					}

					// 新しい位置を計算
					Vector3 newPosition;
					newPosition.x = endPos_.x + radius * cos(angle);
					newPosition.z = endPos_.z + radius * sin(angle);

					// 外積を使って速度を計算
					Vector3 tangent = Vector3::Cross(Vector3(0, 1, 0), toCenter);
					playerVelocity_ = Vector3::Normalize(tangent) * speed_ * 0.016f;
					playerVelocity_.x = newPosition.x - playerPosition_.x;
					playerVelocity_.z = newPosition.z - playerPosition_.z;

					// 速度が非常に小さくなったら停止
					if (Vector3::Length(playerVelocity_) < 0.01f) {
						playerVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
	}

	///========================================================================================
	/// プレイヤーのフック使用時の移動処理
	///

	if (isPulling_) {

		if (isActive_) {

			if (!enemyHit_) {
				///===================================
				/// 壁
				///

				// フックの方向ベクトルを計算
				Vector3 direction = endPos_ - playerPosition_;
				float distance = Vector3::Length(direction);

				// フックの位置に到達したらフックを非アクティブにする
				if (distance < speed_ * 0.016f) { // 0.016fは1フレームの時間（約60FPS）
					playerPosition_ = endPos_;
					isActive_ = false;
					// フックの状態をなしに変更
					requestBehavior_ = Behavior::None;
				} else {
					// フックの方向に向かって移動
					direction.Normalize(direction);
					Vector3 newPosition = playerPosition_ + direction * speed_ * 0.016f; // 0.016fは1フレームの時間（約60FPS）

					// 壁に触れたらそれ以上ポジションを追加しない
					if (newPosition.x < minMoveLimit_.x || newPosition.x > maxMoveLimit_.x || newPosition.z < minMoveLimit_.z || newPosition.z > maxMoveLimit_.z) {
						isActive_ = false;
					} else {
						playerPosition_ = newPosition;
					}
				}

			} else {

				///===============
				/// Enemy
				///

				// フックの方向ベクトルを計算
				Vector3 direction = endPos_ - playerPosition_;
				float distance = Vector3::Length(direction);

				// フックの方向ベクトルを正規化
				direction = Vector3::Normalize(direction);

				// フックの方向に対して垂直なベクトルを計算（外積を使用）
				Vector3 up = {0.0f, 1.0f, 0.0f}; // 上方向のベクトル
				Vector3 perpendicularDirection = Vector3::Cross(up, direction);

				float pullSpeed_ = 30.0f; // 速度（調整可能）
				// フックの方向に向かう速度ベクトルを計算
				playerVelocity_ = direction * pullSpeed_ * 0.016f; // 0.016fは1フレームの時間（約60FPS）

				// EndPosに達したらフックを非アクティブにする
				if (distance < 1.0f) {
					
					isActive_ = false;
					// フックの状態をなしに変更
					requestBehavior_ = Behavior::None;
				}
			}
		}
	}
}

void Hook::BehaviorBackInitialize() {}

void Hook::BehaviorBackUpdate() {
	// フックの終了位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	// フックの方向ベクトルを計算
	Vector3 moveDirection = startPos_ - endPos_;
	float distance = Vector3::Length(moveDirection);

	float backSpeed = 15.0f; // フックの戻る速度

	// 終了位置に到達したらフラグを更新
	if (distance < backSpeed * 0.016f) {
		endPos_ = startPos_;
		isActive_ = true;
		requestBehavior_ = Behavior::None;
	} else {
		Vector3::Normalize(moveDirection);
		endPos_ += moveDirection * backSpeed * 0.016f;
	}
}

void Hook::OnCollision(Collider* other) {
	// 種別IDを種別
	uint32_t typeID = other->GetTypeID();

	// フックがアクティブで、敵と衝突した場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		// 敵の位置を取得して保存
		enemyHitPosition_ = other->GetCenterPosition();
		// フックが敵に当たったフラグを立てる
		enemyHit_ = true;

		// Enemyにあたったときの距離を計算

		// プレイヤーと敵の距離を計算
		distanceToEnemy = Vector3::Distance(playerPosition_, enemyHitPosition_);
		// プレイヤーと敵の距離が最小距離より小さければ最小距離を更新
		if (distanceToEnemy < maxDistance_) {
			// プレイヤーと敵の距離が最小距離より小さければ最小距離を更新
			maxDistance_ = distanceToEnemy;
		}

		// フックを投げる前に敵にあたってなければ
		if (!hookToEnemyHitBeforeThrow_) {

			// フックの終了位置を再計算
			potentialEndPos = playerPosition_ - direction_ * maxDistance_;

			// フックの現在位置を開始位置に設定
			currentPos_ = startPos_;
			// フックの開始時間を記録
			startTime_ = std::chrono::steady_clock::now();
		}

	} else {
		// 敵以外に当たってない場合はフラグを解除
		enemyHit_ = false;
	}
}

void Hook::ShowImGui() {
	// フックの情報を表示
	ImGui::Begin("Hook");

	// 状態の表示
	switch (behavior_) {
	case Hook::Behavior::None:
		ImGui::Text("Behavior: None");
		break;
	case Hook::Behavior::Throw:
		ImGui::Text("Behavior: Throw");
		break;
	case Hook::Behavior::Extend:
		ImGui::Text("Behavior: Extend");
		break;
	case Hook::Behavior::Move:
		ImGui::Text("Behavior: Move");
		break;
	case Hook::Behavior::Back:
		ImGui::Text("Behavior: Back");
		break;
	default:
		break;
	}

	ImGui::Text("rightStick.x: %f", rightStick_.x);
	ImGui::Text("rightStick.y: %f", rightStick_.y);
	// フックが起動してプレイヤーの移動ができるかどうか
	ImGui::Checkbox("IsActive", &isActive_);
	// フックが投げられているかどうか
	ImGui::Checkbox("IsThrowing", &isThrowing_);
	// フックが敵に当たったかどうか
	ImGui::Checkbox("EnemyHit", &enemyHit_);
	// フックを投げる前に敵に当たっていたかどうか
	ImGui::Checkbox("EnemyHitBeforeThrow", &hookToEnemyHitBeforeThrow_);
	// フックの開始位置(基本的プレイヤーの位置)
	ImGui::DragFloat3("StartPos", &startPos_.x, 0.1f);
	// フックの終了位置
	ImGui::DragFloat3("EndPos", &endPos_.x, 0.1f);
	// フックの速度
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.1f);
	// フックの加速度
	ImGui::DragFloat3("Acceleration", &acceleration_.x, 0.1f);
	// フックの角速度
	ImGui::DragFloat3("AngularVelocity", &angularVelocity_.x, 0.1f);
	// フックの方向
	ImGui::DragFloat3("Direction", &direction_.x, 0.1f);
	// フックとプレイヤーの距離
	ImGui::DragFloat("MaxDistance", &maxDistance_, 0.1f);

	ImGui::DragFloat("Speed", &speed_, 0.1f);

	ImGui::DragFloat("AngleSpeed", &angularSpeed, 0.1f);

	ImGui::DragFloat("angle", &angle, 0.1f);

	ImGui::End();
}

Vector3 Hook::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f}; // プレイヤーの中心を考慮
	// フックの中心を計算
	Vector3 worldPosition = endPos_ + offset;
	// フックの中心を返す
	return worldPosition;
}
