#pragma once
#include <Sprite.h>
class TutorialUI
{
public:
	/*------メンバ関数------*/
	TutorialUI();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// ImGuiの描画
	void DrawImGui();

	void AnimationUpdate();

	// 決定UI出現
	void DecisionUIAppear();

	// 決定UI消失
	void DecisionUIDisappear();

	// チュートリアル開始時のUI出現
	void TutorialIntroAppear();

	// チュートリアル開始時のUI消失
	void TutorialIntroDisappear();

	// テキスト出現
	void rightStickGuideAppear();

	// テキスト消失
	void rightStickGuideDisappear();

	void RBGuideAppear();

	void RBGuideDisappear();

	void ArcGuideAppear();

	void ArcGuideDisappear();

	void successAppear();

	void successDisappear();

	void RTGuideAppear();

	void RTGuideDisappear();

	void LBGuideAppear();

	void LBGuideDisappear();

	void PoseGuideAppear();

	void PoseGuideDisappear();

	void PlayerHPGuideAppear();
	void PlayerHPGuideDisappear();

	void EnemyHPGuideAppear();
	void EnemyHPGuideDisappear();

	void TutorialEndUIAppear();

	void TutorialEndUIDisappear();
	
private:
	/*------メンバ変数------*/
	std::unique_ptr<Sprite> decisionUI_; // 決定UIのスプライト
	std::unique_ptr<Sprite> tutorialIntroUI_;
	std::unique_ptr<Sprite> rightStickGuide_; // 右スティックガイドのスプライト
	std::unique_ptr<Sprite> RBGuide_; // LBガイドのスプライト
	std::unique_ptr<Sprite> arcGuide_; // アークガイドのスプライト
	std::unique_ptr<Sprite> RTGuide_; // RTガイドのスプライト
	std::unique_ptr<Sprite> LBGuide_; // LBガイドのスプライト
	std::unique_ptr<Sprite> poseGuide_; // ポーズガイドのスプライト
	std::unique_ptr<Sprite> Success_; // 成功時のスプライト
	std::unique_ptr<Sprite> playerHPGuide_; // プレイヤーHPガイドのスプライト
	std::unique_ptr<Sprite> enemyHPGuide_; // 敵HPガイドのスプライト
	std::unique_ptr<Sprite> tutorialEndUI_; // 右スティックのスプライト

	/*------位置と拡大率の変数------*/
	Vector2 decisionUIPos_ = { 0.0f, 0.0f }; // 決定UIの位置
	Vector2 decisionUIScale_ = { 1.0f, 1.0f }; // 決定UIの拡大率

	Vector2 tutorialIntroPos_ = { 0.0f, 0.0f }; // チュートリアルイントロの位置
	Vector2 tutorialIntroScale_ = { 1.0f, 1.0f }; // チュートリアルイントロの拡大率

	Vector2 rightStickPos_ = { 0.0f, 0.0f }; // 右スティックの位置
	Vector2 rightStickScale_ = { 1.0f, 1.0f }; // 右スティックの拡大率

	Vector2 RBPos_ = { 0.0f, 0.0f }; // LBの位置
	Vector2 RBScale_ = { 1.0f, 1.0f }; // LBの拡大率

	Vector2 arcPos_ = { 0.0f, 0.0f }; // アークの位置
	Vector2 arcScale_ = { 1.0f, 1.0f }; // アークの拡大率

	Vector2 RTPos_ = { 0.0f, 0.0f }; // RTの位置
	Vector2 RTScale_ = { 1.0f, 1.0f }; // RTの拡大率

	Vector2 LBPos_ = { 0.0f, 0.0f }; // LBの位置
	Vector2 LBScale_ = { 1.0f, 1.0f }; // LBの拡大率

	Vector2 posePos_ = { 0.0f, 0.0f }; // ポーズの位置
	Vector2 poseScale_ = { 1.0f, 1.0f }; // ポーズの拡大率

	Vector2 successPos_ = { 0.0f, 0.0f }; // 成功時のスプライトの位置
	Vector2 successScale_ = { 1.0f, 1.0f }; // 成功時のスプライトの拡大率

	Vector2 playerHPPos_ = { 0.0f, 0.0f }; // プレイヤーHPガイドの位置
	Vector2 playerHPScale_ = { 1.0f, 1.0f }; // プレイヤーHPガイドの拡大率

	Vector2 enemyHPPos_ = { 0.0f, 0.0f }; // 敵HPガイドの位置
	Vector2 enemyHPScale_ = { 1.0f, 1.0f }; // 敵HPガイドの拡大率

