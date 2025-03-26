#include "TuboScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"
#include "FadeManager.h"


#include "Object3DCommon.h"
#include <SpriteManager.h>
#include <SkyBoxManager.h>

/// -------------------------------------------------------------
///				　			　初期化処理
/// -------------------------------------------------------------
void TuboScene::Initialize()
{
	FadeManager::GetInstance()->StartFadeOut();

	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	// Playerクラスの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();

	//camera
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
	camera_->SetScale(cameraScale_);

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get());

	field_ = std::make_unique<Field>();
	field_->Initialize();

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
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
	}

	// シーン切り替え（必要なら）
	if (input_->TriggerKey(DIK_F2))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("AkimotoScene");
		}
	}

	if (input_->TriggerKey(DIK_F3))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("SatouScene");
		}
	}
#pragma endregion

	
	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
	camera_->SetScale(cameraScale_);
	camera_->Update();

	field_->Update();


	player_->SetCamera(camera_);
	player_->SetMaxMoveLimit(field_->GetMaxPosition());
	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetIsGameStart(true);
	player_->Update();

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



	/// ---------------------------------------- ///
	/// ---------- オブジェクト3D描画 ---------- ///
	/// ---------------------------------------- ///
	// オブジェクト3D共通描画設定
	Object3DCommon::GetInstance()->SetRenderSetting();

	player_->Draw();

	enemy_->Draw();

	field_->Draw();

	collisionManager_->Draw();

}

///-------------------------------------------------------------
///					終了処理
///-------------------------------------------------------------

void TuboScene::Finalize()
{ 
	player_->Finalize(); 
}

///-------------------------------------------------------------
///					ImGui描画処理
/// -------------------------------------------------------------

void TuboScene::DrawImGui()
{ 
	player_->DrawImGui(); 
	enemy_->ShowImGui("Enemy");
	field_->ShowImGui("Field");

	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", &cameraPos_.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat3("Rotation", &cameraRotate_.x, 0.1f, -10.0f, 10.0f);
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
	collisionManager_->AddCollider(player_->GetWeapon());
	collisionManager_->AddCollider(player_->GetHook());
	collisionManager_->AddCollider(enemy_.get());


	// 複数について

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}
