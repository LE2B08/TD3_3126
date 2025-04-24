#include "PauseMenu.h"
#include <TextureManager.h>
#include <Input.h>
#include <imgui.h>

PauseMenu::PauseMenu() {

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/BlackFilter.png");
	TextureManager::GetInstance()->LoadTexture("Resources/PauseText.png");
	TextureManager::GetInstance()->LoadTexture("Resources/ReturnToGameText.png");
	TextureManager::GetInstance()->LoadTexture("Resources/HowToPlayText.png");
	TextureManager::GetInstance()->LoadTexture("Resources/ReturnToTitleText.png");
	TextureManager::GetInstance()->LoadTexture("Resources/SelectionArrow.png");
	TextureManager::GetInstance()->LoadTexture("Resources/HowToPlay.png");

	// テキストの位置
	pauseTextPos_ = { 488.0f, 60.0f };
	returnToGameTextPos_ = { 484.0f, 360.0f };
	howToPlayTextPos_ = { 516.0f, 488.0f };
	returnToTitleTextPos_ = { 468.0f, 616.0f };

	// 選択している矢印の位置
	selectionArrowPos_ = { 400.0f, 380.0f };

	// 操作説明の位置
	HowToPlayPos_ = { 0.0f, 0.0f };
}

void PauseMenu::Initialize() {

	// 入力の取得
	input_ = Input::GetInstance();

	// 黒のフィルターの生成&初期化
	blackFilter_ = std::make_unique<Sprite>();
	blackFilter_->Initialize("Resources/BlackFilter.png");
	blackFilter_->SetColor({ 0.0f, 0.0f, 0.0f, 0.75f }); // 黒色

	// テキストの生成&初期化
	pauseText_ = std::make_unique<Sprite>();
	pauseText_->Initialize("Resources/PauseText.png");
	pauseText_->SetPosition(pauseTextPos_);

	returnToGameText_ = std::make_unique<Sprite>();
	returnToGameText_->Initialize("Resources/ReturnToGameText.png");
	returnToGameText_->SetPosition(returnToGameTextPos_);

	howToPlayText_ = std::make_unique<Sprite>();
	howToPlayText_->Initialize("Resources/HowToPlayText.png");
	howToPlayText_->SetPosition(howToPlayTextPos_);

	returnToTitleText_ = std::make_unique<Sprite>();
	returnToTitleText_->Initialize("Resources/ReturnToTitleText.png");
	returnToTitleText_->SetPosition(returnToTitleTextPos_);

	// 選択している矢印の生成&初期化
	selectionArrow_ = std::make_unique<Sprite>();
	selectionArrow_->Initialize("Resources/SelectionArrow.png");
	selectionArrow_->SetPosition(selectionArrowPos_);

	// コントローラーの画像の生成&初期化
	HowToPlay = std::make_unique<Sprite>();
	HowToPlay->Initialize("Resources/HowToPlay.png");
	HowToPlay->SetPosition(HowToPlayPos_);
}

void PauseMenu::Update() {

	// 状態ごとの処理
	switch (menuState_) {

	case MenuState::ReturnToGame:

		// 下矢印キーが押されたら
		if (input_->TriggerKey(DIK_DOWN) || input_->TriggerButton(5)) {
			// 選択しているメニューを変更
			menuState_ = MenuState::HowToPlay;
			selectionArrowPos_.y += 128.0f; // 矢印の位置を下に移動
		}

		break;

	case MenuState::HowToPlay:

		// 操作方法の説明を表示していたら
		if (isHowToPlay_) {

			// エンターキーかAボタンが押されたら
			if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {
				// 遊び方の説明を非表示にする
				isHowToPlay_ = false;
			}
		}
		// 操作方法の説明を表示していなかったら
		else {

			// 上矢印キーが押されたら
			if (input_->TriggerKey(DIK_UP) || input_->TriggerButton(4)) {
				// 選択しているメニューを変更
				menuState_ = MenuState::ReturnToGame;
				selectionArrowPos_.y -= 128.0f; // 矢印の位置を上に移動
			}

			// 下矢印キーが押されたら
			if (input_->TriggerKey(DIK_DOWN) || input_->TriggerButton(5)) {
				// 選択しているメニューを変更
				menuState_ = MenuState::ReturnToTitle;
				selectionArrowPos_.y += 128.0f; // 矢印の位置を下に移動
			}

			// エンターキーかAボタンが押されたら
			if (input_->TriggerKey(DIK_RETURN) || input_->TriggerButton(0)) {
				// 遊び方の説明を表示する
				isHowToPlay_ = true;
			}
		}

		break;

	case MenuState::ReturnToTitle:

		// 上矢印キーが押されたら
		if (input_->TriggerKey(DIK_UP) || input_->TriggerButton(4)) {
			// 選択しているメニューを変更
			menuState_ = MenuState::HowToPlay;
			selectionArrowPos_.y -= 128.0f; // 矢印の位置を上に移動
		}

		break;

	default:
		break;
	}

	// フィルターの更新
	blackFilter_->Update();

	// テキストの更新
	pauseText_->Update();

	returnToGameText_->Update();

	howToPlayText_->Update();

	returnToTitleText_->Update();

	// 選択している矢印の更新
	selectionArrow_->SetPosition(selectionArrowPos_);
	selectionArrow_->Update();

	// コントローラーの画像の更新
	HowToPlay->Update();
}

void PauseMenu::Draw() {

	// フィルターの描画
	blackFilter_->Draw();

	// 操作方法の説明を表示していたら
	if (isHowToPlay_) {
		// コントローラーの画像の描画
		HowToPlay->Draw();
	}
	else {

		// テキストの描画
		pauseText_->Draw();

		returnToGameText_->Draw();

		howToPlayText_->Draw();

		returnToTitleText_->Draw();

		// 選択している矢印の描画
		selectionArrow_->Draw();
	}
}

void PauseMenu::ShowImGui() {

	ImGui::Begin("PauseMenu");

	// 選択しているメニューの状態を表示
	switch (menuState_) {
	case MenuState::ReturnToGame:
		ImGui::Text("Return to Game");
		break;
	case MenuState::HowToPlay:
		ImGui::Text("How to Play");
		break;
	case MenuState::ReturnToTitle:
		ImGui::Text("Return to Title");
		break;
	default:
		break;
	}

	ImGui::End();
}