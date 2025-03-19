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
	playerVelocity_ = {};
	playerAcceleration_ = {};
	// フックの位置を初期化
	endPos_ = playerPosition_;
}

void Hook::BehaviorNoneUpdate() {
	///===========================================
	/// 状態を変更
	///

	if (Input::GetInstance()->TriggerButton(9)) {
		requestBehavior_ = Behavior::Throw;
	}
}

void Hook::BehaviorThrowInitialize() {

	isActive_ = false;
	isThrowing_ = false;
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
	maxDistance_ = 50.0f;
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

	///========================================================================================
	/// プレイヤーのフック使用時の移動処理
	///

	if (isPulling_) {

		if (isActive_) {

			if (isDebug_) {

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
					// フックの方向に向かって移動
					direction = Vector3::Normalize(direction);
					playerVelocity_ = direction * speed_ * 0.016f; // 0.016fは1フレームの時間（約60FPS）
					playerAcceleration_ = playerVelocity_ * 0.1f;  // 加速度は速度の10%と仮定

					// EndPosに達したらフックを非アクティブにする
					if (isHitPlayerToEnemy_) {
						// playerPosition_ = endPos_;
						isActive_ = false;
						// フックの状態をなしに変更
						requestBehavior_ = Behavior::None;
					}
				}
			} else {

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
				}
			}
		}
	}

	///============================================================================================================
	/// フック使用時の弧の移動
	///
	///
	///
	/// 弧の動きは外積を使って計算を一度やってみて
	///
	///

	// 右スティックの入力を取得
	rightStick_ = Input::GetInstance()->GetRightStick();
	// フックの終点から中心へのベクトルを計算
	Vector3 toCenter = playerPosition_ - endPos_;
	// フックの終点から中心までの距離を計算
	float radius = toCenter.Length(toCenter);
	// フックの終点から中心までの角度を計算
	float angle = atan2(toCenter.z, toCenter.x);
	float angularSpeed = 3.0f; // 角速度（調整可能）

	if (isActive_) {

		if (!enemyHit_) {

			///===================================
			/// 壁
			///

			///
			/// 壁の上辺
			///

			if (endPos_.z >= maxMoveLimit_.z) {
				// フックの終点が上辺にある場合の弧の移動処理
				//
				// 右スティックの入力に応じて角度を変更
				if (rightStick_.x < -0.1f) {
					// 右に移動
					angle -= angularSpeed * 0.016f;

				} else if (rightStick_.x > 0.1f) {
					// 左に移動
					angle += angularSpeed * 0.016f;
				}
			}

			///
			/// 壁の下辺
			///
			if (endPos_.z <= minMoveLimit_.z) {
				// フックの終点が下辺にある場合の弧の移動処理
				//
				// 右スティックの入力に応じて角度を変更
				if (rightStick_.x < -0.1f) {
					// 右に移動
					angle += angularSpeed * 0.016f;

				} else if (rightStick_.x > 0.1f) {
					// 左に移動
					angle -= angularSpeed * 0.016f;
				}
			}

			///
			/// 壁の左辺
			///
			if (endPos_.x <= minMoveLimit_.x) {

				// 右スティックの入力に応じて角度を変更
				if (rightStick_.x < -0.1f) {
					// 右に移動
					angle += angularSpeed * 0.016f;
				} else if (rightStick_.x > 0.1f) {
					// 左に移動
					angle -= angularSpeed * 0.016f;
				}
			}

			///
			/// 壁の右辺
			///
			if (endPos_.x >= maxMoveLimit_.x) {
				// フックの終点が右辺にある場合の弧の移動処理
				//
				// 右スティックの入力に応じて角度を変更
				if (rightStick_.x < -0.1f) {
					// 右に移動
					angle -= angularSpeed * 0.016f;
				} else if (rightStick_.x > 0.1f) {
					// 左に移動
					angle += angularSpeed * 0.016f;
				}
			}

		} else {
			///===================================
			/// Enemy
			///
			// 右スティックの入力を取得
			rightStick_ = Input::GetInstance()->GetRightStick();

			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = playerPosition_ - endPos_;
			// フックの終点から中心までの距離を計算
			float radius = Vector3::Length(toCenter);
			// フックの終点から中心までの角度を計算
			float startAngle = atan2(toCenter.z, toCenter.x);
			float angularSpeed = 3.0f; // 角速度（調整可能）

			// 右スティックの入力に応じて角度を変更
			float endAngle = startAngle;
			if (rightStick_.x < -0.1f) {
				endAngle -= angularSpeed * 0.016f;
			} else if (rightStick_.x > 0.1f) {
				endAngle += angularSpeed * 0.016f;
			}

			// 角度を線形補間
			float angle = Vector3::AngleLerp(startAngle, endAngle, rightStick_.x);

			// 新しい位置を計算
			playerPosition_.x = endPos_.x + radius * cos(angle);
			playerPosition_.z = endPos_.z + radius * sin(angle);

			// 外積を使って速度を計算
			Vector3 tangent = Vector3::Cross(Vector3(0, 1, 0), toCenter);
			playerVelocity_ = Vector3::Normalize(tangent) * speed_ * 0.032f; // 速度を2倍に増加

			// 慣性を入れるために軽減処理を追加
			playerVelocity_ *= 0.95f; // 速度を95%に軽減

			// 位置を更新
			playerPosition_ += playerVelocity_;
			if (isHitPlayerToEnemy_) {
				// フックを非アクティブにする
				isActive_ = false;
				// フックの状態をなしに変更
				requestBehavior_ = Behavior::None;
			}
		}
		
	}
}

void Hook::BehaviorBackInitialize() {}

void Hook::BehaviorBackUpdate() {

	// フックの方向ベクトルを計算
	Vector3 moveDirection = startPos_ - endPos_;
	float distance = Vector3::Length(moveDirection);

	// 終了位置に到達したらフラグを更新
	if (distance < speed_ * 0.016f) {
		endPos_ = startPos_;
		isActive_ = true;
		requestBehavior_ = Behavior::None;
	} else {
		Vector3::Normalize(moveDirection);
		endPos_ += moveDirection * speed_ * 0.016f;
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
		if (enemyHit_) {
			// プレイヤーと敵の距離を計算
			distanceToEnemy = Vector3::Distance(playerPosition_, enemyHitPosition_);
			// プレイヤーと敵の距離が最小距離より小さければ最小距離を更新
			if (distanceToEnemy < maxDistance_) {
				// プレイヤーと敵の距離が最小距離より小さければ最小距離を更新
				maxDistance_ = distanceToEnemy;
			}
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
		// 敵以外に当経ってない場合はフラグを解除
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
	ImGui::Text("rightStick: %f, %f", rightStick_.x, rightStick_.y);
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

	ImGui::End();
}

Vector3 Hook::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f}; // プレイヤーの中心を考慮
	// フックの中心を計算
	Vector3 worldPosition = endPos_ + offset;
	// フックの中心を返す
	return worldPosition;
}
