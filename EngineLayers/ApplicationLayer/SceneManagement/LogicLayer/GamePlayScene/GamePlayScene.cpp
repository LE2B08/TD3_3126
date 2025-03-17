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

#include "FadeManager.h"

#ifdef _DEBUG
#include <DebugCamera.h>
#endif // _DEBUG

using namespace Easing;

/// -------------------------------------------------------------
///				　			　初期化処理
/// -------------------------------------------------------------
void GamePlayScene::Initialize()
{
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

	/// ---------- サウンドの初期化 ---------- ///
	wavLoader_ = std::make_unique<WavLoader>();
	wavLoader_->StreamAudioAsync("Get-Ready.wav", 0.1f, 1.0f, false);

	player_ = std::make_unique<Player>();
	player_->Initialize();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get());

	enemyBullets_ = &enemy_->GetBullets();

	field_ = std::make_unique<Field>();
	field_->Initialize();

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

	if (FadeManager::GetInstance()->IsFadeComplete())
	{
		camera_->SetRotate({ 1.57f,0.0f,0.0f });
		camera_->SetTranslate(cameraPosition_);
	}

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

	field_->Update();

	weapon_ = player_->GetWeapon();

	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetMaxMoveLimit(field_->GetMaxPosition());
	player_->SetEnemy(enemy_.get());
	player_->Update();

	enemy_->Update();

	collisionManager_->Update();
	// 衝突判定と応答
	CheckAllCollisions();
	player_->CheckAllCollisions();
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
		}
		else {
			// ランダムな揺れを生成
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(-shakeMagnitude_, shakeMagnitude_);
			Vector3 shakeOffset = { dis(gen), dis(gen), dis(gen) };
			camera_->SetTranslate(cameraPosition_ + shakeOffset);
		}
	}
}
