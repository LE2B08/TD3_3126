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
void GamePlayScene::Initialize()
{
#ifdef _DEBUG
	// デバッグカメラの初期化
	DebugCamera::GetInstance()->Initialize();
#endif // _DEBUG

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	textureManager = TextureManager::GetInstance();
	particleManager = ParticleManager::GetInstance();

	fadeManager_ = std::make_unique<FadeManager>();
	fadeManager_->Initialize();

	// シーン開始時に白からフェードアウトする（白 → 透明）
	fadeManager_->StartFadeFromWhite(0.02f);

	// カメラの初期化
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	// 生成処理
	player_ = std::make_unique<Player>();
	weapon_ = std::make_unique<Weapon>();
	hook_ = std::make_unique<Hook>();
	enemy_ = std::make_unique<Enemy>();
	field_ = std::make_unique<Field>();
	playerUI_ = std::make_unique<PlayerUI>();
	enemyUI_ = std::make_unique<EnemyUI>();
	controllerUI_ = std::make_unique<ControllerUI>();
	dynamicCamera_ = std::make_unique<DynamicCamera>();
	effectManager_ = std::make_unique<EffectManager>();

	// 演出の初期化
	effectManager_->Initialize(camera_, input_, player_.get(), field_.get());

	// Playerクラスの初期化
	player_->Initialize();
	player_->SetWeapon(weapon_.get()); // プレイヤーに武器をセット

	// 武器の初期化
	weapon_->SetPlayer(player_.get()); // プレイヤーの情報を武器にセット
	weapon_->SetEnemy(enemy_.get()); // 敵の情報を武器にセット
	weapon_->Initialize();

	// フックの生成 初期化
	hook_->SetPlayer(player_.get());
	hook_->SetEnemy(enemy_.get());
	hook_->SetField(field_.get());
	hook_->Initialize();

	// 敵の初期化
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get()); // プレイヤーの情報を敵にセット
	enemyUI_->Initialize();
	enemyUI_->SetEnemy(enemy_.get()); // 敵の情報をUIにセット



	// 敵の弾の情報をセット
	enemyBullets_ = &enemy_->GetBullets();

	// フィールドの初期化
	field_->Initialize();

	// PlayerUIの初期化
	playerUI_->Initialize();
	playerUI_->SetPlayer(player_.get());

	controllerUI_->Initialize();

	// スカイボックス
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize("rostock_laage_airport_4k.dds");

	// ダイナミックカメラの初期化
	dynamicCamera_->Initialize();
	dynamicCamera_->SetPlayer(player_.get());
	dynamicCamera_->SetEnemy(enemy_.get());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
}


/// -------------------------------------------------------------
///				　			　 更新処理
/// -------------------------------------------------------------
void GamePlayScene::Update()
{
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F12)) {
		Object3DCommon::GetInstance()->SetDebugCamera(!Object3DCommon::GetInstance()->GetDebugCamera());
		Wireframe::GetInstance()->SetDebugCamera(!Wireframe::GetInstance()->GetDebugCamera());
		skyBox_->SetDebugCamera(!skyBox_->GetDebugCamera());
		isDebugCamera_ = !isDebugCamera_;
	}
#endif // _DEBUG

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
		effectManager_->SetIsCameraShaking(true);
	}

	// 演出の更新
	effectManager_->Update();

	// 次の状態がリクエストされたら
	if (nextGameState_) {

		// 次の状態に遷移
		gameState_ = nextGameState_.value();

		// 状態ごとの初期化を一回行う
		switch (gameState_) {

		case GameSceneState::Start:
			GameStartInitialize();
			break;

		case GameSceneState::Play:
			GamePlayInitialize();
			break;

		case GameSceneState::GameClear:
			GameClearInitialize();
			break;

		case GameSceneState::GameOver:
			GameOverInitialize();
			break;

		default:
			break;
		}

		// 次の状態をリセット
		nextGameState_ = std::nullopt;
	}

	// 状態ごとの更新を行う
	switch (gameState_) {

	case GameSceneState::Start:
		GameStartUpdate();
		break;

	case GameSceneState::Play:
		GamePlayUpdate();
		break;

	case GameSceneState::GameClear:
		GameClearUpdate();
		break;

	case GameSceneState::GameOver:
		GameOverUpdate();
		break;

	default:
		break;
	}

	// カメラの更新
	camera_->Update();

	// フィールドの更新
	field_->Update();

	// プレイヤー更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// プレイヤーUIの更新
	playerUI_->Update();
	// 敵のUIの更新
	enemyUI_->Update();

	// コントローラー用UIの更新
	controllerUI_->Update();

	// スカイボックスの更新処理
	skyBox_->Update();

	// 衝突マネージャの更新
	collisionManager_->Update();
	CheckAllCollisions();// 衝突判定と応答

	// フェードマネージャの更新（ここから下は書かない）
	fadeManager_->Update();
}


