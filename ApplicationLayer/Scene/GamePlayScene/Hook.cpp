#define NOMINMAX
#include "Hook.h"
#include "Enemy.h"
#include "Field.h"
#include "Input.h"
#include "Player.h"

#include "CollisionTypeIdDef.h"
#include "ImGuiManager.h"
#include "Wireframe.h"

/// -------------------------------------------------------------
///							初期化処理
/// -------------------------------------------------------------
void Hook::Initialize() {
	// フックのコライダーの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kHook));

	// フックの初期化
	isExtending_ = false;
	isThrowing_ = false;
	// フックの位置を初期化
	endPos_ = playerPosition_;
}

/// -------------------------------------------------------------
///							更新処理
/// -------------------------------------------------------------
void Hook::Update() {
	if (player_) {
		playerPosition_ = player_->GetPosition();
		playerRotation_ = player_->GetRotation();
		playerVelocity_ = player_->GetVelocity();
		playerAcceleration_ = player_->GetAcceleration();
		isHitPlayerToEnemy_ = player_->GetIsHitEnemy();
	}

	// フックの状態ごとの更新を行う
	if (field_) {
		minMoveLimit_ = field_->GetMinPosition();
		maxMoveLimit_ = field_->GetMaxPosition();
	}

	// 状態の変更がリクエストされている場合
	if (behaviorRequest_) {

		// 状態を変更する
		behavior_ = behaviorRequest_.value();

		// 状態ごとの初期化を行う
		switch (behavior_) {
		case Behavior::None:
			BehaviorNoneInitialize();
			break;
		case Behavior::Throw:
			BehaviorThrowInitialize();
			break;
		case Behavior::Extend:
			BehaviorExtendInitialize();
			break;
		case Behavior::Move:
			BehaviorMoveInitialize();
			break;
		case Behavior::Back:
			BehaviorBackInitialize();
			break;
		default:
			break;
		}

		// リクエストをクリア
		behaviorRequest_ = std::nullopt;
	}

	// 状態ごとの更新を行う
	switch (behavior_) {
	case Behavior::None:
		BehaviorNoneUpdate();
		break;
	case Behavior::Throw:
		BehaviorThrowUpdate();
		break;
	case Behavior::Extend:
		BehaviorExtendUpdate();
		break;
	case Behavior::Move:
		BehaviorMoveUpdate();
		break;
	case Behavior::Back:
		BehaviorBackUpdate();
		break;
	default:
		break;
	}
}

/// -------------------------------------------------------------
///							描画処理
/// -------------------------------------------------------------
void Hook::Draw() {
	// フックの線
	Wireframe::GetInstance()->DrawLine(startPos_, endPos_, {1.0f, 1.0f, 1.0f, 1.0f});
}

