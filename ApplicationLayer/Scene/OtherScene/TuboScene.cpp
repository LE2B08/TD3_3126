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
void TuboScene::Initialize()
{
#ifdef _DEBUG
	// デバッグカメラの初期化
	DebugCamera::GetInstance()->Initialize();
#endif // _DEBUG

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	textureManager = TextureManager::GetInstance();
	particleManager = ParticleManager::GetInstance();

	// カメラの初期化
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	// 生成処理
	player_ = std::make_unique<Player>();
	weapon_ = std::make_unique<Weapon>();
	hook_ = std::make_unique<Hook>();
	enemy_ = std::make_unique<Enemy>();
	field_ = std::make_unique<Field>();
	playerUI_ = std::make_unique<PlayerUI>();
	controllerUI_ = std::make_unique<ControllerUI>();
	dynamicCamera_ = std::make_unique<DynamicCamera>();
	effectManager_ = std::make_unique<EffectManager>();

	// 演出の初期化
	effectManager_->Initialize(camera_, input_, player_.get(), field_.get());

	// Playerクラスの初期化
	player_->Initialize();
	player_->SetWeapon(weapon_.get()); // プレイヤーに武器をセット
	player_->SetPosition({ 1000.0f, 1000.0f, 1000.0f });

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
	enemy_->SetPlayer(player_.get());

	// 敵の弾の情報をセット
	enemyBullets_ = &enemy_->GetBullets();

	// フィールドの初期化
	field_->Initialize();
	field_->SetScale(fieldScale_);

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
void TuboScene::Update()
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

	//// 演出の更新
	effectManager_->Update();

	// ダイナミックカメラの更新

	GameStart();

	// 計算したあとのカメラの値をセット
	camera_->SetTranslate(cameraPosition_);
	camera_->SetScale(dynamicCamera_->GetScale());
	camera_->SetRotate(dynamicCamera_->GetRotate());
	camera_->SetTranslate(dynamicCamera_->GetTranslate());
	
	// カメラの更新
	camera_->Update();
	dynamicCamera_->Update();

	// フィールドの更新
	field_->SetScale(fieldScale_);
	field_->Update();

	// フックから移動情報を取得
	if (isGameStart_)
	{
		// プレイヤーの位置をフックにセット
		player_->SetPosition(hook_->GetPlayerPosition());
		player_->SetVelocity(hook_->GetPlayerVelocity());
		player_->SetAcceleration(hook_->GetPlayerAcceleration());

		player_->SetMinMoveLimit(field_->GetMinPosition());
		player_->SetMaxMoveLimit(field_->GetMaxPosition());
	}

	// プレイヤーの更新
	player_->Update();

	if (player_->IsDead())
	{
		sceneManager_->ChangeScene("GameOverScene");
	}

	// プレイヤーUIの更新
	playerUI_->Update();

	// フックの更新処理
	hook_->Update();

	enemy_->SetMinMoveLimit(field_->GetMinPosition());
	enemy_->SetMaxMoveLimit(field_->GetMaxPosition());
	enemy_->Update();

	// 攻撃判定
    if (weapon_->GetIsAttack() && enemy_->GetIsHit()) {
        enemy_->SetIsHitFromAttack(true);
    }

	// コントローラー用UIの更新
	controllerUI_->Update();

	// スカイボックスの更新処理
	skyBox_->Update();

	// 衝突マネージャの更新
	collisionManager_->Update();
	CheckAllCollisions();// 衝突判定と応答
}


/// -------------------------------------------------------------
///				　			　 描画処理
/// -------------------------------------------------------------
void TuboScene::Draw()
{
	/// ------------------------------------------ ///
	/// ---------- スカイボックスの描画 ---------- ///
	/// ------------------------------------------ ///
	SkyBoxManager::GetInstance()->SetRenderSetting();
	skyBox_->Draw();


	/// ------------------------------------------ ///
	/// ---------- スカイボックスの描画 ---------- ///
	/// ------------------------------------------ ///


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
	/// ----------  スプライトの描画  ---------- ///
	/// ---------------------------------------- ///


	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	// プレイヤー
	player_->Draw();

	// フックの描画
	hook_->Draw();

	enemy_->Draw();

	field_->Draw();

	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///

	collisionManager_->Draw();

	// ワイヤーフレームの描画
	// Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, { 0.25f, 0.25f, 0.25f,1.0f });
}


/// -------------------------------------------------------------
///				　			　 終了処理
/// -------------------------------------------------------------
void TuboScene::Finalize()
{

}


/// -------------------------------------------------------------
///				　			ImGui描画処理
/// -------------------------------------------------------------
void TuboScene::DrawImGui()
{

	player_->DrawImGui();
	playerUI_->DrawImGui();
}


/// -------------------------------------------------------------
///				　			衝突判定と応答
/// -------------------------------------------------------------
void TuboScene::CheckAllCollisions()
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

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}


void TuboScene::GameStart()
{
	if (!isGameStartEffectEnabled_)
	{
		isGameStart_ = true;
		return;
	}

	// エンターキーかAボタンが押されるたびに演出を開始
	if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0))
	{
		if (!isStartEasing_)
		{
			startTimer_ = 0.0f;
			playerStartTimer_ = 0.0f;
			isStartEasing_ = true;
		}
	}

	if (isStartEasing_)
	{
		if (startTimer_ >= maxStartT_)
		{
			startTimer_ = maxStartT_;
			isStartEasing_ = false;
			isPlayerPositionSet_ = true;
		}
		else
		{
			startTimer_ += 0.5f;
		}

		fieldScale_ = Vector3::Lerp(startFieldScale_, defaultFieldScale_, easeOutBounce(startTimer_ / maxStartT_));
	}

	if (isPlayerPositionSet_)
	{
		player_->SetPosition({ 8.0f, 20.0f, 8.0f });

		if (playerStartTimer_ >= maxPlayerStartT_)
		{
			playerStartTimer_ = maxPlayerStartT_;
			isPlayerPositionSet_ = false;
			isGameStart_ = true;
		}
		else
		{
			playerStartTimer_ += 0.5f;
			player_->SetPosition(Vector3::Lerp({ 8.0f, 20.0f, 8.0f }, { 8.0f, 0.0f, 8.0f }, easeOutBounce(playerStartTimer_ / maxPlayerStartT_)));
		}
	}
}