/// -------------------------------------------------------------
///				　			　 描画処理
/// -------------------------------------------------------------
void GamePlayScene::Draw()
{
#pragma region スカイボックスの描画

	// スカイボックスの描画設定
	SkyBoxManager::GetInstance()->SetRenderSetting();
	skyBox_->Draw();

#pragma endregion


#pragma region スプライトの描画（後面描画・背景用）

	// スプライトの描画設定（後面）
	SpriteManager::GetInstance()->SetRenderSetting_Background();

#pragma endregion


#pragma region オブジェクト3Dの描画

	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	switch (gameState_) {

	case GameSceneState::Start:

		// プレイヤー
		player_->Draw();

		// 敵の描画
		enemy_->Draw();

		break;

	case GameSceneState::Play:

		// プレイヤー
		player_->Draw();

		// 敵の描画
		enemy_->Draw();

		// フックの描画
		hook_->Draw();

		break;

	case GameSceneState::GameClear:

		// 敵の描画
		enemy_->Draw();

		break;

	case GameSceneState::GameOver:

		// プレイヤー
		player_->Draw();

		break;

	default:
		break;
	}

	// フィールドの描画
	field_->Draw();

#pragma endregion


#pragma region スプライトの描画（前面描画・UI用）

	// スプライトの描画設定（前面）
	SpriteManager::GetInstance()->SetRenderSetting_UI();

	// プレイヤーUI
	playerUI_->Draw();

	// 敵のUI
	enemyUI_->Draw();

	// コントローラー用UIの描画
	controllerUI_->Draw();

	// フェードマネージャーの描画（ここから下は書かない）
	fadeManager_->Draw();

#pragma endregion

	// コリジョンマネージャーの描画
	collisionManager_->Draw();

	// ワイヤーフレームの描画
	// Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, { 0.25f, 0.25f, 0.25f,1.0f });
}


/// -------------------------------------------------------------
///				　			　 終了処理
/// -------------------------------------------------------------
void GamePlayScene::Finalize()
{

}


/// -------------------------------------------------------------
///				　			ImGui描画処理
/// -------------------------------------------------------------
void GamePlayScene::DrawImGui()
{
	playerUI_->DrawImGui();
	enemyUI_->DrawImGui();

	enemy_->ShowImGui("Enemy");

	//ImGui::Begin("GamePlayScene");
	//
	//// シーンの状態を表示
	//switch (gameState_) {
	//case GameSceneState::Start:
	//	ImGui::Text("Game Start");
	//	break;
	//case GameSceneState::Play:
	//	ImGui::Text("Game Play");
	//	break;
	//case GameSceneState::GameClear:
	//	ImGui::Text("Game Clear");
	//	break;
	//case GameSceneState::GameOver:
	//	ImGui::Text("Game Over");
	//	break;
	//default:
	//	break;
	//}
	//
	//ImGui::End();
}


/// -------------------------------------------------------------
///				　			衝突判定と応答
/// -------------------------------------------------------------
void GamePlayScene::CheckAllCollisions()
{
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());

	// 攻撃フラグを取得したらコライダーを追加
	if (player_->GetIsAttack())
	{
		collisionManager_->AddCollider(weapon_.get());
	}
	collisionManager_->AddCollider(hook_.get());
	collisionManager_->AddCollider(enemy_.get());

	// 複数についてコライダーをリストに登録
	for (const auto& bullet : *enemyBullets_)
	{
		collisionManager_->AddCollider(bullet.get());
	}

	// プレイヤーが死亡したらコライダーを削除または敵が死亡したらコライダーを削除
	if (player_->GetHp() <= 0 || enemy_->GetHp() <= 0)
	{
		collisionManager_->RemoveCollider(player_.get());
		collisionManager_->RemoveCollider(weapon_.get());
		collisionManager_->RemoveCollider(hook_.get());
		collisionManager_->RemoveCollider(enemy_.get());

		// 複数についてコライダーを削除
		for (const auto& bullet : *enemyBullets_)
		{
			collisionManager_->RemoveCollider(bullet.get());
		}
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

/// -------------------------------------------------------------
///				　		ゲームスタート初期化
/// -------------------------------------------------------------
void GamePlayScene::GameStartInitialize() {

	// アニメーションフラグを下げておく
	isStartAnimation_ = false;
}

/// -------------------------------------------------------------
///				　		ゲームスタート更新
/// -------------------------------------------------------------
void GamePlayScene::GameStartUpdate() {
	// まだアニメーションフラグが立っていなくて対応するキーが押されたら
	if (!isStartAnimation_ && (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0))) {

		// アニメーションを開始
		isStartAnimation_ = true;
	}

	// アニメーションフラグが立っていたら
	if (isStartAnimation_) {

		// フィールドの拡縮を開始
		field_->ScalingAnimation();
	}

	// フィールドの拡縮が終わったら
	if (field_->IsScaleEnd()) {

		// プレイヤーを落とす
		player_->FallingAnimation();
	}


	// プレイヤーの落下が終わったら
	if (player_->GetIsFallEnd()) {

		// 降ってくるアニメーションフラグが立っていなかったら(まだ行っていなかったら)
		if (enemy_->GetIsEnemyCameraEffect()) {

			// カメラ目線にして
			enemy_->SetRotation(Vector3(0.0f, 1.5f, 0.0f));

			// アニメーションが終わったことを確認して
			if (!enemy_->GetIsCameraEffectEnd()) {

				// アニメーションを行う
				enemy_->SpawnEffect();
			}
		}
	}
	
	// 敵の降ってくるアニメーションが終了してたら
	if (enemy_->GetIsCameraEffectEnd()) {

		// 状態をゲームプレイに変更
		nextGameState_ = GameSceneState::Play;
	}
}

