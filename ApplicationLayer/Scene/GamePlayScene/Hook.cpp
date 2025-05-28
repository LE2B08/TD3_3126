#define NOMINMAX
#include "Hook.h"
#include "Enemy.h"
#include "Field.h"
#include "Input.h"
#include "Player.h"
#include "TutorialPlayer.h"

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
	else if (tutorialPlayer_) {
		playerPosition_ = tutorialPlayer_->GetPosition();
		playerRotation_ = tutorialPlayer_->GetRotation();
		playerVelocity_ = tutorialPlayer_->GetVelocity();
		playerAcceleration_ = tutorialPlayer_->GetAcceleration();
		isHitPlayerToEnemy_ = tutorialPlayer_->GetIsHitEnemy();
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
	ImGui::DragFloat("backSpeed", &backSpeed_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("backDecelerationRate", &backDecelerationRate, 0.01f, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("Hook Variables");

	ImGui::Text("=== Player ===");
	ImGui::DragFloat3("playerPosition", &playerPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("playerRotation", &playerRotation_.x, 0.1f, -3.15f, 3.15f);
	ImGui::DragFloat3("playerVelocity", &playerVelocity_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("playerAcceleration", &playerAcceleration_.x, 0.1f, -100.0f, 100.0f);

	ImGui::Text("=== Hook ===");
	ImGui::DragFloat3("startPos", &startPos_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("endPos", &endPos_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("currentPos", &currentPos_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("direction", &direction_.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("ArcVelocity", &arcMoveVelocity_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("MoveVelocity", &pullVelocity_.x, 0.1f, -100.0f, 100.0f);

	ImGui::Text("=== Enemy ===");
	ImGui::DragFloat3("enemyPosition", &enemyPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("enemyHitPosition", &enemyHitPosition_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("distanceToEnemy", &distanceToEnemy, 0.1f, 0.0f, 100.0f);

	ImGui::Text("=== Field Limit ===");
	ImGui::DragFloat3("minMoveLimit", &minMoveLimit_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("maxMoveLimit", &maxMoveLimit_.x, 0.1f, -100.0f, 100.0f);

	ImGui::Text("=== State/Flags ===");
	ImGui::Checkbox("isActive", &isActive_);
	ImGui::Checkbox("isExtending", &isExtending_);
	ImGui::Checkbox("isThrowing", &isThrowing_);
	ImGui::Checkbox("isPulling", &isPulling_);
	ImGui::Checkbox("enemyHit", &enemyHit_);
	ImGui::Checkbox("isHitPlayerToEnemy", &isHitPlayerToEnemy_);
	ImGui::Checkbox("isLeftStickLeft", &isLeftStickLeft);
	ImGui::Checkbox("isLeftStickRight", &isLeftStickRight);

	ImGui::Text("=== Behavior ===");
	ImGui::Text(
	    "Behavior: %s", behavior_ == Behavior::None     ? "None"
	                    : behavior_ == Behavior::Throw  ? "Throw"
	                    : behavior_ == Behavior::Extend ? "Extend"
	                    : behavior_ == Behavior::Move   ? "Move"
	                    : behavior_ == Behavior::Back   ? "Back"
	                                                    : "Unknown");

	ImGui::Text("=== Other ===");
	ImGui::DragFloat("maxDistance", &maxDistance_, 0.1f, 0.0f, 200.0f);
	ImGui::DragFloat("angle", &angle, 0.01f, -3.15f, 3.15f);
	ImGui::DragFloat("angularSpeed", &angularSpeed, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("maxAngularSpeed", &maxAngularSpeed, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("angularSpeedIncrement", &angularSpeedIncrement, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("decelerationRate", &decelerationRate, 0.01f, 0.0f, 1.0f);

	ImGui::Text("=== Wall Hit ===");
	ImGui::Checkbox("isHitWallTop", &isHitWallTop);
	ImGui::Checkbox("isHitWallBottom", &isHitWallBottom);
	ImGui::Checkbox("isHitWallLeft", &isHitWallLeft);
	ImGui::Checkbox("isHitWallRight", &isHitWallRight);


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
	isBack_ = false;
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
		isHitEnemyBeforeThrow_ = true;
	} else {
		// あたってなければフラグを解除
		isHitEnemyBeforeThrow_ = false;
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
void Hook::BehaviorExtendInitialize() { isBack_ = false; }

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
	isBack_ = false;

	// 弧の移動速度・引っ張り移動速度をリセット
	arcMoveVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
	pullVelocity_ = Vector3(0.0f, 0.0f, 0.0f);

	// 角速度や弧の動きのフラグもリセット
	angularSpeed = 0.0f;
	isLeftStickLeft = false;
	isLeftStickRight = false;
	angle = 0.0f;
}



/// -------------------------------------------------------------
///		フックの移動状態の更新処理
/// -------------------------------------------------------------
void Hook::BehaviorMoveUpdate() {
	///----------------------------
	/// プレイヤー位置の更新
	///----------------------------
	// フックの開始位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;

	///----------------------------
	/// キー入力処理
	///----------------------------
	// フックを引っ張るかどうかの入力をチェック
	if (!isPulling_) {
		if (Input::GetInstance()->TriggerButton(15)) {
			isPulling_ = true;
		}
	}

	// フックを戻すかどうかの入力をチェック
	if (Input::GetInstance()->TriggerButton(9)) {
		behaviorRequest_ = Behavior::Back;
	}

	///----------------------------
	/// フックの移動処理
	///----------------------------

	// 弧の移動速度（ArcVelocity_）の計算
	// ※ArcVelocity_は後で計算するのでここでは初期化しない

	// 右スティックの入力を取得
	leftStick_ = Input::GetInstance()->GetLeftStick();

	// isArcMove_の判定
	if (!isPulling_) {
		if (playerVelocity_.x != 0.0f || playerVelocity_.y != 0.0f || playerVelocity_.z != 0.0f) {
			isArcMove_ = true;
		}
		else {
			isArcMove_ = false;
		}
	}
	else {
		isArcMove_ = false;
	}

	// プレイヤーからフック終点へのベクトル
	Vector3 toCenter = playerPosition_ - endPos_;
	// プレイヤーとフック終点の距離（弧の半径）
	float radius = toCenter.Length(toCenter);

	///----------------------------
	/// 弧の移動
	///----------------------------
	if (isActive_) {
		// 敵に当たっていない場合
		if (!enemyHit_) {
			// 現在の弧の角度を計算
			angle = atan2(toCenter.z, toCenter.x);

			// 右スティック入力を小数第3位で丸める
			leftStick_.x = round(leftStick_.x * 1000) / 1000;
			leftStick_.y = round(leftStick_.y * 1000) / 1000;

			// 角度変化量
			float angleDelta = 0.0f;
			// 右スティック入力があったか
			bool stickInput = false;

			///----------------------------
			/// 壁ヒット判定（イプシロン付き）
			///----------------------------
			constexpr float wallEpsilon = 0.05f; // 壁判定の許容誤差
			isHitWallTop = (endPos_.z >= maxMoveLimit_.z - wallEpsilon);
			isHitWallBottom = (endPos_.z <= minMoveLimit_.z + wallEpsilon);
			isHitWallLeft = (endPos_.x <= minMoveLimit_.x + wallEpsilon);
			isHitWallRight = (endPos_.x >= maxMoveLimit_.x - wallEpsilon);

			// 右スティック入力がある場合、回転方向フラグをリセット
			if (!Input::GetInstance()->LStickInDeadZone()) {
				isLeftStickLeft = false;
				isLeftStickRight = false;
			}

			///----------------------------
			/// 各壁ごとの角度変化・フラグ処理
			///----------------------------

			// 上壁
			if (isHitWallTop) {
				// 右スティック左入力
				if (leftStick_.x < -0.1f) {
					angleDelta -= angularSpeed * 0.016f;
					isLeftStickLeft = true;
					stickInput = true;
				}
				// 右スティック右入力
				else if (leftStick_.x > 0.1f) {
					angleDelta += angularSpeed * 0.016f;
					isLeftStickRight = true;
					stickInput = true;
				}
			}
			// 下壁
			if (isHitWallBottom) {
				if (leftStick_.x < -0.1f) {
					angleDelta += angularSpeed * 0.016f;
					isLeftStickRight = true;
					stickInput = true;
				} else if (leftStick_.x > 0.1f) {
					angleDelta -= angularSpeed * 0.016f;
					isLeftStickLeft = true;
					stickInput = true;
				}
			}
			// 左壁
			if (isHitWallLeft) {
				if (leftStick_.x < -0.1f) {
					angleDelta += angularSpeed * 0.016f;
					isLeftStickRight = true;
					stickInput = true;
				} else if (leftStick_.x > 0.1f) {
					angleDelta -= angularSpeed * 0.016f;
					isLeftStickLeft = true;
					stickInput = true;
				}
			}
			// 右壁
			if (isHitWallRight) {
				if (leftStick_.x < -0.1f) {
					angleDelta -= angularSpeed * 0.016f;
					isLeftStickLeft = true;
					stickInput = true;
				} else if (leftStick_.x > 0.1f) {
					angleDelta += angularSpeed * 0.016f;
					isLeftStickRight = true;
					stickInput = true;
				}
			}

			///----------------------------
			/// 角速度・角度の更新
			///----------------------------
			if (stickInput) {
				// 入力があった場合：角速度を加速し、角度を更新
				angularSpeed = std::min(maxAngularSpeed, angularSpeed + angularSpeedIncrement * 0.016f);
				angle += angleDelta;
			} else {
				// 入力がない場合：角速度を減衰し、直前の入力方向で慣性回転
				angularSpeed *= decelerationRate;
				if (isLeftStickRight) {
					angle += angularSpeed * 0.016f;
				}
				if (isLeftStickLeft) {
					angle -= angularSpeed * 0.016f;
				}
			}

			///----------------------------
			/// 新しい弧の終点座標の計算
			///----------------------------
			Vector3 newPosition;
			newPosition.x = endPos_.x + radius * cos(angle);
			newPosition.z = endPos_.z + radius * sin(angle);

			// 弧の速度ベクトルを計算
			arcMoveVelocity_.x = newPosition.x - playerPosition_.x;
			arcMoveVelocity_.z = newPosition.z - playerPosition_.z;

			// 入力がなく、弧の速度がほぼゼロなら停止
			if (!stickInput && Vector3::Length(arcMoveVelocity_) < 0.01f) {
				arcMoveVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
				isLeftStickLeft = false;
				isLeftStickRight = false;
			}
		}
		//----------------------------
		// 敵に当たっている場合の弧の移動
		//----------------------------
		else {
			if (enemyHit_) {
				// 終点を敵の位置に設定（半径分手前で止める）
				Vector3 toEnemy = enemyPosition_ - playerPosition_;
				Vector3 direction = Vector3::Normalize(toEnemy);
				float enemyRadius = enemy_ ? enemy_->GetRadius() : 0.0f;
				endPos_ = enemyPosition_ - direction * enemyRadius;

				// 右スティックの入力を取得
				leftStick_ = Input::GetInstance()->GetLeftStick();
				Vector3 toCenter = playerPosition_ - endPos_;
				float radius = Vector3::Length(toCenter);
				angle = atan2(toCenter.z, toCenter.x);

				// 右スティック入力を小数第3位で丸める
				leftStick_.x = round(leftStick_.x * 1000) / 1000;
				leftStick_.y = round(leftStick_.y * 1000) / 1000;

				// 右スティック入力による角度更新・フラグ
				if (leftStick_.x < -0.1f) {
					angle -= angularSpeed * 0.016f;
					isLeftStickLeft = true;
					isLeftStickRight = false;
				} else if (leftStick_.x > 0.1f) {
					angle += angularSpeed * 0.016f;
					isLeftStickRight = true;
					isLeftStickLeft = false;
				}

				// 角速度・弧の速度更新
				if (!Input::GetInstance()->LStickInDeadZone()) {
					angularSpeed = std::min(maxAngularSpeed, angularSpeed + angularSpeedIncrement * 0.016f);

					Vector3 newPosition;
					newPosition.x = endPos_.x + radius * cos(angle);
					newPosition.z = endPos_.z + radius * sin(angle);

					arcMoveVelocity_.x = newPosition.x - playerPosition_.x;
					arcMoveVelocity_.z = newPosition.z - playerPosition_.z;
				} else {
					angularSpeed *= decelerationRate;
					if (isLeftStickRight) {
						angle += angularSpeed * 0.016f;
					} else if (isLeftStickLeft) {
						angle -= angularSpeed * 0.016f;
					}
					Vector3 newPosition;
					newPosition.x = endPos_.x + radius * cos(angle);
					newPosition.z = endPos_.z + radius * sin(angle);

					arcMoveVelocity_.x = newPosition.x - playerPosition_.x;
					arcMoveVelocity_.z = newPosition.z - playerPosition_.z;

					if (Vector3::Length(arcMoveVelocity_) < 0.01f) {
						arcMoveVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
	}

	///----------------------------
	/// フックで引っ張る移動速度（MoveVelocity_）の計算
	///----------------------------
	pullVelocity_ = Vector3(0.0f, 0.0f, 0.0f);
	if (isPulling_ && isActive_) {
		// プレイヤーからフック終点への方向ベクトル
		Vector3 direction = endPos_ - playerPosition_;
		float distance = Vector3::Length(direction);
		direction = Vector3::Normalize(direction);

		// 引っ張り速度を計算
		pullVelocity_ = direction * pullSpeed_ * 0.016f;

		// 終点に近づいたらフックを非アクティブ化
		if (distance < 1.0f) {
			isActive_ = false;
			behaviorRequest_ = Behavior::None;
		}
	}

	///----------------------------
	/// 合成速度をplayerVelocity_に設定
	///----------------------------
	playerVelocity_ = arcMoveVelocity_ + pullVelocity_;
}

/// -------------------------------------------------------------
///						フックを戻す初期化処理
/// -------------------------------------------------------------
void Hook::BehaviorBackInitialize() { isBack_ = true; }

/// -------------------------------------------------------------
///						フックを戻す更新処理
/// -------------------------------------------------------------
void Hook::BehaviorBackUpdate() {

	isBack_ = true;

	// フックの終了位置をプレイヤーの位置に設定
	startPos_ = playerPosition_;
	playerVelocity_ *= backDecelerationRate;

	// フックの方向ベクトルを計算
	Vector3 moveDirection = startPos_ - endPos_;
	float distance = Vector3::Length(moveDirection);

	// 終了位置に到達したらフラグを更新
	if (distance < backSpeed_ * 0.016f) {
		endPos_ = startPos_;
		isActive_ = true;
		behaviorRequest_ = Behavior::None;
	} else {
		Vector3::Normalize(moveDirection);
		endPos_ += moveDirection * backSpeed_ * 0.016f;
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
		// if (!hookToEnemyHitBeforeThrow_) {

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
