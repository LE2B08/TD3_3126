#include "TutorialUI.h"
#include <TextureManager.h>
#include <Easing.h>
#include <algorithm>
#include <imgui.h>

TutorialUI::TutorialUI(){
	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Decision2.png");
	TextureManager::GetInstance()->LoadTexture("Resources/TutorialIntro.png");
	TextureManager::GetInstance()->LoadTexture("Resources/rightStickGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/OK.png");
	TextureManager::GetInstance()->LoadTexture("Resources/RBGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/ArcMovementGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/RTGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/LBGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/poseGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/playerHPGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/enemyHPGuide.png");
	TextureManager::GetInstance()->LoadTexture("Resources/TutorialEnd.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Skip.png");
	
	decisionUIPos_ = Vector2(650.0f, 620.0f); // 初期位置を設定
	tutorialIntroPos_ = Vector2(658.0f, 130.0f); // 初期位置を設定
	rightStickPos_ = Vector2(658.0f, 130.0f); // 初期位置を設定
    RBPos_ = Vector2(658.0f, 130.0f); // 初期位置を設定
	arcPos_ = Vector2(658.0f, 165.0f); // 初期位置を設定
	RTPos_ = Vector2(658.0f, 165.0f); // 初期位置を設定
	LBPos_ = Vector2(650.0f, 100.0f); // 初期位置を設定
	posePos_ = Vector2(650.0f, 100.0f); // 初期位置を設定
	successPos_ = Vector2(650.0f, 100.0f); // 初期位置を設定
	playerHPPos_ = Vector2(350.0f, 583.0f); // 初期位置を設定
	enemyHPPos_ = Vector2(650.0f, 115.0f); // 初期位置を設定
	tutorialEndPos_ = Vector2(650.0f, 165.0f); // 初期位置を設定
	skipUIPos_ = Vector2(950.0f, 640.0f); // 初期位置を設定

	decisionUIScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	tutorialIntroScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	rightStickScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	RBScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	arcScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	RTScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	LBScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	poseScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	successScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	playerHPScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	enemyHPScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
	tutorialEndScale_ = Vector2(0.0f, 0.0f); // 初期スケールを設定
}

void TutorialUI::Initialize() {
	// UIの初期化
	decisionUI_ = std::make_unique<Sprite>();
	decisionUI_->Initialize("Resources/Decision2.png");
	decisionUI_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// チュートリアルイントロの生成
	tutorialIntroUI_ = std::make_unique<Sprite>();
	tutorialIntroUI_->Initialize("Resources/TutorialIntro.png");
	tutorialIntroUI_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// 右スティックガイドの生成
	rightStickGuide_ = std::make_unique<Sprite>();
	rightStickGuide_->Initialize("Resources/rightStickGuide.png");
	rightStickGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// RBガイドの生成
	RBGuide_ = std::make_unique<Sprite>();
	RBGuide_->Initialize("Resources/RBGuide.png");
	RBGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// アーク移動ガイドの生成
	arcGuide_ = std::make_unique<Sprite>();
	arcGuide_->Initialize("Resources/ArcMovementGuide.png");
	arcGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// RTガイドの生成
	RTGuide_ = std::make_unique<Sprite>();
	RTGuide_->Initialize("Resources/RTGuide.png");
	RTGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// LBガイドの生成（必要に応じて追加）
	LBGuide_ = std::make_unique<Sprite>();
	LBGuide_->Initialize("Resources/LBGuide.png");
	LBGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// ポーズガイドの生成（必要に応じて追加）
	poseGuide_ = std::make_unique<Sprite>();
	poseGuide_->Initialize("Resources/poseGuide.png");
	poseGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// OKボタンの生成
	Success_ = std::make_unique<Sprite>();
	Success_->Initialize("Resources/OK.png");
	Success_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// プレイヤーHPガイドの生成
	playerHPGuide_ = std::make_unique<Sprite>();
	playerHPGuide_->Initialize("Resources/playerHPGuide.png");
	playerHPGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// 敵HPガイドの生成
	enemyHPGuide_ = std::make_unique<Sprite>();
	enemyHPGuide_->Initialize("Resources/enemyHPGuide.png");
	enemyHPGuide_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定
	// チュートリアル終了の生成
	tutorialEndUI_ = std::make_unique<Sprite>();
	tutorialEndUI_->Initialize("Resources/TutorialEnd.png");
	tutorialEndUI_->SetAnchorPoint(Vector2(0.5f, 0.5f)); // 中心をアンカーポイントに設定

	// スキップUIの生成
	skipUI_ = std::make_unique<Sprite>();
	skipUI_->Initialize("Resources/Skip.png");
}

