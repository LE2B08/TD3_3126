#include "Player.h"
#include "Input.h"
#include "Wireframe.h"
#include "imgui.h"
#include "ParticleManager.h"

#undef max
#undef min

void Player::Initialize() {
	// オブジェクト3D
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("sphere.gltf");
	// 位置
	position_ = {8.0f, 0.0f, 8.0f};
	// 回転
	rotation_ = {0.0f, 0.0f, 0.0f};
	// スケール
	scale_ = {1.0f, 1.0f, 1.0f};
	// 速度
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 加速度
	acceleration_ = {0.0f, 0.0f, 0.0f};
	// 角速度
	angularVelocity_ = {0.0f, 0.0f, 0.0f};
	
	// フックの生成 初期化
	hook_ = std::make_unique<Hook>();
	hook_->Initialize();

	// 武器の初期化
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(ParticleManager::GetInstance(), "EnemyHitParticles");
}

void Player::Update() {

#ifdef _DEBUG

	// コントローラーのStartボタンとBackボタンを同時に押すとデバックモードになる
	if (!isDebug_) {
		if (Input::GetInstance()->TriggerButton(12) && Input::GetInstance()->TriggerButton(13)) {
			isDebug_ = true;
		}
	} else {
		if (Input::GetInstance()->TriggerButton(12) && Input::GetInstance()->TriggerButton(13)) {
			isDebug_ = false;
		}
	}
	// デバックモード
	// プレイヤーをの移動が
	// 左スティックとキーボードで入力できる。

#endif // DEBUG

	// フックを投げるボタンを押した瞬間
	if (Input::GetInstance()->TriggerButton(9)) {
		hook_->Throw();
	}
	// フックを投げるボタンを押している間
	if (hook_->GetIsActive()) {
		hook_->Extend();
	}

	// フックがアクティブで、フックの伸びが止まっている場合、プレイヤーを移動させる
	if (hook_->GetIsActive()) {
		hook_->Move();
	}

	// 移動処理
	Move();

	// 攻撃処理
	Attack();

	// 移動制限
	position_.x = std::clamp(position_.x, minMoveLimit_.x, maxMoveLimit_.x);
	position_.z = std::clamp(position_.z, minMoveLimit_.z, maxMoveLimit_.z);;

	// フックの更新処理
	hook_->SetPlayerRotation(rotation_);
	hook_->SetPlayerPosition(position_);
	hook_->SetMinMoveLimit(minMoveLimit_);
	hook_->SetMaxMoveLimit(maxMoveLimit_);

	// フックの更新処理
	hook_->Update();


	// 武器の更新処理
	weapon_->SetPlayerPosition(position_);
	weapon_->SetPlayerRotation(rotation_);
	weapon_->SetPlayerScale(scale_);
	weapon_->Update();

	// Transform更新処理
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();

	collisionManager_->Update();
}

void Player::Draw() {

	// 描画処理
	object3D_->Draw();

	// フックの描画
	hook_->Draw();

	// 武器の描画
	weapon_->Draw();

	collisionManager_->Draw();

	// プレイヤーの向きを示す線を描画
	Vector3 direction = {cos(rotation_.y), 0.0f, sin(rotation_.y)};
	Vector3 endPos = position_ + -direction * 5.0f;                                  // 5.0fは線の長さ
	Wireframe::GetInstance()->DrawLine(position_, endPos, {0.0f, 1.0f, 0.0f, 1.0f}); // 緑色の線
}

void Player::Finalize() {}

