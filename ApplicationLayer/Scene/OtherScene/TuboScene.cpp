#include "TuboScene.h"
#include "FadeManager.h"
#include "SceneManager.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>

#include "Object3DCommon.h"
#include <SkyBoxManager.h>
#include <SpriteManager.h>

/// -------------------------------------------------------------
///				　			　初期化処理
/// -------------------------------------------------------------
void TuboScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// フィールドの初期化
	field_ = std::make_unique<Field>();
	field_->Initialize();

	// Playerクラスの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();

	// PlayerUIの初期化
	playerUI_ = std::make_unique<PlayerUI>();
	playerUI_->Initialize();
	playerUI_->SetPlayer(player_.get());

	// 武器の初期化
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	// フックの生成 初期化
	hook_ = std::make_unique<Hook>();
	/*hook_->SetPlayerPosition(player_->GetPosition());
	hook_->SetPlayerRotation(player_->GetRotation());
	hook_->SetPlayerVelocity(player_->GetVelocity());
	hook_->SetPlayerAcceleration(player_->GetAcceleration());

	hook_->SetMinMoveLimit(field_->GetMinPosition());
	hook_->SetMaxMoveLimit(field_->GetMaxPosition());*/
	hook_->Initialize();

	// camera
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
	camera_->SetScale(cameraScale_);

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
}

///-------------------------------------------------------------
///					更新処理
/// -------------------------------------------------------------

void TuboScene::Update()
{
#pragma region SceneChange
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_) {
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
	}

	// シーン切り替え（必要なら）
	if (input_->TriggerKey(DIK_F2)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	if (input_->TriggerKey(DIK_F3)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene("SatouScene");
		}
	}
#pragma endregion

	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
	camera_->SetScale(cameraScale_);
	camera_->Update();

	field_->Update();

	// フックから移動情報を取得
	// ========================================================
	// ここは本来isGameStartがtrueの時のみ更新処理を行う
	//player_->SetPosition(hook_->GetPlayerPosition());
	//player_->SetVelocity(hook_->GetPlayerVelocity());
	//player_->SetAcceleration(hook_->GetPlayerAcceleration());
	//========================================================

	//player_->SetCamera(camera_);
	player_->SetMaxMoveLimit(field_->GetMaxPosition());
	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetIsGameStart(true);
	player_->Update();

	playerUI_->Update();

	// 武器の更新処理
	weapon_->Update();

	// フックの更新処理
	/*hook_->SetPlayerPosition(player_->GetPosition());
	hook_->SetPlayerRotation(player_->GetRotation());
	hook_->SetPlayerVelocity(player_->GetVelocity());
	hook_->SetPlayerAcceleration(player_->GetAcceleration());
	hook_->SetMinMoveLimit(field_->GetMinPosition());
	hook_->SetMaxMoveLimit(field_->GetMaxPosition());*/
	hook_->SetIsHitPlayerToEnemy(player_->GetIsHitEnemy());

	// フックの更新処理
	hook_->Update();

	enemy_->Update();

	CheckAllCollisions();
	collisionManager_->Update();
}

///-------------------------------------------------------------
///					描画処理
///-------------------------------------------------------------

void TuboScene::Draw()
{
	/// ------------------------------------------ ///
	/// ---------- スカイボックスの描画 ---------- ///
	/// ------------------------------------------ ///
	SkyBoxManager::GetInstance()->SetRenderSetting();

	/// ---------------------------------------- ///
	/// ----------  スプライトの描画  ---------- ///
	/// ---------------------------------------- ///
	// スプライトの共通描画設定
	SpriteManager::GetInstance()->SetRenderSetting();
	// プレイヤーUI
	playerUI_->Draw();

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
}


///-------------------------------------------------------------
///					終了処理
///-------------------------------------------------------------
void TuboScene::Finalize()
{

}


///-------------------------------------------------------------
///					ImGui描画処理
/// -------------------------------------------------------------
void TuboScene::DrawImGui()
{
	playerUI_->DrawImGui();

	enemy_->ShowImGui("Enemy");
	field_->ShowImGui("Field");
	//hook_->ShowImGui();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", &cameraPos_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("Rotation", &cameraRotate_.x, 0.1f, -10.0f, 10.0f);
	ImGui::End();
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
	collisionManager_->AddCollider(weapon_.get());
	collisionManager_->AddCollider(hook_.get());
	collisionManager_->AddCollider(enemy_.get());

	// 攻撃中の場合
	//if (weapon_->GetIsAttack()) {
	//	// 衝突判定と応答
	//	collisionManager_->CheckAllCollisions();
	//	if (enemy_->GetIsHit()) {
	//		enemy_->SetIsHitFromAttack(true);
	//	}
	//}

	// 複数について

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}
