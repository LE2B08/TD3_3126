#include "PauseMenu.h"
#include <TextureManager.h>

void PauseMenu::Initialize() {

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/BlackFilter.png");

	// 黒のフィルターの生成&初期化
	blackFilter_ = std::make_unique<Sprite>();
	blackFilter_->Initialize("Resources/BlackFilter.png");
	blackFilter_->SetColor({ 0.0f, 0.0f, 0.0f, 0.75f }); // 黒色
}

void PauseMenu::Update() {

	// フィルターの更新
	blackFilter_->Update();
}

void PauseMenu::Draw() {

	// フィルターの描画
	blackFilter_->Draw();
}
