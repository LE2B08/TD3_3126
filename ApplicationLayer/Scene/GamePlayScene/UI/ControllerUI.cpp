#include "ControllerUI.h"
#include "TextureManager.h"
#include <imgui.h>


/// -------------------------------------------------------------
///						　	初期化処理
/// -------------------------------------------------------------
ControllerUI::ControllerUI()
{
	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/xbox_rb.png");
	TextureManager::GetInstance()->LoadTexture("Resources/xbox_lb.png");
	TextureManager::GetInstance()->LoadTexture("Resources/xbox_rt.png");
	TextureManager::GetInstance()->LoadTexture("Resources/xbox_rs.png");
	TextureManager::GetInstance()->LoadTexture("Resources/xbox_button_a.png");

	TextureManager::GetInstance()->LoadTexture("Resources/throw_ui.png");
	TextureManager::GetInstance()->LoadTexture("Resources/back_ui.png");
	TextureManager::GetInstance()->LoadTexture("Resources/wind_ui.png");
	TextureManager::GetInstance()->LoadTexture("Resources/attack_ui.png");
	TextureManager::GetInstance()->LoadTexture("Resources/direction_ui.png");
	TextureManager::GetInstance()->LoadTexture("Resources/game_start_ui.png");

	TextureManager::GetInstance()->LoadTexture("Resources/SelectPause.png");

	// 位置の初期化
	RButtonPosition_ = { 1000.0f, 0.0f };
	LButtonPosition_ = { 1000.0f, 60.0f };
	RTriggerPosition_ = { 1000.0f, 120.0f };
	RStickPosition_ = { 1000.0f, 180.0f };
	AButtonPosition_ = { 1000.0f, 240.0f };

	throwPosition_ = { 1064.0f, 0.0f };
	backPosition_ = { 1064.0f, 0.0f };
	windPosition_ = { 1064.0f, 120.0f };
	attackPosition_ = { 1064.0f, 60.0f };
	directionPosition_ = { 1064.0f, 180.0f };
	gameStartPosition_ = { 1064.0f, 240.0f };

	pauseButtonPosition_ = { 950.0f, 32.0f };
}


/// -------------------------------------------------------------
///						　	初期化処理
/// -------------------------------------------------------------
void ControllerUI::Initialize()
{
	// Rボタンの生成&初期化
	RButton_ = std::make_unique<Sprite>();
	RButton_->Initialize("Resources/xbox_rb.png");

	// Lボタンの生成&初期化
	LButton_ = std::make_unique<Sprite>();
	LButton_->Initialize("Resources/xbox_lb.png");

	// Rトリガーの生成&初期化
	RTrigger_ = std::make_unique<Sprite>();
	RTrigger_->Initialize("Resources/xbox_rt.png");

	// Rスティックの生成&初期化
	RStick_ = std::make_unique<Sprite>();
	RStick_->Initialize("Resources/xbox_rs.png");

	// Aボタンの生成&初期化
	AButton_ = std::make_unique<Sprite>();
	AButton_->Initialize("Resources/xbox_button_a.png");

	// 投げるの生成&初期化
	throw_ = std::make_unique<Sprite>();
	throw_->Initialize("Resources/throw_ui.png");

	// 戻すの生成&初期化
	back_ = std::make_unique<Sprite>();
	back_->Initialize("Resources/back_ui.png");

	// 巻くの生成&初期化
	wind_ = std::make_unique<Sprite>();
	wind_->Initialize("Resources/wind_ui.png");

	// 攻撃の生成&初期化
	attack_ = std::make_unique<Sprite>();
	attack_->Initialize("Resources/attack_ui.png");

	// 向きの生成&初期化
	direction_ = std::make_unique<Sprite>();
	direction_->Initialize("Resources/direction_ui.png");

	// ゲームスタートの生成&初期化
	gameStart_ = std::make_unique<Sprite>();
	gameStart_->Initialize("Resources/game_start_ui.png");

	// ポーズボタンの生成&初期化
	pauseButton_ = std::make_unique<Sprite>();
	pauseButton_->Initialize("Resources/SelectPause.png");
}


/// -------------------------------------------------------------
///						　	更新処理
/// -------------------------------------------------------------
void ControllerUI::Update()
{

	// Rボタンの更新
	RButton_->SetPosition(RButtonPosition_);
	RButton_->Update();

	// Lボタンの更新
	LButton_->SetPosition(LButtonPosition_);
	LButton_->Update();

	// Rトリガーの更新
	RTrigger_->SetPosition(RTriggerPosition_);
	RTrigger_->Update();

	// Rスティックの更新
	RStick_->SetPosition(RStickPosition_);
	RStick_->Update();

	// Aボタンの更新
	AButton_->SetPosition(AButtonPosition_);
	AButton_->Update();

	// 投げるの更新
	throw_->SetPosition(throwPosition_);
	throw_->Update();

	// 戻すの更新
	back_->SetPosition(backPosition_);
	back_->Update();

	// 巻くの更新
	wind_->SetPosition(windPosition_);
	wind_->Update();

	// 攻撃の更新
	attack_->SetPosition(attackPosition_);
	attack_->Update();

	// 向きの更新
	direction_->SetPosition(directionPosition_);
	direction_->Update();

	// ゲームスタートの更新
	gameStart_->SetPosition(gameStartPosition_);
	gameStart_->Update();

	// ポーズボタンの更新
	pauseButton_->SetPosition(pauseButtonPosition_);
	pauseButton_->Update();
}


/// -------------------------------------------------------------
///						　	描画処理
/// -------------------------------------------------------------
void ControllerUI::Draw()
{

	// Rボタンの描画
	//RButton_->Draw();

	// Lボタンの描画
	//LButton_->Draw();

	// Rトリガーの描画
	//RTrigger_->Draw();

	// Rスティックの描画
	//RStick_->Draw();

	// Aボタンの描画
	//AButton_->Draw();

	// 投げるの描画
	//throw_->Draw();

	// 戻すの描画
	//back_->Draw();

	// 巻くの描画
	//wind_->Draw();

	// 攻撃の描画
	//attack_->Draw();

	// 向きの描画
	//direction_->Draw();

	// ゲームスタートの描画
	//gameStart_->Draw();

	// ポーズボタンの描画
	pauseButton_->Draw();
}


/// -------------------------------------------------------------
///						　	ImGui描画処理
/// -------------------------------------------------------------
void ControllerUI::DrawImGui()
{

	ImGui::Begin("ControllerUI");
	
	ImGui::DragFloat2("RButton Position", &RButtonPosition_.x, 1.0f);
	ImGui::DragFloat2("LButton Position", &LButtonPosition_.x, 1.0f);
	ImGui::DragFloat2("RTrigger Position", &RTriggerPosition_.x, 1.0f);
	ImGui::DragFloat2("RStick Position", &RStickPosition_.x, 1.0f);
	ImGui::DragFloat2("AButton Position", &AButtonPosition_.x, 1.0f);

	ImGui::DragFloat2("Throw Position", &throwPosition_.x, 1.0f);
	ImGui::DragFloat2("Back Position", &backPosition_.x, 1.0f);
	ImGui::DragFloat2("Wind Position", &windPosition_.x, 1.0f);
	ImGui::DragFloat2("Attack Position", &attackPosition_.x, 1.0f);
	ImGui::DragFloat2("Direction Position", &directionPosition_.x, 1.0f);
	ImGui::DragFloat2("GameStart Position", &gameStartPosition_.x, 1.0f);

	ImGui::End();
}
