#include "TuboScene.h"
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
void TuboScene::Initialize() {
#ifdef _DEBUG
	// デバッグカメラの初期化
	DebugCamera::GetInstance()->Initialize();
#endif // _DEBUG

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	textureManager = TextureManager::GetInstance();
	particleManager = ParticleManager::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();
	wavLoader_->StreamAudioAsync("Get-Ready.wav", 0.1f, 1.0f, true);

	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();

	// シーン開始時に白からフェードアウトする（白 → 透明）
	fadeManager_->StartFadeFromWhite(0.02f);

	// カメラの初期化
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	// 生成処理
	player_ = std::make_unique<TutorialPlayer>();
	weapon_ = std::make_unique<Weapon>();
	hook_ = std::make_unique<Hook>();
	enemy_ = std::make_unique<Enemy>();
	field_ = std::make_unique<Field>();
	controllerUI_ = std::make_unique<ControllerUI>();
	dynamicCamera_ = std::make_unique<DynamicCamera>();
	
	playerDirectionalArrow_ = std::make_unique<PlayerDirectionalArrow>();

	
	// Playerクラスの初期化
	player_->Initialize();
	player_->SetWeapon(weapon_.get()); // プレイヤーに武器をセット

	// 武器の初期化
	weapon_->SetTutorialPlayer(player_.get()); // プレイヤーの情報を武器にセット
	weapon_->SetEnemy(enemy_.get());           // 敵の情報を武器にセット
	weapon_->Initialize();

	// フックの生成 初期化
	hook_->SetTutorialPlayer(player_.get());
	hook_->SetEnemy(enemy_.get());
	hook_->SetField(field_.get());
	hook_->Initialize();

	// 敵の初期化
	 //enemy_->Initialize();
	 //enemy_->SetTutorialPlayer(tutorialplayer_.get()); // プレイヤーの情報を敵にセット
	// 敵の弾の情報をセット
	// enemyBullets_ = &enemy_->GetBullets();

	// フィールドの初期化
	field_->Initialize();

	controllerUI_->Initialize();

	// スカイボックス
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize("rostock_laage_airport_4k.dds");

	// ダイナミックカメラの初期化
	dynamicCamera_->Initialize();
	dynamicCamera_->SetTutorialPlayer(player_.get());
	dynamicCamera_->SetEnemy(enemy_.get());

	// ポーズメニューの初期化
	pauseMenu_ = std::make_unique<PauseMenu>();
	pauseMenu_->Initialize();

	playerDirectionalArrow_->Initialize();
	playerDirectionalArrow_->SetTutorialPlayer(player_.get());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
}

/// -------------------------------------------------------------
///				　			　 更新処理
/// -------------------------------------------------------------
void TuboScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F12)) {
		Object3DCommon::GetInstance()->SetDebugCamera(!Object3DCommon::GetInstance()->GetDebugCamera());
		Wireframe::GetInstance()->SetDebugCamera(!Wireframe::GetInstance()->GetDebugCamera());
		skyBox_->SetDebugCamera(!skyBox_->GetDebugCamera());
		isDebugCamera_ = !isDebugCamera_;
	}

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
#endif // _DEBUG

	
	// 次の状態がリクエストされたら
	if (nextGameState_) {

		// 次の状態に遷移
		sceneStatus_ = nextGameState_.value();

		switch (sceneStatus_) {
	
		case SceneStatus::Pause:
			// ポーズメニューの初期化
			PauseInitialize();
			break;	
		case SceneStatus::Play:
			// ゲームプレイの初期化
			PlayInitialize();
			break;
		default:
			break;
		}

		// 次の状態をリセット
		nextGameState_ = std::nullopt;
	}

	switch (sceneStatus_) {
	case SceneStatus::Pause:
		// ポーズメニューの更新
		PauseUpdate();
		break;

	case SceneStatus::Play:

		// エスケープキーかスタートボタンを押したら
		if (input_->TriggerKey(DIK_ESCAPE) || input_->TriggerButton(12)) {
			// ポーズ状態に変更
			nextGameState_ = SceneStatus::Pause;
		}

		PlayUpdate();

		
		switch (tutorialSteps_) {
		case TutorialSteps::Start:
			// チュートリアル開始時の初期化
			TutorialStartUpdate();
			break;

		case TutorialSteps::PlayerRotation:
			// プレイヤーの回転の更新
			TutorialPlayerRotationUpdate();
			break;
		case TutorialSteps::HookThrowAndBack:
			// フックを投げて戻すの更新
			TutorialHookThrowAndBackUpdate();
			break;
		case TutorialSteps::HookArcMove:
			// フックの弧を描く移動の更新
			TutorialHookArcMoveUpdate();
			break;
		case TutorialSteps::HookMove:
			// フックの移動の更新
			TutorialHookMoveUpdate();
			break;
		case TutorialSteps::Attack:
			// 攻撃の更新
			TutorialAttackUpdate();
			break;

		case TutorialSteps::End:
			// チュートリアル終了の更新
			TutorialEndUpdate();
			break;
		default:
			break;
		}
		break;

	
	default:
		break;
	}

	// カメラの更新
	camera_->SetTranslate(cameraPosition_);
	camera_->SetScale(dynamicCamera_->GetScale());
	camera_->SetRotate(dynamicCamera_->GetRotate());
	camera_->SetTranslate(dynamicCamera_->GetTranslate());
	camera_->Update();
	dynamicCamera_->Update();

	field_->TutorialScale();
	// フィールドの更新
	field_->Update();

	// スカイボックスの更新処理
	skyBox_->Update();
	// 衝突マネージャの更新
	collisionManager_->Update();
	CheckAllCollisions(); // 衝突判定と応答
	// フェードマネージャの更新（ここから下は書かない）
	fadeManager_->Update();
}