	Vector2 tutorialEndPos_ = { 0.0f, 0.0f }; // チュートリアル終了時のUIの位置
	Vector2 tutorialEndScale_ = { 1.0f, 1.0f }; // チュートリアル終了時のUIの拡大率

	/*------アニメーションの変数------*/
	float decisionUIAnimTime_ = 0.0f; // 決定UIのアニメーション時間
	bool isDecisionUIAppearing_ = false; // 決定UIのアニメーション状態
	bool isDecisionUIDisappearing_ = false; // 決定UIのアニメーション状態
	const float decisionUIAnimDuration_ = 1.0f; // 決定UIのアニメーションの持続時間

	// チュートリアル開始時のUIの表示タイマー
	float tutorialIntroAnimTime_ = 0.0f;
	// チュートリアル開始時のUIのアニメーション状態
	bool isTutorialIntroAppearing_ = false;
	bool isTutorialIntroDisappearing_ = false;
	// チュートリアル開始時のUIのアニメーションの持続時間
	const float tutorialIntroAnimDuration_ = 1.0f; // 1秒

	// アニメーションの時間管理
	float rightStickAnimTime_ = 0.0f;
	// 右スティックガイドのアニメーション状態
	bool isRightStickAppearing_ = false;
	bool isRightStickDisappearing_ = false;
	// 右スティックガイドのアニメーションの持続時間
	const float rightStickAnimDuration_ = 1.0f; // 1秒

	// LBの表示タイマー
	float RBAnimTime_ = 0.0f;
	// LBのアニメーション状態
	bool isRBAppearing_ = false;
	bool isRBDisappearing_ = false;
	// LBのアニメーションの持続時間
	const float RBAnimDuration_ = 1.0f; // 1秒

	// アークの表示タイマー
	float arcAnimTime_ = 0.0f;
	// アークのアニメーション状態
	bool isArcAppearing_ = false;
	bool isArcDisappearing_ = false;
	// アークのアニメーションの持続時間
	const float arcAnimDuration_ = 1.0f; // 1秒

	// RTの表示タイマー
	float RTAnimTime_ = 0.0f;
	// RTのアニメーション状態
	bool isRTAppearing_ = false;
	bool isRTDisappearing_ = false;
	// RTのアニメーションの持続時間
	const float RTAnimDuration_ = 1.0f; // 1秒

	// LBの表示タイマー
	float LBAnimTime_ = 0.0f;
	// LBのアニメーション状態
	bool isLBAppearing_ = false;
	bool isLBDisappearing_ = false;
	// LBのアニメーションの持続時間
	const float LBAnimDuration_ = 1.0f; // 1秒

	// ポーズの表示タイマー
	float poseAnimTime_ = 0.0f;
	// ポーズのアニメーション状態
	bool isPoseAppearing_ = false;
	bool isPoseDisappearing_ = false;
	// ポーズのアニメーションの持続時間
	const float poseAnimDuration_ = 1.0f; // 1秒

	// 成功時の表示タイマー
	float successAnimTime_ = 0.0f;
	// 成功時のアニメーション状態
	bool isSuccessAppearing_ = false;
	bool isSuccessDisappearing_ = false;
	// 成功時のアニメーションの持続時間
	const float successAnimDuration_ = 0.5f; // 0.5秒

	// プレイヤーHPガイドの表示タイマー
	float playerHPAnimTime_ = 0.0f;
	// プレイヤーHPガイドのアニメーション状態
	bool isPlayerHPAppearing_ = false;
	bool isPlayerHPDisappearing_ = false;
	// プレイヤーHPガイドのアニメーションの持続時間
	const float playerHPAnimDuration_ = 1.0f; // 1秒

	// 敵HPガイドの表示タイマー
	float enemyHPAnimTime_ = 0.0f;
	// 敵HPガイドのアニメーション状態
	bool isEnemyHPAppearing_ = false;
	bool isEnemyHPDisappearing_ = false;
	// 敵HPガイドのアニメーションの持続時間
	const float enemyHPAnimDuration_ = 1.0f; // 1秒

	// チュートリアル終了時のUIの表示タイマー
	float tutorialEndAnimTime_ = 0.0f;
	// チュートリアル終了時のUIのアニメーション状態
	bool isTutorialEndAppearing_ = false;
	bool isTutorialEndDisappearing_ = false;
	// チュートリアル終了時のUIのアニメーションの持続時間
	const float tutorialEndAnimDuration_ = 1.0f; // 1秒

};