void TutorialUI::Update(){
	// ここでUIの更新処理を行う
	decisionUI_->SetPosition(decisionUIPos_);
	decisionUI_->SetSize(decisionUIScale_);

	tutorialIntroUI_->SetPosition(tutorialIntroPos_);
	tutorialIntroUI_->SetSize(tutorialIntroScale_);

	rightStickGuide_->SetPosition(rightStickPos_);
	rightStickGuide_->SetSize(rightStickScale_);

	RBGuide_->SetPosition(RBPos_);
	RBGuide_->SetSize(RBScale_);

	arcGuide_->SetPosition(arcPos_);
	arcGuide_->SetSize(arcScale_);

	RTGuide_->SetPosition(RTPos_);
	RTGuide_->SetSize(RTScale_);

	LBGuide_->SetPosition(LBPos_);
	LBGuide_->SetSize(LBScale_);

	poseGuide_->SetPosition(posePos_);
	poseGuide_->SetSize(poseScale_);

	Success_->SetPosition(successPos_);
	Success_->SetSize(successScale_);

	playerHPGuide_->SetPosition(playerHPPos_);
	playerHPGuide_->SetSize(playerHPScale_);

	enemyHPGuide_->SetPosition(enemyHPPos_);
	enemyHPGuide_->SetSize(enemyHPScale_);

	tutorialEndUI_->SetPosition(tutorialEndPos_);
	tutorialEndUI_->SetSize(tutorialEndScale_);

	skipUI_->SetPosition(skipUIPos_);

	AnimationUpdate();
	decisionUI_->Update();
	tutorialIntroUI_->Update();
	rightStickGuide_->Update();
	RBGuide_->Update();
	arcGuide_->Update();
	RTGuide_->Update();
	LBGuide_->Update();
	poseGuide_->Update();
	Success_->Update();
	playerHPGuide_->Update();
	enemyHPGuide_->Update();
	tutorialEndUI_->Update();
	skipUI_->Update();
}

void TutorialUI::Draw(){
	// UIの描画処理を行う
	decisionUI_->Draw();
	// チュートリアルイントロの描画
	tutorialIntroUI_->Draw();
	// 右スティックガイドの描画
	rightStickGuide_->Draw();
	// RBガイドの描画
	RBGuide_->Draw();
	// アーク移動ガイドの描画
	arcGuide_->Draw();
	// RTガイドの描画
	RTGuide_->Draw();
	// LBガイドの描画
	LBGuide_->Draw();
	// ポーズガイドの描画
	poseGuide_->Draw();
	// OKボタンの描画
	Success_->Draw();
	// プレイヤーHPガイドの描画
	playerHPGuide_->Draw();
	// 敵HPガイドの描画
	enemyHPGuide_->Draw();
	// チュートリアル終了の描画
	tutorialEndUI_->Draw();
	// スキップUIの描画
	skipUI_->Draw();
}

