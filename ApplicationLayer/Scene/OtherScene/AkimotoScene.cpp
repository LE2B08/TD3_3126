#include "AkimotoScene.h"
#include <DirectXCommon.h>
#include <ImGuiManager.h>
#include <Input.h>
#include <ParameterManager.h>
#include <ParticleManager.h>
#include "SceneManager.h"
#include "Object3DCommon.h"
#include "Wireframe.h"
#include <SkyBoxManager.h>
#include <SpriteManager.h>
#include "FadeManager.h"

void AkimotoScene::Initialize()
{
	FadeManager::GetInstance()->StartFadeOut();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	wavLoader_ = std::make_unique<WavLoader>();

	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetPosition({ 20.0f, 0.0f, -20.0f });
	player_->SetIsGameStart(true);

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get());

	field_ = std::make_unique<Field>();
	field_->Initialize();

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();

	dynamicCamera_ = std::make_unique<DynamicCamera>();
	dynamicCamera_->Initialize();
	dynamicCamera_->SetPlayer(player_.get());
	dynamicCamera_->SetEnemy(enemy_.get());

	Object3DCommon::GetInstance()->SetDefaultCamera(camera_);
}

void AkimotoScene::Update()
{
	// 入力によるシーン切り替え
	if (input_->TriggerKey(DIK_RETURN)) // Enterキーが押されたら
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("GamePlayScene"); // シーン名を指定して変更
		}
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
	if (input_->TriggerKey(DIK_F3))
	{
		if (sceneManager_)
		{
			sceneManager_->ChangeScene("SatouScene");
		}
	}

	// フィールドの更新
	field_->Update();

	player_->SetMinMoveLimit(field_->GetMinPosition());
	player_->SetMaxMoveLimit(field_->GetMaxPosition());

	// プレイヤーの更新
	player_->Update();

	enemy_->SetMinMoveLimit(field_->GetMinPosition());
	enemy_->SetMaxMoveLimit(field_->GetMaxPosition());

	// 敵の更新
	enemy_->Update();

	// ダイナミックカメラの更新
	dynamicCamera_->Update();

	// 計算したあとのカメラの値をセット
	camera_->SetScale(dynamicCamera_->GetScale());
	camera_->SetRotate(dynamicCamera_->GetRotate());
	camera_->SetTranslate(dynamicCamera_->GetTranslate());

	// カメラの更新
	camera_->Update();
}

void AkimotoScene::Draw() {
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

	// プレイヤーの描画
	player_->Draw();

	// 敵の描画
	enemy_->Draw();

	// フィールドの描画
	field_->Draw();

	// ラインの描画
	Wireframe::GetInstance()->DrawLine(player_->GetPosition(), enemy_->GetPosition(), { 1.0f, 0.0f, 0.0f, 1.0f });

	// 中心点の描画
	Wireframe::GetInstance()->DrawCircle((player_->GetPosition() + enemy_->GetPosition()) / 2.0f, 0.19f, 16, { 1.0f, 0.0f, 0.0f, 1.0f });
}

void AkimotoScene::Finalize()
{
}

void AkimotoScene::DrawImGui()
{
	enemy_->ShowImGui("Enemy");

	field_->ShowImGui("Field");

	dynamicCamera_->ShowImGui("DynamicCamera");
}