void Player::DrawImGui() {

	ImGui::Begin("Player");
	ImGui::Text("Position");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::Text("Rotation");
	ImGui::SliderFloat3("Rotate", &rotation_.x, -10.0f, 10.0f);
	ImGui::Text("Scale");
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::Text("Velocity");
	ImGui::SliderFloat3("Velocity", &velocity_.x, -10.0f, 10.0f);
	ImGui::Text("Acceleration");
	ImGui::SliderFloat3("Accel", &acceleration_.x, -10.0f, 10.0f);
	ImGui::Text("AngularVelocity");
	ImGui::SliderFloat3("AngleVelo", &angularVelocity_.x, -10.0f, 10.0f);
	ImGui::End();

	hook_->ShowImGui();
}
void Player::Move() {

	///============================
	/// 移動処理
	/// はデバック用に残しておく
	/// 最終的には消す
	///
#ifdef _DEBUG
	if (isDebug_) {
		// コントローラーが接続されてる時
		if (Input::GetInstance()->GetGamePadState().Gamepad.sThumbLX || Input::GetInstance()->GetGamePadState().Gamepad.sThumbLY) {
			// 左スティックの入力があるとき
			// プレイヤーを移動させる
			acceleration_.x = Input::GetInstance()->GetLeftStick().x * 0.01f;
			acceleration_.z = Input::GetInstance()->GetLeftStick().y * 0.01f;

		} else {
			// コントローラーが接続されてないとき

			// WとSキーでプレイヤーを上下に移動
			if (Input::GetInstance()->PushKey(DIK_W)) {
				acceleration_.z = 0.005f;
			} else if (Input::GetInstance()->PushKey(DIK_S)) {
				acceleration_.z = -0.005f;
			} else {

				acceleration_.z = 0.0f;
			}
			// AとDキーでプレイヤーを左右に移動
			if (Input::GetInstance()->PushKey(DIK_A)) {
				acceleration_.x = -0.005f;
			} else if (Input::GetInstance()->PushKey(DIK_D)) {
				acceleration_.x = 0.005f;
			} else {
				acceleration_.x = 0.0f;
			}
		}
		// Y軸は固定
		acceleration_.y = -0.0f;
	}
#endif // _DEBUG

	///================
	/// プレイヤーのフック使用時の移動処理
	///

	// フックがアクティブで、フックの伸びが止まっている場合、プレイヤーを移動させる
	if (hook_->GetIsActive()) {
		// フックの方向ベクトルを計算
		Vector3 direction = hook_->GetEndPos() - position_;
		float distance = Vector3::Length(direction);

		// フックの位置に到達したらフックを非アクティブにする
		if (distance < hook_->GetSpeed() * 0.016f) { // 0.016fは1フレームの時間（約60FPS）
			position_ = hook_->GetEndPos();
			hook_->SetIsActive(false);
		}
		else {
			// フックの方向に向かって移動
			direction.Normalize(direction);
			Vector3 newPosition = position_ + direction * hook_->GetSpeed() * 0.016f; // 0.016fは1フレームの時間（約60FPS）

			// 壁に触れたらそれ以上ポジションを追加しない
			if (newPosition.x < minMoveLimit_.x || newPosition.x > maxMoveLimit_.x || newPosition.z < minMoveLimit_.z || newPosition.z > maxMoveLimit_.z) {
				hook_->SetIsActive(false);
			}
			else {
				position_ = newPosition;
			}
		}

		///===================================
		/// フック使用時の弧の移動
		///

		///
		/// 壁の上辺
		///
		if (hook_->GetEndPos().z >= maxMoveLimit_.z) {
			// フックの終点が上辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = position_ - hook_->GetEndPos();
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
			position_.x = hook_->GetEndPos().x + radius * cos(angle);
			position_.z = hook_->GetEndPos().z + radius * sin(angle);
		}

		///
		/// 壁の下辺
		///
		if (hook_->GetEndPos().z <= minMoveLimit_.z) {
			// フックの終点が下辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = position_ - hook_->GetEndPos();
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
			position_.x = hook_->GetEndPos().x + radius * cos(angle);
			position_.z = hook_->GetEndPos().z + radius * sin(angle);
		}

		///
		/// 壁の左辺
		///
		if (hook_->GetEndPos().x <= minMoveLimit_.x) {
			// フックの終点が左辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = position_ - hook_->GetEndPos();
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
			position_.x = hook_->GetEndPos().x + radius * cos(angle);
			position_.z = hook_->GetEndPos().z + radius * sin(angle);
		}

		///
		/// 壁の右辺
		///
		if (hook_->GetEndPos().x >= maxMoveLimit_.x) {
			// フックの終点が右辺にある場合の弧の移動処理
			//
			// フックの終点から中心へのベクトルを計算
			Vector3 toCenter = position_ - hook_->GetEndPos();
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
			position_.x = hook_->GetEndPos().x + radius * cos(angle);
			position_.z = hook_->GetEndPos().z + radius * sin(angle);
		}
	}

	///================
	/// プレイヤーの回転処理
	///

	// プレイヤーの向きを左スティックの向きにする

	// 右スティックの入力があるとき
	if (Input::GetInstance()->GetRightStick().x != 0.0f || Input::GetInstance()->GetRightStick().y != 0.0f) {
		// プレイヤーの向きを変える
		rotation_.y = -atan2(Input::GetInstance()->GetRightStick().x, Input::GetInstance()->GetRightStick().y) - std::numbers::pi_v<float> / 2.0f;
	} else {
	}
	// 右スティックの向きを優先

	// プレイヤーの移動処理
	velocity_ += acceleration_;
	position_ += velocity_;

	// 減速処理
	const float friction = 0.98f; // 摩擦係数
	velocity_ *= friction;
}

void Player::Attack() {
	// 攻撃処理
	// 攻撃ボタンを押した瞬間
	if (Input::GetInstance()->TriggerButton(8)) {
		// 武器の攻撃フラグを立てる
		weapon_->SetIsAttack(true);
	}
}

void Player::OnCollision(Collider* other) {
}

Vector3 Player::GetCenterPosition() const
{
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // プレイヤーの中心を考慮
	Vector3 worldPosition = position_ + offset;
	return worldPosition;
}

void Player::CheckAllCollisions(Enemy* enemy) {
}
