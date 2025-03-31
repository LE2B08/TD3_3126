#include "GamePlayScene.h"
#include "Camera.h"
#include "Easing.h"
#include "Object3DCommon.h"
#include "SceneManager.h"
#include "Wireframe.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>

#include "FadeManager.h"

#ifdef _DEBUG
#include <DebugCamera.h>
#endif // _DEBUG
#include <SkyBoxManager.h>
#include <SpriteManager.h>

using namespace Easing;

/// -------------------------------------------------------------
///				　			　初期化処理
/// -------------------------------------------------------------
void GamePlayScene::Initialize() {
	// フェードアウト開始
	FadeManager::GetInstance()->StartFadeOut();

#ifdef _DEBUG
	// デバッグカメラの初期化
	DebugCamera::GetInstance()->Initialize();
#endif // _DEBUG

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	textureManager = TextureManager::GetInstance();
	particleManager = ParticleManager::GetInstance();

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	field_ = std::make_unique<Field>();
	field_->Initialize();
	field_->SetScale(fieldScale_);

	// Playerクラスの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetPosition({ 1000.0f, 1000.0f, 1000.0f });

	// PlayerUIの初期化
	playerUI_ = std::make_unique<PlayerUI>();
	playerUI_->Initialize();

	// 武器の初期化
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	// フックの生成 初期化
	hook_ = std::make_unique<Hook>();
	hook_->SetPlayerPosition(player_->GetPosition());
	hook_->SetPlayerRotation(player_->GetRotation());
	hook_->SetPlayerVelocity(player_->GetVelocity());
	hook_->SetPlayerAcceleration(player_->GetAcceleration());

	hook_->SetMinMoveLimit(field_->GetMinPosition());
	hook_->SetMaxMoveLimit(field_->GetMaxPosition());
	hook_->Initialize();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	enemyBullets_ = &enemy_->GetBullets();

	// fieldScale_ = { 0.0f,0.0f,0.0f };

	// コントローラー用UIの生成&初期化
	controllerUI_ = std::make_unique<ControllerUI>();
	controllerUI_->Initialize();

	// スカイボックス
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize("rostock_laage_airport_4k.dds");

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
}

/// -------------------------------------------------------------
///				　			　 更新処理
/// -------------------------------------------------------------
void GamePlayScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F12)) {
		Object3DCommon::GetInstance()->SetDebugCamera(!Object3DCommon::GetInstance()->GetDebugCamera());
		Wireframe::GetInstance()->SetDebugCamera(!Wireframe::GetInstance()->GetDebugCamera());
		skyBox_->SetDebugCamera(!skyBox_->GetDebugCamera());
		isDebugCamera_ = !isDebugCamera_;
	}
#endif // _DEBUG

	camera_->SetTranslate(cameraPosition_);

	// シーン切り替え
	if (input_->TriggerKey(DIK_F1)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_F2)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_F3)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("SatouScene");
		}
	}

	if (player_->GetIsHitEnemy()) {
		// カメラの揺れを開始
		isCameraShaking_ = true;
		shakeElapsedTime_ = 0.0f;
	}

	// enemyにプレイヤーがあたったときにカメラをシェイクする
	CameraShake();

	camera_->Update();

	// ゲーム開始演出
	if (!isGameStart_) {
		GameStart();
	}

	field_->SetScale(fieldScale_);
	field_->Update();

	// フックから移動情報を取得
	if (isGameStart_) {

		// プレイヤーの位置をフックにセット
		player_->SetPosition(hook_->GetPlayerPosition());
		player_->SetVelocity(hook_->GetPlayerVelocity());
		player_->SetAcceleration(hook_->GetPlayerAcceleration());

		player_->SetMinMoveLimit(field_->GetMinPosition());
		player_->SetMaxMoveLimit(field_->GetMaxPosition());
		player_->SetEnemy(enemy_.get());
		player_->SetIsGameStart(isGameStart_);
		player_->SetIsAttack(weapon_->GetIsAttack());

	}
	player_->Update();


	playerUI_->SetHp(player_->GetHp());
	playerUI_->Update();

	// 武器の更新処理
	weapon_->SetPlayerPosition(player_->GetPosition());
	weapon_->SetPlayerRotation(player_->GetRotation());
	weapon_->SetPlayerScale(player_->GetScale());
	weapon_->SetIsAttack(player_->GetIsAttack());
	weapon_->Update();

	// フックの更新処理
	hook_->SetPlayerPosition(player_->GetPosition());
	hook_->SetPlayerRotation(player_->GetRotation());
	hook_->SetPlayerVelocity(player_->GetVelocity());
	hook_->SetPlayerAcceleration(player_->GetAcceleration());
	hook_->SetMinMoveLimit(field_->GetMinPosition());
	hook_->SetMaxMoveLimit(field_->GetMaxPosition());
	hook_->SetIsHitPlayerToEnemy(player_->GetIsHitEnemy());

	// フックの更新処理
	hook_->Update();

	enemy_->SetMinMoveLimit(field_->GetMinPosition());
	enemy_->SetMaxMoveLimit(field_->GetMaxPosition());
	enemy_->SetPlayer(player_.get());
	enemy_->Update();

	collisionManager_->Update();
	if (isGameStart_) {
		// 衝突判定と応答
		CheckAllCollisions();
		// player_->CheckAllCollisions();
	}

	// 攻撃中の場合
	if (weapon_->GetIsAttack()) {
		// 衝突判定と応答
		collisionManager_->CheckAllCollisions();
		if (enemy_->GetIsHit()) {
			enemy_->SetIsHitFromAttack(true);
		}
	}

	// コントローラー用UIの更新
	controllerUI_->Update();

	// スカイボックスの更新処理
	skyBox_->Update();
}