/// -------------------------------------------------------------
///				　			　 描画処理
/// -------------------------------------------------------------
void TuboScene::Draw() {
#pragma region スカイボックスの描画

	// スカイボックスの描画設定
	SkyBoxManager::GetInstance()->SetRenderSetting();
	// skyBox_->Draw();

#pragma endregion

#pragma region スプライトの描画（後面描画・背景用）

	// スプライトの描画設定（後面）
	SpriteManager::GetInstance()->SetRenderSetting_Background();

#pragma endregion

#pragma region オブジェクト3Dの描画

	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// プレイヤー
	player_->Draw();

	// 敵の描画
	// enemy_->Draw();

	// フックの描画
	hook_->Draw();
	// 矢印の描画
	playerDirectionalArrow_->Draw();

	switch (sceneStatus_) {
	case SceneStatus::Play:

		switch (tutorialSteps_) {
		case TutorialSteps::Start:
			break;

		case TutorialSteps::PlayerRotation:
			break;
		case TutorialSteps::HookThrowAndBack:
			break;
		case TutorialSteps::HookArcMove:
			break;
		case TutorialSteps::HookMove:
			break;
		case TutorialSteps::Attack:
			break;

		case TutorialSteps::End:
			break;
		default:
			break;
		}
		break;

	case SceneStatus::Pause:
		break;
	default:
		break;
	}

	// フィールドの描画
	field_->Draw();
	// スカイドームの描画

#pragma endregion

#pragma region スプライトの描画（前面描画・UI用）

	// スプライトの描画設定（前面）
	SpriteManager::GetInstance()->SetRenderSetting_UI();

	switch (sceneStatus_) {
	case SceneStatus::Play:
		// コントローラー用UIの描画
		controllerUI_->Draw();

		switch (tutorialSteps_) {
		case TutorialSteps::Start:
			break;

		case TutorialSteps::PlayerRotation:
			break;
		case TutorialSteps::HookThrowAndBack:
			break;
		case TutorialSteps::HookArcMove:
			break;
		case TutorialSteps::HookMove:
			break;
		case TutorialSteps::Attack:
			break;

		case TutorialSteps::End:
			break;
		default:
			break;
		}
		break;
	case SceneStatus::Pause:
		// poseMenuの描画
		pauseMenu_->Draw();
		break;
	default:
		break;
	}

	// フェードマネージャーの描画（ここから下は書かない）
	fadeManager_->Draw();

#pragma endregion

#ifdef _DEBUG
	// コリジョンマネージャーの描画
	collisionManager_->Draw();
#endif // _DEBUG

	// ワイヤーフレームの描画
	// Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, { 0.25f, 0.25f, 0.25f,1.0f });
}

/// -------------------------------------------------------------
///				　			　 終了処理
/// -------------------------------------------------------------
void TuboScene::Finalize() {}

/// -------------------------------------------------------------
///				　			ImGui描画処理
/// -------------------------------------------------------------

void TuboScene::DrawImGui() {
	enemy_->ShowImGui("Enemy");

	// SceneStatusの表示
	ImGui::Begin("TuboScene State");
	ImGui::Text("SceneStatus: %s", sceneStatus_ == SceneStatus::Play ? "Play" : sceneStatus_ == SceneStatus::Pause ? "Pause" : "Unknown");

	// TutorialStepsの表示
	ImGui::Text(
	    "TutorialSteps: %s", tutorialSteps_ == TutorialSteps::Start              ? "Start"
	                         : tutorialSteps_ == TutorialSteps::PlayerRotation   ? "PlayerRotation"
	                         : tutorialSteps_ == TutorialSteps::HookThrowAndBack ? "HookThrowAndBack"
	                         : tutorialSteps_ == TutorialSteps::HookArcMove      ? "HookArcMove"
	                         : tutorialSteps_ == TutorialSteps::HookMove         ? "HookMove"
	                         : tutorialSteps_ == TutorialSteps::Attack           ? "Attack"
	                         : tutorialSteps_ == TutorialSteps::End              ? "End"
	                                                                             : "Unknown");

	ImGui::End();
}