void TutorialUI::DrawImGui(){
	ImGui::Begin("Tutorial UI Debug");
	ImGui::SliderFloat2("Decision UI Position", &decisionUIPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Right Stick Position", &rightStickPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::Text("%.02f", rightStickGuide_->GetSize().x);
	ImGui::Text("%.02f", rightStickGuide_->GetSize().y);
	ImGui::SliderFloat2("RB Position", &RBPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("arc Position", &arcPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("RT Position", &RTPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("LB Position", &LBPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Pose Position", &posePos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Success Position", &successPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Success Scale", &successScale_.x, 0.0f, 2.0f); // スケールの範囲を調整
	ImGui::SliderFloat2("Player HP Position", &playerHPPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Enemy HP Position", &enemyHPPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Tutorial End Position", &tutorialEndPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::SliderFloat2("Skip UI Position", &skipUIPos_.x, 0.0f, 800.0f); // 画面幅に合わせて調整
	ImGui::End();
}


void TutorialUI::AnimationUpdate()
{
	// 決定UIのスケールアニメーション
	if (isDecisionUIAppearing_) {
		decisionUIAnimTime_ += 1.0f / 60.0f;
		float t = min(decisionUIAnimTime_ / decisionUIAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		decisionUIScale_ = Vector2(200.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isDecisionUIAppearing_ = false;
		}
	} else if (isDecisionUIDisappearing_) {
		decisionUIAnimTime_ += 1.0f / 60.0f;
		float t = min(decisionUIAnimTime_ / decisionUIAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		decisionUIScale_ = Vector2(200.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isDecisionUIDisappearing_ = false;
		}
	}
	
	// チュートリアルイントロのスケールアニメーション
	if (isTutorialIntroAppearing_) {
		tutorialIntroAnimTime_ += 1.0f / 60.0f;
		float t = min(tutorialIntroAnimTime_ / tutorialIntroAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		tutorialIntroScale_ = Vector2(640.0f, 128.0f * scale);
		if (t >= 1.0f) {
			isTutorialIntroAppearing_ = false;
			DecisionUIAppear(); // チュートリアルイントロが表示されたら決定UIを表示
		}
	} else if (isTutorialIntroDisappearing_) {
		tutorialIntroAnimTime_ += 1.0f / 60.0f;
		float t = min(tutorialIntroAnimTime_ / tutorialIntroAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		tutorialIntroScale_ = Vector2(640.0f, 128.0f * scale);
		if (t >= 1.0f) {
			isTutorialIntroDisappearing_ = false;
		}
	}
	
    // rightStickGuideのスケールアニメーション
    if (isRightStickAppearing_) {
        rightStickAnimTime_ += 1.0f / 60.0f;
        float t = min(rightStickAnimTime_ / rightStickAnimDuration_, 1.0f);
        float scale = Easing::easeInOutElastic(t);
        rightStickScale_ = Vector2(720.0f, 128.0f * scale);
        if (t >= 1.0f) {
            isRightStickAppearing_ = false;
        }
    } else if (isRightStickDisappearing_) {
        rightStickAnimTime_ += 1.0f / 60.0f;
        float t = min(rightStickAnimTime_ / rightStickAnimDuration_, 1.0f);
        float scale = Easing::easeInOutElastic(1.0f - t);
        rightStickScale_ = Vector2(720.0f, 128.0f * scale);
        if (t >= 1.0f) {
            isRightStickDisappearing_ = false;
            // Successの出現アニメーションを開始
            successAppear();
        }
    }
	// RBGuideのスケールアニメーション
	if (isRBAppearing_) {
		RBAnimTime_ += 1.0f / 60.0f;
		float t = min(RBAnimTime_ / RBAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		RBScale_ = Vector2(800.0f, 128.0f * scale);
		if (t >= 1.0f) {
			isRBAppearing_ = false;
		}
	} else if (isRBDisappearing_) {
		RBAnimTime_ += 1.0f / 60.0f;
		float t = min(RBAnimTime_ / RBAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		RBScale_ = Vector2(800.0f, 128.0f * scale);
		if (t >= 1.0f) {
			isRBDisappearing_ = false;
			successAppear();
		}
	}
	// arcGuideのスケールアニメーション
	if (isArcAppearing_) {
		arcAnimTime_ += 1.0f / 60.0f;
		float t = min(arcAnimTime_ / arcAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		arcScale_ = Vector2(640.0f, 192.0f * scale);
		if (t >= 1.0f) {
			isArcAppearing_ = false;
		}
	} else if (isArcDisappearing_) {
		arcAnimTime_ += 1.0f / 60.0f;
		float t = min(arcAnimTime_ / arcAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		arcScale_ = Vector2(640.0f, 192.0f * scale);
		if (t >= 1.0f) {
			isArcDisappearing_ = false;
			successAppear();
		}
	}
	// RTGuideのスケールアニメーション
	if (isRTAppearing_) {
		RTAnimTime_ += 1.0f / 60.0f;
		float t = min(RTAnimTime_ / RTAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		RTScale_ = Vector2(540.0f, 192.0f * scale);
		if (t >= 1.0f) {
			isRTAppearing_ = false;
		}
	} else if (isRTDisappearing_) {
		RTAnimTime_ += 1.0f / 60.0f;
		float t = min(RTAnimTime_ / RTAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		RTScale_ = Vector2(540.0f, 192.0f * scale);
		if (t >= 1.0f) {
			isRTDisappearing_ = false;
			// Successの出現アニメーションを開始
			successAppear();
		}
	}
	
	// LBGuideのスケールアニメーション
	if (isLBAppearing_) {
		LBAnimTime_ += 1.0f / 60.0f;
		float t = min(LBAnimTime_ / LBAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		LBScale_ = Vector2(460.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isLBAppearing_ = false;
		}
	} else if (isLBDisappearing_) {
		LBAnimTime_ += 1.0f / 60.0f;
		float t = min(LBAnimTime_ / LBAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		LBScale_ = Vector2(460.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isLBDisappearing_ = false;
			// Successの出現アニメーションを開始
			successAppear();
		}
	}

	// poseGuideのスケールアニメーション
	if (isPoseAppearing_) {
		poseAnimTime_ += 1.0f / 60.0f;
		float t = min(poseAnimTime_ / poseAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		poseScale_ = Vector2(600.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isPoseAppearing_ = false;
		}
	} else if (isPoseDisappearing_) {
		poseAnimTime_ += 1.0f / 60.0f;
		float t = min(poseAnimTime_ / poseAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		poseScale_ = Vector2(600.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isPoseDisappearing_ = false;
			// Successの出現アニメーションを開始
			successAppear();
		}
	}
	
    // Successのスケールアニメーション
    if (isSuccessAppearing_) {
        successAnimTime_ += 1.0f / 60.0f;
        float t = min(successAnimTime_ / successAnimDuration_, 1.0f);
        float scale = Easing::easeInOutElastic(t);
        successScale_ = Vector2(120.0f, 64.0f * scale);
        if (t >= 1.0f) {
            isSuccessAppearing_ = false;
            // 3秒表示タイマー開始
			DecisionUIAppear(); // チュートリアルイントロが表示されたら決定UIを表示
        }
    } else if (isSuccessDisappearing_) {
        successAnimTime_ += 1.0f / 60.0f;
        float t = min(successAnimTime_ / successAnimDuration_, 1.0f);
        float scale = Easing::easeInOutElastic(1.0f - t);
        successScale_ = Vector2(120.0f, 64.0f * scale);
        if (t >= 1.0f) {
            isSuccessDisappearing_ = false;
        }
    }


	// プレイヤーHPガイドのスケールアニメーション
	if (isPlayerHPAppearing_) {
		playerHPAnimTime_ += 1.0f / 60.0f;
		float t = min(playerHPAnimTime_ / playerHPAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		playerHPScale_ = Vector2(400.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isPlayerHPAppearing_ = false;
		}
	} else if (isPlayerHPDisappearing_) {
		playerHPAnimTime_ += 1.0f / 60.0f;
		float t = min(playerHPAnimTime_ / playerHPAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		playerHPScale_ = Vector2(400.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isPlayerHPDisappearing_ = false;
		}
	}

	// 敵HPガイドのスケールアニメーション
	if(isEnemyHPAppearing_){
		enemyHPAnimTime_ += 1.0f / 60.0f;
		float t = min(enemyHPAnimTime_ / enemyHPAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		enemyHPScale_ = Vector2(220.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isEnemyHPAppearing_ = false;
		}
	} else if (isEnemyHPDisappearing_) {
		enemyHPAnimTime_ += 1.0f / 60.0f;
		float t = min(enemyHPAnimTime_ / enemyHPAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		enemyHPScale_ = Vector2(220.0f, 64.0f * scale);
		if (t >= 1.0f) {
			isEnemyHPDisappearing_ = false;
		}
	}

	// チュートリアル終了のスケールアニメーション
	if (isTutorialEndAppearing_) {
		tutorialEndAnimTime_ += 1.0f / 60.0f;
		float t = min(tutorialEndAnimTime_ / tutorialEndAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(t);
		tutorialEndScale_ = Vector2(740.0f, 256.0f * scale);
		if (t >= 1.0f) {
			isTutorialEndAppearing_ = false;
			DecisionUIAppear();
		}
	} else if (isTutorialEndDisappearing_) {
		tutorialEndAnimTime_ += 1.0f / 60.0f;
		float t = min(tutorialEndAnimTime_ / tutorialEndAnimDuration_, 1.0f);
		float scale = Easing::easeInOutElastic(1.0f - t);
		tutorialEndScale_ = Vector2(740.0f, 256.0f * scale);
		if (t >= 1.0f) {
			isTutorialEndDisappearing_ = false;
		}
	}
	
}

void TutorialUI::DecisionUIAppear()
{
	isDecisionUIAppearing_ = true;
	isDecisionUIDisappearing_ = false;
	decisionUIAnimTime_ = 0.0f;
}

void TutorialUI::DecisionUIDisappear()
{
	isDecisionUIDisappearing_ = true;
	isDecisionUIAppearing_ = false;
	decisionUIAnimTime_ = 0.0f;
}

void TutorialUI::TutorialIntroAppear()
{
	isTutorialIntroAppearing_ = true;
	isTutorialIntroDisappearing_ = false;
	tutorialIntroAnimTime_ = 0.0f;
}

void TutorialUI::TutorialIntroDisappear()
{
	isTutorialIntroDisappearing_ = true;
	isTutorialIntroAppearing_ = false;
	tutorialIntroAnimTime_ = 0.0f;
}

void TutorialUI::rightStickGuideAppear()
{
	isRightStickAppearing_ = true;
	isRightStickDisappearing_ = false;
	rightStickAnimTime_ = 0.0f;
}

void TutorialUI::rightStickGuideDisappear()
{
	isRightStickDisappearing_ = true;
	isRightStickAppearing_ = false;
	rightStickAnimTime_ = 0.0f;
}

void TutorialUI::RBGuideAppear()
{
	isRBAppearing_ = true;
	isRBDisappearing_ = false;
	RBAnimTime_ = 0.0f;
}

void TutorialUI::RBGuideDisappear()
{
	isRBDisappearing_ = true;
	isRBAppearing_ = false;
	RBAnimTime_ = 0.0f;
}

void TutorialUI::ArcGuideAppear()
{
	isArcAppearing_ = true;
	isArcDisappearing_ = false;
	arcAnimTime_ = 0.0f;
}

void TutorialUI::ArcGuideDisappear()
{
	isArcDisappearing_ = true;
	isArcAppearing_ = false;
	arcAnimTime_ = 0.0f;
}

void TutorialUI::successAppear()
{
	isSuccessAppearing_ = true;
	isSuccessDisappearing_ = false;
	successAnimTime_ = 0.0f;
}

void TutorialUI::successDisappear()
{
	isSuccessDisappearing_ = true;
	isSuccessAppearing_ = false;
	successAnimTime_ = 0.0f;
}

void TutorialUI::RTGuideAppear()
{
	// RTガイドの表示アニメーションを開始する処理を追加
	isRTAppearing_ = true;
	isRTDisappearing_ = false;
	RTAnimTime_ = 0.0f;
}

void TutorialUI::RTGuideDisappear()
{
	// RTガイドの非表示アニメーションを開始する処理を追加
	isRTDisappearing_ = true;
	isRTAppearing_ = false;
	RTAnimTime_ = 0.0f;
}

void TutorialUI::LBGuideAppear()
{
	// LBガイドの表示アニメーションを開始する処理を追加
	isLBAppearing_ = true;
	isLBDisappearing_ = false;
	LBAnimTime_ = 0.0f;
}

void TutorialUI::LBGuideDisappear()
{
	// LBガイドの非表示アニメーションを開始する処理を追加
	isLBDisappearing_ = true;
	isLBAppearing_ = false;
	LBAnimTime_ = 0.0f;
}

void TutorialUI::PoseGuideAppear()
{
	isPoseAppearing_ = true;
	isPoseDisappearing_ = false;
	poseAnimTime_ = 0.0f;
}

void TutorialUI::PoseGuideDisappear()
{
	isPoseDisappearing_ = true;
	isPoseAppearing_ = false;
	poseAnimTime_ = 0.0f;
}

void TutorialUI::PlayerHPGuideAppear()
{
	// プレイヤーHPガイドの表示アニメーションを開始する処理を追加
	isPlayerHPAppearing_ = true;
	isPlayerHPDisappearing_ = false;
	playerHPAnimTime_ = 0.0f;
}

void TutorialUI::PlayerHPGuideDisappear()
{
	// プレイヤーHPガイドの非表示アニメーションを開始する処理を追加
	isPlayerHPDisappearing_ = true;
	isPlayerHPAppearing_ = false;
	playerHPAnimTime_ = 0.0f;
}

void TutorialUI::EnemyHPGuideAppear()
{
	// 敵HPガイドの表示アニメーションを開始する処理を追加
	isEnemyHPAppearing_ = true;
	isEnemyHPDisappearing_ = false;
	enemyHPAnimTime_ = 0.0f;
}

void TutorialUI::EnemyHPGuideDisappear()
{
	isEnemyHPDisappearing_ = true;
	isEnemyHPAppearing_ = false;
	enemyHPAnimTime_ = 0.0f;
}

void TutorialUI::TutorialEndUIAppear()
{
	isTutorialEndAppearing_ = true;
	isTutorialEndDisappearing_ = false;
	tutorialEndAnimTime_ = 0.0f;
}

void TutorialUI::TutorialEndUIDisappear()
{
	isTutorialEndDisappearing_ = true;
	isTutorialEndAppearing_ = false;
	tutorialEndAnimTime_ = 0.0f;
}
