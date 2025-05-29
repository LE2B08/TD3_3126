#include "TutorialScene.h"
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

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void TutorialScene::Initialize() {

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
	enemy_ = std::make_unique<TutorialEnemy>();
	field_ = std::make_unique<Field>();
	controllerUI_ = std::make_unique<ControllerUI>();
	dynamicCamera_ = std::make_unique<DynamicCamera>();
	tutorialUI_ = std::make_unique<TutorialUI>();

	playerDirectionalArrow_ = std::make_unique<PlayerDirectionalArrow>();

	// Playerクラスの初期化
	player_->Initialize();
	player_->SetWeapon(weapon_.get()); // プレイヤーに武器をセット
	player_->SetPosition({ 0.0f, 0.0f, -8.0f }); // プレイヤーの初期位置を設定

	// 武器の初期化
	weapon_->SetTutorialPlayer(player_.get()); // プレイヤーの情報を武器にセット
	weapon_->SetTutorialEnemy(enemy_.get());   // 敵の情報を武器にセット
	weapon_->Initialize();

	// フックの生成 初期化
	hook_->SetTutorialPlayer(player_.get());
	hook_->SetTutorialEnemy(enemy_.get());
	hook_->SetField(field_.get());
	hook_->Initialize();

	// 敵の初期化
	enemy_->Initialize();
	enemy_->SetPosition({ 0.0f, 0.0f, 0.0f });
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
	dynamicCamera_->SetTutorialEnemy(enemy_.get());

	// ポーズメニューの初期化
	pauseMenu_ = std::make_unique<PauseMenu>();
	pauseMenu_->Initialize();

	playerDirectionalArrow_->Initialize();
	playerDirectionalArrow_->SetTutorialPlayer(player_.get());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// チュートリアルUIの初期化
	tutorialUI_->Initialize();

	// テクスチャの読み込み
	textureManager->LoadTexture("Resources/Green.png");
	// スプライトの初期化
	gauge_ = std::make_unique<Sprite>();
	gauge_->Initialize("Resources/Green.png");
	gauge_->SetPosition(gaugePosition_);
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void TutorialScene::Update() {

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

	// シーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("GamePlayScene"); // ゲームプレイシーンに戻る
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
	dynamicCamera_->Update();
	camera_->SetTranslate(cameraPosition_);
	camera_->SetScale(dynamicCamera_->GetScale());
	camera_->SetRotate(dynamicCamera_->GetRotate());
	camera_->SetTranslate(dynamicCamera_->GetTranslate());
	camera_->Update();

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

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void TutorialScene::Draw() {

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
	enemy_->Draw();

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
			// チュートリアル開始のUIの描画
			//tutorialUI_->StartDraw();
			break;

		case TutorialSteps::PlayerRotation:
			// プレイヤーの回転のUIの描画
			//tutorialUI_->RotationDraw();
			// ゲージの描画
			gauge_->Draw();
			break;
		case TutorialSteps::HookThrowAndBack:
			// フックを投げて戻すのUIの描画
			//tutorialUI_->ThrowDraw();
			// ゲージの描画
			gauge_->Draw();
			break;
		case TutorialSteps::HookArcMove:
			// フックの弧を描く移動のUIの描画
			//tutorialUI_->ArcDraw();
			// ゲージの描画
			gauge_->Draw();
			break;
		case TutorialSteps::HookMove:
			// フックの移動のUIの描画
			//tutorialUI_->MoveDraw();
			// ゲージの描画
			gauge_->Draw();
			break;
		case TutorialSteps::Attack:
			// 攻撃のUIの描画
			//tutorialUI_->AttackDraw();
			// ゲージの描画
			gauge_->Draw();
			break;

		case TutorialSteps::End:
			// チュートリアル終了のUIの描画
			//tutorialUI_->EndDraw();
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

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void TutorialScene::Finalize() {
}

///-------------------------------------------/// 
/// ImGuiの描画処理
///-------------------------------------------///
void TutorialScene::DrawImGui() {

	enemy_->ShowImGui("Enemy");

	// SceneStatusの表示
	ImGui::Begin("TuboScene State");
	ImGui::Text("SceneStatus: %s", sceneStatus_ == SceneStatus::Play ? "Play" : sceneStatus_ == SceneStatus::Pause ? "Pause" : "Unknown");

	// TutorialStepsの表示
	ImGui::Text(
		"TutorialSteps: %s", tutorialSteps_ == TutorialSteps::Start ? "Start"
		: tutorialSteps_ == TutorialSteps::PlayerRotation ? "PlayerRotation"
		: tutorialSteps_ == TutorialSteps::HookThrowAndBack ? "HookThrowAndBack"
		: tutorialSteps_ == TutorialSteps::HookArcMove ? "HookArcMove"
		: tutorialSteps_ == TutorialSteps::HookMove ? "HookMove"
		: tutorialSteps_ == TutorialSteps::Attack ? "Attack"
		: tutorialSteps_ == TutorialSteps::End ? "End"
		: "Unknown");

	// --- ゲージ表示 ---
	// プレイヤー回転（時間ゲージ）
	ImGui::Text("Player Rotation");
	ImGui::ProgressBar(rotationStepTimer_ / 3.0f, ImVec2(200, 20));
	ImGui::SameLine();
	ImGui::Text("%.1f / 3.0 sec", rotationStepTimer_);

	// フック投げ回数
	ImGui::Text("Hook Throw");
	ImGui::ProgressBar((float)hookThrowCount_ / 2.0f, ImVec2(200, 20));
	ImGui::SameLine();
	ImGui::Text("%d / 2", hookThrowCount_);

	// フック戻し回数
	ImGui::Text("Hook Back");
	ImGui::ProgressBar((float)hookBackCount_ / 2.0f, ImVec2(200, 20));
	ImGui::SameLine();
	ImGui::Text("%d / 2", hookBackCount_);

	// 弧移動（時間ゲージ）
	ImGui::Text("Arc Move");
	ImGui::ProgressBar(arcMoveStepTimer_ / 5.0f, ImVec2(200, 20));
	ImGui::SameLine();
	ImGui::Text("%.1f / 5.0 sec", arcMoveStepTimer_);

	// フック移動回数
	ImGui::Text("Hook Move");
	ImGui::ProgressBar((float)hookActiveCount_ / 3.0f, ImVec2(200, 20));
	ImGui::SameLine();
	ImGui::Text("%d / 3", hookActiveCount_);

	// 攻撃回数
	ImGui::Text("Attack");
	ImGui::ProgressBar((float)playerAttackCount_ / 3.0f, ImVec2(200, 20));
	ImGui::SameLine();
	ImGui::Text("%d / 3", playerAttackCount_);

	ImGui::End();

	enemy_->ShowImGui("Enemy");
}

///-------------------------------------------/// 
/// 衝突判定と応答処理
///-------------------------------------------///
void TutorialScene::CheckAllCollisions() {

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
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

///-------------------------------------------/// 
/// チュートリアル開始時初期化
///-------------------------------------------///
void TutorialScene::TutorialStartInitialize() {
}

///-------------------------------------------/// 
/// チュートリアル開始時更新
///-------------------------------------------///
void TutorialScene::TutorialStartUpdate() {

	// Aボタンが押されたら
	if (Input::GetInstance()->PushButton(0)) {

		// 次に行く処理
		SetTutorialStep(TutorialSteps::PlayerRotation);
	}
}

///-------------------------------------------/// 
/// プレイヤーの回転初期化
///-------------------------------------------///
void TutorialScene::TutorialPlayerRotationInitialize() {

	rotationStepTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// プレイヤーの回転更新
///-------------------------------------------///
void TutorialScene::TutorialPlayerRotationUpdate() {

	static constexpr float kRotationStepWaitTime = 3.0f;

	if (player_->GetIsRotation()) {
		rotationStepTimer_ += ImGui::GetIO().DeltaTime; // フレーム時間を加算
	}

	// ゲージの進行度を更新
	GaugeProgress(rotationStepTimer_, kRotationStepWaitTime);

	// タイマーが指定時間を超えたら
	if (rotationStepTimer_ >= kRotationStepWaitTime) {

		// Aボタンが押されたら
		if (Input::GetInstance()->PushButton(0)) {

			// チュートリアルステップをフックの投げ戻しに変更
			SetTutorialStep(TutorialSteps::HookThrowAndBack);
			rotationStepTimer_ = 0.0f;
		}
	}
}

///-------------------------------------------/// 
/// フックの投げ戻し初期化
///-------------------------------------------///
void TutorialScene::TutorialHookThrowAndBackInitialize() {

	hookThrowCount_ = 0;
	hookBackCount_ = 0;
}

///-------------------------------------------/// 
/// フックの投げ戻し更新
///-------------------------------------------///
void TutorialScene::TutorialHookThrowAndBackUpdate() {

	// フックを投げた回数をカウント
	static bool prevIsThrow = false;
	static bool prevIsBack = false;

	bool isThrow = hook_->GetIsThrowing();
	bool isBack = hook_->GetIsBack();

	// 立ち上がり判定でカウント
	if (isThrow && !prevIsThrow) {
		++hookThrowCount_;
	}
	if (isBack && !prevIsBack) {
		++hookBackCount_;
	}

	prevIsThrow = isThrow;
	prevIsBack = isBack;

	// ゲージの進行度を更新
	GaugeProgress((float)hookThrowCount_, (float)maxHookThrowCount_);

	// 投げ戻しの回数がそれぞれ最大値に達したら
	if (hookThrowCount_ >= maxHookThrowCount_ && hookBackCount_ >= maxHookBackCount_) {

		// Aボタンが押されたら
		if (Input::GetInstance()->PushButton(0)) {

			// チュートリアルステップをフックの弧を描く移動に変更
			SetTutorialStep(TutorialSteps::HookArcMove);
		}
	}
}

///-------------------------------------------/// 
/// 弧の移動初期化
///-------------------------------------------///
void TutorialScene::TutorialHookArcMoveInitialize() {

	arcMoveStepTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// 弧の移動更新
///-------------------------------------------///
void TutorialScene::TutorialHookArcMoveUpdate() {

	static constexpr float kArcMoveStepWaitTime = 5.0f;

	if (hook_->GetIsArcMove()) {
		arcMoveStepTimer_ += ImGui::GetIO().DeltaTime;
	}

	// ゲージの進行度を更新
	GaugeProgress(arcMoveStepTimer_, kArcMoveStepWaitTime);

	// タイマーが指定時間を超えたら
	if (arcMoveStepTimer_ >= kArcMoveStepWaitTime) {

		// Aボタンが押されたら
		if (Input::GetInstance()->PushButton(0)) {

			// チュートリアルステップをフックの移動に変更
			SetTutorialStep(TutorialSteps::HookMove);
			arcMoveStepTimer_ = 0.0f;
		}
	}
}

///-------------------------------------------/// 
/// フックの移動初期化
///-------------------------------------------///
void TutorialScene::TutorialHookMoveInitialize() {

	hookActiveCount_ = 0;
}

///-------------------------------------------/// 
/// フックの移動更新
///-------------------------------------------///
void TutorialScene::TutorialHookMoveUpdate() {

	static bool prevIsActive = false;
	bool isActive = hook_->GetIsPulling();

	// 立ち上がり判定でカウント
	if (isActive && !prevIsActive) {
		++hookActiveCount_;
	}
	prevIsActive = isActive;

	// ゲージの進行度を更新
	GaugeProgress((float)hookActiveCount_, static_cast<float>(maxHookActiveCount_));

	// 最大回数になったら
	if (hookActiveCount_ >= maxHookActiveCount_) {

		// Aボタンが押されたら
		if (Input::GetInstance()->PushButton(0)) {

			// チュートリアルステップを攻撃に変更
			SetTutorialStep(TutorialSteps::Attack);
		}
	}
}

///-------------------------------------------/// 
/// 攻撃の初期化
///-------------------------------------------///
void TutorialScene::TutorialAttackInitialize() {

	playerAttackCount_ = 0;
}

///-------------------------------------------/// 
/// 攻撃の更新
///-------------------------------------------///
void TutorialScene::TutorialAttackUpdate() {

	static bool prevIsAttack = false;
	bool isAttack = player_->GetIsAttack();

	// 立ち上がり判定でカウント
	if (isAttack && !prevIsAttack) {
		++playerAttackCount_;
	}
	prevIsAttack = isAttack;

	// ゲージの進行度を更新
	GaugeProgress((float)playerAttackCount_, static_cast<float>(maxPlayerAttackCount_));

	// 最大回数になったら
	if (playerAttackCount_ >= maxPlayerAttackCount_) {

		// Aボタンが押されたら
		if (Input::GetInstance()->PushButton(0)) {
			// チュートリアルステップをチュートリアル終了に変更
			SetTutorialStep(TutorialSteps::End);
		}
	}
}

///-------------------------------------------/// 
/// シーン初期化
///-------------------------------------------///
void TutorialScene::PlayInitialize() {
}

///-------------------------------------------/// 
/// シーン更新
///-------------------------------------------///
void TutorialScene::PlayUpdate() {

	field_->Update();

	hook_->SetTutorialEnemy(enemy_.get());
	hook_->SetEnemyPosition(enemy_->GetPosition());
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

	enemy_->SetMinMoveLimit(field_->GetMinPosition());
	enemy_->SetMaxMoveLimit(field_->GetMaxPosition());

	// 攻撃判定
	if (weapon_->GetIsAttack() && enemy_->GetIsHit()) {
		enemy_->SetIsHitFromAttack(true);
	}
	enemy_->SetTutorialPlayer(player_.get());

	// プレイヤーの更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// コントローラー用UIの更新
	controllerUI_->Update();
	// プレイヤーの方向UIの更新
	playerDirectionalArrow_->Update();
	// チュートリアルUIの更新
	tutorialUI_->Update();
}

///-------------------------------------------/// 
/// ポーズ初期化
///-------------------------------------------///
void TutorialScene::PauseInitialize() {
}

///-------------------------------------------/// 
/// ポーズ更新
///-------------------------------------------///
void TutorialScene::PauseUpdate() {

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

///-------------------------------------------/// 
/// チュートリアル終了初期化
///-------------------------------------------///
void TutorialScene::TutorialEndInitialize() {
}

///-------------------------------------------/// 
/// チュートリアル終了更新
///-------------------------------------------///
void TutorialScene::TutorialEndUpdate() {
}

///-------------------------------------------/// 
/// チュートリアルステップを設定
///-------------------------------------------///
void TutorialScene::SetTutorialStep(TutorialSteps step) {

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

void TutorialScene::GaugeProgress(float count, float maxCount) {

	// カウントが最大値を超えたら
	if (count > maxCount) {

		count = maxCount; // 最大値に制限
	}

	// 進行度の割合を計算
	const float progressRatio_ = count / maxCount;

	// ゲージのサイズを計算
	Vector2 size = { gaugeDefaultSize_.x * progressRatio_, gaugeDefaultSize_.y };

	// ゲージにサイズを設定
	gauge_->SetSize(size);

	// ゲージの更新
	gauge_->Update();
}