#include "GamePlayScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include "SceneManager.h"
#include "Object3DCommon.h"
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "Wireframe.h"
#include "Camera.h"
#include "Easing.h"

#ifdef _DEBUG
#include <DebugCamera.h>
#endif // _DEBUG

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

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	camera_->SetRotate({ 1.57f,0.0f,0.0f });
	camera_->SetTranslate(cameraPosition_);

	/// ---------- サウンドの初期化 ---------- ///
	wavLoader_ = std::make_unique<WavLoader>();
	wavLoader_->StreamAudioAsync("Get-Ready.wav", 0.1f, 1.0f, false);

	player_ = std::make_unique<Player>();
	player_->Initialize();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get());

	enemyBullets_ = &enemy_->GetBullets();

	//fieldScale_ = { 0.0f,0.0f,0.0f };
	field_ = std::make_unique<Field>();
	field_->Initialize();
	field_->SetScale(fieldScale_);
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
	if (input_->TriggerKey(DIK_F12))
	{
		Object3DCommon::GetInstance()->SetDebugCamera(!Object3DCommon::GetInstance()->GetDebugCamera());
		Wireframe::GetInstance()->SetDebugCamera(!Wireframe::GetInstance()->GetDebugCamera());
		isDebugCamera_ = !isDebugCamera_;
	}
#endif // _DEBUG

	// シーン切り替え
	if (input_->TriggerKey(DIK_F1))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("TuboScene");
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_F2))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	// シーン切り替え
	if (input_->TriggerKey(DIK_F3))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("SatouScene");
		}
	}
	if (player_->GetIsHit()) {
		// カメラの揺れを開始
		isCameraShaking_ = true;
		shakeElapsedTime_ = 0.0f;
	}
	CameraShake();

	camera_->Update();

	// ゲーム開始演出
	GameStart();
	
	

	field_->SetScale(fieldScale_);
	field_->Update();
	
	weapon_ = player_->GetWeapon();

	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetMaxMoveLimit(field_->GetMaxPosition());
	player_->SetEnemy(enemy_.get());
	player_->SetIsGameStart(isGameStart_);
	player_->Update();

	enemy_->Update();

	collisionManager_->Update();
	if (isGameStart_) {
		// 衝突判定と応答
		CheckAllCollisions();
		player_->CheckAllCollisions();
	}
}


/// -------------------------------------------------------------
///				　			　 描画処理
/// -------------------------------------------------------------
void GamePlayScene::Draw()
{


	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	player_->Draw();

	enemy_->Draw();

	field_->Draw();

	collisionManager_->Draw();

	// ワイヤーフレームの描画
	//Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, { 0.25f, 0.25f, 0.25f,1.0f });

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

	enemy_->ShowImGui("Enemy");

	field_->ShowImGui("Field");
}


/// -------------------------------------------------------------
///				　			衝突判定と応答
/// -------------------------------------------------------------
void GamePlayScene::CheckAllCollisions()
{
	/*------プレイヤーと敵------*/
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
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

void GamePlayScene::CameraShake()
{
	// カメラの揺れを更新
	if (isCameraShaking_) {
		shakeElapsedTime_ += 1.0f / 60.0f;
		if (shakeElapsedTime_ >= shakeDuration_) {
			isCameraShaking_ = false;
			camera_->SetTranslate(cameraPosition_); // 元の位置に戻す
		} else {
			// ランダムな揺れを生成
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(-shakeMagnitude_, shakeMagnitude_);
			Vector3 shakeOffset = { dis(gen), dis(gen), dis(gen) };
			camera_->SetTranslate(cameraPosition_ + shakeOffset);
		}
	}
}

void GamePlayScene::GameStart()
{
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
		//easeTを40から0まで変動させる
		if (startTimer_ >= maxStartT_) {
			startTimer_ = maxStartT_;
			isGameStart_ = true;
			isStartEasing_ = false;
			isPlayerPositionSet_ = true;
		} else {
			startTimer_ += 0.5f;
		}
		// ポヨンポヨンしてフィールドが広がる演出
		fieldScale_ = Vector3::Lerp(startFieldScale_, defaultFieldScale_, easeOutBounce(startTimer_ / maxStartT_));
	}

	// プレイヤーの位置がセットされたか
	if (isPlayerPositionSet_) {
		// プレイヤーの位置をセット
		player_->SetPosition({ 8.0f, 20.0f, 8.0f });
		if(playerStartTimer_ >= maxPlayerStartT_){
			playerStartTimer_ = maxPlayerStartT_;
			isPlayerPositionSet_ = false;
		} else {
			playerStartTimer_ += 0.5f;
		}
		// 上からプレイヤーが登場する
		player_->SetPosition(Vector3::Lerp({ 8.0f, 20.0f, 8.0f }, { 8.0f, 0.0f, 8.0f }, easeOutBounce(playerStartTimer_ / maxPlayerStartT_)));
	}
	
}
