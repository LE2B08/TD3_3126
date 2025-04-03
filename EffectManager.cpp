#include "EffectManager.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "Field.h"
#include <random>
#include <Easing.h>
using namespace Easing;


/// -------------------------------------------------------------
///					　		初期化処理
/// -------------------------------------------------------------
void EffectManager::Initialize(Camera* camera, Input* input, Player* player, Field* field)
{
	// 各クラスのポインタを受け取る
	camera_ = camera;
	input_ = input;
	player_ = player;
	field_ = field;

	// カメラの初期位置を取得
	cameraPosition_ = camera_->GetTranslate();
}


/// -------------------------------------------------------------
///					　		更新処理
/// -------------------------------------------------------------
void EffectManager::Update()
{
	// カメラの揺れ
	CameraShake();
}


/// -------------------------------------------------------------
///					　		ゲーム開始演出を開始
/// -------------------------------------------------------------
void EffectManager::StartGameEffect()
{
	if (!isStartEasing_)
	{
		startTimer_ = 0.0f;
		playerStartTimer_ = 0.0f;
		isStartEasing_ = true;
	}
}


/// -------------------------------------------------------------
///					　		カメラの揺れ
/// -------------------------------------------------------------
void EffectManager::CameraShake()
{
	// カメラの揺れを更新
	if (isCameraShaking_)
	{
		shakeElapsedTime_ += 1.0f / 60.0f;

		if (shakeElapsedTime_ >= shakeDuration_)
		{
			input_->StopVibration();
			isCameraShaking_ = false;
			camera_->SetTranslate(cameraPosition_);
			shakeElapsedTime_ = 0.0f;
		}
		else
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(-shakeMagnitude_, shakeMagnitude_);
			Vector3 shakeOffset = { dis(gen), dis(gen), dis(gen) };
			camera_->SetTranslate(cameraPosition_ + shakeOffset);
			input_->SetVibration(100, 100);
		}
	}
}


/// -------------------------------------------------------------
///					　		ゲーム開始演出
/// -------------------------------------------------------------
void EffectManager::GameStart()
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