/// -------------------------------------------------------------
///				　		ゲームプレイ初期化
/// -------------------------------------------------------------
void GamePlayScene::GamePlayInitialize() {
	enemy_->SetPosition(Vector3(0.0f, 0.0f, 8.0f));
}

/// -------------------------------------------------------------
///				　		ゲームプレイ更新
/// -------------------------------------------------------------
void GamePlayScene::GamePlayUpdate() {

	// プレイヤーの体力がなくなったら
	if (player_->GetHp() <= 0) {
		
		// 状態をゲームオーバーに変更
		nextGameState_ = GameSceneState::GameOver;
	}

	// 敵の体力が無くなったら
	if (enemy_->GetHp() <= 0) {

		// 状態をゲームクリアに変更
		nextGameState_ = GameSceneState::GameClear;
	}

	// フックの更新処理
	hook_->Update();

	// プレイヤーの位置をフックにセット
	player_->SetPosition(hook_->GetPlayerPosition());
	player_->SetVelocity(hook_->GetPlayerVelocity());
	player_->SetAcceleration(hook_->GetPlayerAcceleration());

	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetMaxMoveLimit(field_->GetMaxPosition());

	// 計算したあとのカメラの値をセット
	if (player_->GetHp() > 0) {
		camera_->SetTranslate(cameraPosition_);
		camera_->SetScale(dynamicCamera_->GetScale());
		camera_->SetRotate(dynamicCamera_->GetRotate());
		camera_->SetTranslate(dynamicCamera_->GetTranslate());
	}

	enemy_->SetMinMoveLimit(field_->GetMinPosition());
	enemy_->SetMaxMoveLimit(field_->GetMaxPosition());

	// 攻撃判定
	if (weapon_->GetIsAttack() && enemy_->GetIsHit()) {
		enemy_->SetIsHitFromAttack(true);
	}

	dynamicCamera_->Update();
}

/// -------------------------------------------------------------
///				　		ゲームクリア初期化
/// -------------------------------------------------------------
void GamePlayScene::GameClearInitialize() {
	
}

/// -------------------------------------------------------------
///				　		ゲームクリア更新
/// -------------------------------------------------------------
void GamePlayScene::GameClearUpdate() {
	// 敵の死亡アニメーションが終わったときenemyのisDeadがtrueになる
	enemy_->FaildCameraMove();
	// エンターキーかAボタンが押されたら
	if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {

		fadeManager_->StartFadeToWhite(0.02f, [this]() {
			// ゲームクリアシーンに移動
			sceneManager_->ChangeScene("GameClearScene");
		});
	}
}

/// -------------------------------------------------------------
///				　		ゲームオーバー初期化
/// -------------------------------------------------------------
void GamePlayScene::GameOverInitialize() {
	
}

/// -------------------------------------------------------------
///				　		ゲームオーバー更新
/// -------------------------------------------------------------
void GamePlayScene::GameOverUpdate() {
	// 内部でアニメーションが終わったときplayerのisDeadがtrueになる
	player_->DeathCameraMove();

	// エンターキーかAボタンが押されたら
	if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {

		if (sceneManager_) {
			fadeManager_->StartFadeToWhite(0.02f, [this]() {
				// フェード完了後の処理
				sceneManager_->ChangeScene("GameOverScene"); // シーン名を指定して変更
				});
		}
	}
}