/// -------------------------------------------------------------
///				　			　 描画処理
/// -------------------------------------------------------------
void GamePlayScene::Draw() {
	/// ------------------------------------------ ///
	/// ---------- スカイボックスの描画 ---------- ///
	/// ------------------------------------------ ///
	SkyBoxManager::GetInstance()->SetRenderSetting();
	skyBox_->Draw();

	/// ---------------------------------------- ///
	/// ----------  スプライトの描画  ---------- ///
	/// ---------------------------------------- ///
	// スプライトの共通描画設定
	SpriteManager::GetInstance()->SetRenderSetting();
	// プレイヤーUI
	playerUI_->Draw();

	// コントローラー用UIの描画
	controllerUI_->Draw();

	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// プレイヤー
	player_->Draw();

	// 武器の描画
	weapon_->Draw();

	// フックの描画
	hook_->Draw();

	enemy_->Draw();

	field_->Draw();

	collisionManager_->Draw();

	// ワイヤーフレームの描画
	// Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, { 0.25f, 0.25f, 0.25f,1.0f });
}

/// -------------------------------------------------------------
///				　			　 終了処理
/// -------------------------------------------------------------
void GamePlayScene::Finalize() {}

/// -------------------------------------------------------------
///				　			ImGui描画処理
/// -------------------------------------------------------------
void GamePlayScene::DrawImGui() {
	ImGui::Begin("Test Window");

	ImGui::SliderFloat("CameraShakeDuration", &shakeDuration_, 0.0f, 10.0f);
	ImGui::SliderFloat("CameraShakeMagnitude", &shakeMagnitude_, 0.0f, 10.0f);
	ImGui::Text("isGameStart : %s", isGameStart_ ? "true" : "false");
	ImGui::Text("startTimer : %f", startTimer_);
	ImGui::Text("maxStartT : %f", maxStartT_);
	ImGui::Text("isStartEasing : %s", isStartEasing_ ? "true" : "false");
	ImGui::Text("isPlayerPositionSet : %s", isPlayerPositionSet_ ? "true" : "false");
	ImGui::End();

	player_->DrawImGui();
	playerUI_->DrawImGui();
	weapon_->DrawImGui();
	hook_->ShowImGui();

	enemy_->ShowImGui("Enemy");

	field_->ShowImGui("Field");

	controllerUI_->DrawImGui();
}

/// -------------------------------------------------------------
///				　			衝突判定と応答
/// -------------------------------------------------------------
void GamePlayScene::CheckAllCollisions() {
	/*------プレイヤーと敵------*/
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(weapon_.get());
	collisionManager_->AddCollider(hook_.get());
	collisionManager_->AddCollider(enemy_.get());

	// 複数について

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();

	/*------プレイヤーと敵の弾------*/

	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	for (const auto& bullet : *enemyBullets_) {
		collisionManager_->AddCollider(bullet.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

void GamePlayScene::CameraShake() {
	// カメラの揺れを更新
	if (isCameraShaking_) {
		shakeElapsedTime_ += 1.0f / 60.0f;
		if (shakeElapsedTime_ >= shakeDuration_) {
			// 振動を止める
			input_->StopVibration();
			isCameraShaking_ = false;
			camera_->SetTranslate(cameraPosition_); // 元の位置に戻す
		}
		else {
			// ランダムな揺れを生成
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(-shakeMagnitude_, shakeMagnitude_);
			Vector3 shakeOffset = { dis(gen), dis(gen), dis(gen) };
			camera_->SetTranslate(cameraPosition_ + shakeOffset);
			// コントローラーを振動させる
			input_->SetVibration(1, 1);
		}
	}
}

void GamePlayScene::GameStart() {
	// エンターキーかAボタンが押されるたびに演出を開始
	// デバッグ用なのであとで消すこと
	if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {
		if (!isStartEasing_) {
			startTimer_ = 0.0f;
			playerStartTimer_ = 0.0f;
			isStartEasing_ = true;
		}
	}
	// イージングがスタートしているか
	if (isStartEasing_) {
		// easeTを40から0まで変動させる
		if (startTimer_ >= maxStartT_) {
			startTimer_ = maxStartT_;
			isStartEasing_ = false;
			isPlayerPositionSet_ = true;

		}
		else {
			startTimer_ += 0.5f;
		}
		// ポヨンポヨンしてフィールドが広がる演出
		fieldScale_ = Vector3::Lerp(startFieldScale_, defaultFieldScale_, easeOutBounce(startTimer_ / maxStartT_));
	}

	// プレイヤーの位置がセットされたか
	if (isPlayerPositionSet_) {
		// プレイヤーの位置をセット
		player_->SetPosition({ 8.0f, 20.0f, 8.0f });
		if (playerStartTimer_ >= maxPlayerStartT_) {
			playerStartTimer_ = maxPlayerStartT_;
			isPlayerPositionSet_ = false;
			isGameStart_ = true;
		}
		else {
			playerStartTimer_ += 0.5f;
			// 上からプレイヤーが登場する
			player_->SetPosition(Vector3::Lerp({ 8.0f, 20.0f, 8.0f }, { 8.0f, 0.0f, 8.0f }, easeOutBounce(playerStartTimer_ / maxPlayerStartT_)));
		}

	}
}