/// -------------------------------------------------------------
///				　			衝突判定と応答
/// -------------------------------------------------------------
void TuboScene::CheckAllCollisions() {
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());

	// 攻撃フラグを取得したらコライダーを追加
	if (player_->GetIsAttack()) {
		collisionManager_->AddCollider(weapon_.get());
	}
	collisionManager_->AddCollider(hook_.get());
	collisionManager_->AddCollider(enemy_.get());

	// プレイヤーが死亡したらコライダーを削除または敵が死亡したらコライダーを削除
	if (player_->GetHp() <= 0 || enemy_->GetHp() <= 0) {
		collisionManager_->RemoveCollider(player_.get());
		collisionManager_->RemoveCollider(weapon_.get());
		collisionManager_->RemoveCollider(hook_.get());
		collisionManager_->RemoveCollider(enemy_.get());

		// 複数についてコライダーを削除
		for (const auto& bullet : *enemyBullets_) {
			collisionManager_->RemoveCollider(bullet.get());
		}
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

void TuboScene::TutorialStartInitialize() {}

void TuboScene::TutorialStartUpdate() {

	// 次に行く処理
	SetTutorialStep(TutorialSteps::PlayerRotation);
}

void TuboScene::TutorialPlayerRotationInitialize() {}

void TuboScene::TutorialPlayerRotationUpdate() {

	// 次に行く処理
	SetTutorialStep(TutorialSteps::HookThrowAndBack);
}

void TuboScene::TutorialHookThrowAndBackInitialize() {}

void TuboScene::TutorialHookThrowAndBackUpdate() {
	// 次に行く処理
	SetTutorialStep(TutorialSteps::HookArcMove);
}

void TuboScene::TutorialHookArcMoveInitialize() {}

void TuboScene::TutorialHookArcMoveUpdate() {
	// 次に行く処理
	SetTutorialStep(TutorialSteps::HookMove);
}

void TuboScene::TutorialHookMoveInitialize() {}

void TuboScene::TutorialHookMoveUpdate() {
	// 次に行く処理
	SetTutorialStep(TutorialSteps::Attack);
}

void TuboScene::TutorialAttackInitialize() {}

void TuboScene::TutorialAttackUpdate() {

	// 次に行く処理
	SetTutorialStep(TutorialSteps::End);
}

void TuboScene::PlayInitialize() {}

void TuboScene::PlayUpdate() {

	hook_->Update();
	// プレイヤーの位置をフックにセット
	player_->SetPosition(hook_->GetPlayerPosition());
	// プレイヤーの向きをフックにセット
	player_->SetVelocity(hook_->GetPlayerVelocity());
	// プレイヤーの加速度をフックにセット
	player_->SetAcceleration(hook_->GetPlayerAcceleration());

	// プレイヤーの移動制限をフィールドにセット
	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetMaxMoveLimit(field_->GetMaxPosition());
	//プレイヤーの更新
	player_->Update();

	
	// コントローラー用UIの更新
	controllerUI_->Update();
	// プレイヤーの方向UIの更新
	playerDirectionalArrow_->Update();
	
	
}

void TuboScene::TutorialEndInitialize() {}

void TuboScene::TutorialEndUpdate() {}

/// -------------------------------------------------------------
///				　		  ポーズ初期化
/// -------------------------------------------------------------
void TuboScene::PauseInitialize() {}

/// -------------------------------------------------------------
///				　		   ポーズ更新
/// -------------------------------------------------------------
void TuboScene::PauseUpdate() {

	// ポーズメニューの更新
	pauseMenu_->Update();

	// エスケープキーかSTARTボタンを押したら
	if (input_->TriggerKey(DIK_ESCAPE) || input_->TriggerButton(12)) {
		// ゲームプレイ状態に変更
		nextGameState_ = SceneStatus::Play;
	}

	// メニューの状態が「ゲームに戻る」でAボタンが押されたら
	if (pauseMenu_->GetMenuState() == MenuState::ReturnToGame) {
		if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {
			// ゲームプレイ状態に変更
			nextGameState_ = SceneStatus::Play;
		}
	}

	// メニューの状態が「遊び方」でAボタンが押されたら
	if (pauseMenu_->GetMenuState() == MenuState::HowToPlay) {
		if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {
			// 遊び方の説明を表示する処理を追加
			// 例: ShowHowToPlay();
		}
	}

	// メニューの状態が「タイトルに戻る」でAボタンが押されたら
	if (pauseMenu_->GetMenuState() == MenuState::ReturnToTitle) {
		if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {
			if (sceneManager_) {
				fadeManager_->StartFadeToWhite(0.02f, [this]() { sceneManager_->ChangeScene("TitleScene"); });
			}
		}
	}
}

void TuboScene::SetTutorialStep(TutorialSteps step) {
	tutorialSteps_ = step;
	switch (tutorialSteps_) {
	case TutorialSteps::Start:
		TutorialStartInitialize();
		break;
	case TutorialSteps::PlayerRotation:
		TutorialPlayerRotationInitialize();
		break;
	case TutorialSteps::HookThrowAndBack:
		TutorialHookThrowAndBackInitialize();
		break;
	case TutorialSteps::HookArcMove:
		TutorialHookArcMoveInitialize();
		break;
	case TutorialSteps::HookMove:
		TutorialHookMoveInitialize();
		break;
	case TutorialSteps::Attack:
		TutorialAttackInitialize();
		break;
	case TutorialSteps::End:
		TutorialEndInitialize();
		break;
	default:
		break;
	}
}