void Hook::ImGuiDraw() {

	ImGui::Begin("Hook Speed");
	ImGui::DragFloat("pullSpeed", &pullSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("extendSpeed", &extendSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("arcSpeed", &arcSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("backSpeed", &backSpeed, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("backDecelerationRate", &backDecelerationRate, 0.01f, 0.0f, 1.0f);
	ImGui::End();


	ImGui::Begin("Hook");
	ImGui::DragFloat3("playerVelocity", &playerVelocity_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("enemyPosition", &enemyPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::Text("endPosition: (%f, %f, %f)", endPos_.x, endPos_.y, endPos_.z);
	// Behaviorの状態を表示
    ImGui::Text("Behavior: %s",  
					behavior_ == Behavior::None ? "None" :  
					behavior_ == Behavior::Throw ? "Throw" :  
					behavior_ == Behavior::Extend ? "Extend" :  
					behavior_ == Behavior::Move ? "Move" :  
					behavior_ == Behavior::Back ? "Back" : "Unknown");

	ImGui::End();
}

/// -------------------------------------------------------------
///						フックの初期化処理
/// -------------------------------------------------------------
void Hook::BehaviorNoneInitialize() {
	// フックの位置を初期化
	startPos_ = playerPosition_;
	endPos_ = playerPosition_;
}

/// -------------------------------------------------------------
///						フックの更新処理
/// -------------------------------------------------------------
void Hook::BehaviorNoneUpdate() {
	startPos_ = playerPosition_;
	endPos_ = playerPosition_;
	if (Input::GetInstance()->TriggerButton(9)) {
		behaviorRequest_ = Behavior::Throw;
	}
}

/// -------------------------------------------------------------
///						フックを投げる初期化処理
/// -------------------------------------------------------------
void Hook::BehaviorThrowInitialize() {
	isActive_ = false;
	isThrowing_ = false;
	isExtending_ = false;
	angularSpeed = 0.0f;
	playerVelocity_ = {0.0f, 0.0f, 0.0f};
	playerAcceleration_ = {0.0f, 0.0f, 0.0f};
}

/// -------------------------------------------------------------
///						フックを投げる更新処理
/// -------------------------------------------------------------
void Hook::BehaviorThrowUpdate() {
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
	/// フックの終点を計算する処理
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
	behaviorRequest_ = Behavior::Extend;
}

/// -------------------------------------------------------------
///						フックを伸ばす初期化処理
/// -------------------------------------------------------------
void Hook::BehaviorExtendInitialize() {}

/// -------------------------------------------------------------
///						フックを伸ばす更新処理
/// -------------------------------------------------------------
void Hook::BehaviorExtendUpdate() {
	// 開始位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	// フックが投げられている場合、終了位置を徐々に更新
	if (isThrowing_) {
		Vector3 direction = potentialEndPos - endPos_;
		float distance = Vector3::Length(direction);

		// 終了位置に到達したらフラグを更新
		if (distance < extendSpeed_) {
			endPos_ = potentialEndPos;
			isThrowing_ = false;
			isActive_ = true;

			// フックの状態を移動に変更
			behaviorRequest_ = Behavior::Move;

		} else {
			direction = Vector3::Normalize(direction);
			endPos_ += direction * extendSpeed_;
		}
	}
}

/// -------------------------------------------------------------
///						フックを移動する初期化処理
/// -------------------------------------------------------------
void Hook::BehaviorMoveInitialize() {
	// フックの引っ張りフラグをリセット
	isPulling_ = false;

	// 弧の移動速度・引っ張り移動速度をリセット
	ArcVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
	MoveVelocity_ = Vector3(0.0f, 0.0f, 0.0f);

	// 角速度や弧の動きのフラグもリセット
	angularSpeed = 0.0f;
	isRightStickLeft = false;
	isRightStickRight = false;
}


void Hook::BehaviorMoveUpdate() {
	// フックの開始位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	///===========================================
	/// キー入力
	/// 
	
	// フックを引っ張るかどうかの入力をチェック
	if (isPulling_) {
		if (Input::GetInstance()->TriggerButton(15)) {
			isPulling_ = false;
		}
	} else {
		if (Input::GetInstance()->TriggerButton(15)) {
			isPulling_ = true;
		}
	}

	// フックを戻すかどうかの入力をチェック
	if (Input::GetInstance()->TriggerButton(9)) {
		behaviorRequest_ = Behavior::Back;
	}

	///===========================================
	/// フックの移動処理
	/// 

	// 弧の移動速度（ArcVelocity_）の計算
	ArcVelocity_ = Vector3(0.0f, 0.0f, 0.0f);

	// 右スティックの入力を取得
	rightStick_ = Input::GetInstance()->GetRightStick();
	Vector3 toCenter = playerPosition_ - endPos_;
	float radius = toCenter.Length(toCenter);

	if (isActive_) {
		if (!enemyHit_) {
			// 壁
			angle = atan2(toCenter.z, toCenter.x);

			rightStick_.x = round(rightStick_.x * 1000) / 1000;
			rightStick_.y = round(rightStick_.y * 1000) / 1000;

			// 壁の辺ごとに角度を変更
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

			if (!Input::GetInstance()->RStickInDeadZone()) {
				angularSpeed = std::min(maxAngularSpeed, angularSpeed + angularSpeedIncrement * 0.016f);

				Vector3 newPosition;
				newPosition.x = endPos_.x + radius * cos(angle);
				newPosition.z = endPos_.z + radius * sin(angle);

				ArcVelocity_.x = newPosition.x - playerPosition_.x;
				ArcVelocity_.z = newPosition.z - playerPosition_.z;
			} else {
				angularSpeed *= decelerationRate;
				if (isRightStickRight) {
					angle += angularSpeed * 0.016f;
				} else if (isRightStickLeft) {
					angle -= angularSpeed * 0.016f;
				}
				Vector3 newPosition;
				newPosition.x = endPos_.x + radius * cos(angle);
				newPosition.z = endPos_.z + radius * sin(angle);

				ArcVelocity_.x = newPosition.x - playerPosition_.x;
				ArcVelocity_.z = newPosition.z - playerPosition_.z;

				if (Vector3::Length(ArcVelocity_) < 0.01f) {
					ArcVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
				}
			}
		} else {
			// Enemy
			if (enemyHit_) {
				endPos_ = enemyPosition_;
				rightStick_ = Input::GetInstance()->GetRightStick();
				Vector3 toCenter = playerPosition_ - endPos_;
				float radius = Vector3::Length(toCenter);
				angle = atan2(toCenter.z, toCenter.x);

				rightStick_.x = round(rightStick_.x * 1000) / 1000;
				rightStick_.y = round(rightStick_.y * 1000) / 1000;

				if (rightStick_.x < -0.1f) {
					angle -= angularSpeed * 0.016f;
					isRightStickLeft = true;
					isRightStickRight = false;
				} else if (rightStick_.x > 0.1f) {
					angle += angularSpeed * 0.016f;
					isRightStickRight = true;
					isRightStickLeft = false;
				}
				if (!Input::GetInstance()->RStickInDeadZone()) {
					angularSpeed = std::min(maxAngularSpeed, angularSpeed + angularSpeedIncrement * 0.016f);

					Vector3 newPosition;
					newPosition.x = endPos_.x + radius * cos(angle);
					newPosition.z = endPos_.z + radius * sin(angle);

					ArcVelocity_.x = newPosition.x - playerPosition_.x;
					ArcVelocity_.z = newPosition.z - playerPosition_.z;
				} else {
					angularSpeed *= decelerationRate;
					if (isRightStickRight) {
						angle += angularSpeed * 0.016f;
					} else if (isRightStickLeft) {
						angle -= angularSpeed * 0.016f;
					}
					Vector3 newPosition;
					newPosition.x = endPos_.x + radius * cos(angle);
					newPosition.z = endPos_.z + radius * sin(angle);

					ArcVelocity_.x = newPosition.x - playerPosition_.x;
					ArcVelocity_.z = newPosition.z - playerPosition_.z;

					if (Vector3::Length(ArcVelocity_) < 0.01f) {
						ArcVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
	}

	// フックで引っ張る移動速度（MoveVelocity_）の計算
	MoveVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
	if (isPulling_ && isActive_) {
		Vector3 direction = endPos_ - playerPosition_;
		float distance = Vector3::Length(direction);
		direction = Vector3::Normalize(direction);
		MoveVelocity_ = direction * pullSpeed_ * 0.016f;

		if (distance < 1.0f) {
			isActive_ = false;
			behaviorRequest_ = Behavior::None;
		}
	}

	// 合成速度をplayerVelocity_に設定
	playerVelocity_ = ArcVelocity_ + MoveVelocity_;
}

/// -------------------------------------------------------------
///						フックを戻す初期化処理
/// -------------------------------------------------------------
void Hook::BehaviorBackInitialize() {}

/// -------------------------------------------------------------
///						フックを戻す更新処理
/// -------------------------------------------------------------
void Hook::BehaviorBackUpdate() {
	// フックの終了位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;
	playerVelocity_ *= backDecelerationRate;

	// フックの方向ベクトルを計算
	Vector3 moveDirection = startPos_ - endPos_;
	float distance = Vector3::Length(moveDirection);

	// 終了位置に到達したらフラグを更新
	if (distance < backSpeed * 0.016f) {
		endPos_ = startPos_;
		isActive_ = true;
		behaviorRequest_ = Behavior::None;
	} else {
		Vector3::Normalize(moveDirection);
		endPos_ += moveDirection * backSpeed * 0.016f;
	}
}

/// -------------------------------------------------------------
///				　		　衝突判定処理
/// -------------------------------------------------------------
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
		//if (!hookToEnemyHitBeforeThrow_) {

			// フックの終了位置を再計算
			potentialEndPos = playerPosition_ - direction_ * maxDistance_;

			// フックの現在位置を開始位置に設定
			currentPos_ = startPos_;
			// フックの開始時間を記録
			startTime_ = std::chrono::steady_clock::now();
	//	}

	} else {
		// 敵以外に当たってない場合はフラグを解除
		enemyHit_ = false;
	}
}

/// -------------------------------------------------------------
///				　中心座標を取得する純粋仮想関数
/// -------------------------------------------------------------
Vector3 Hook::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f}; // プレイヤーの中心を考慮
	// フックの中心を計算
	Vector3 worldPosition = endPos_ + offset;
	// フックの中心を返す
	return worldPosition;
}
