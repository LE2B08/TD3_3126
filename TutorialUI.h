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

	// テキスト出現
	void rightStickGuideAppear();

	// テキスト消失
	void rightStickGuideDisappear();

	void LBGuideAppear();

	void LBGuideDisappear();

	void ArcGuideAppear();

	void ArcGuideDisappear();

	void successAppear();

	void successDisappear();

	void LTGuideAppear();

	void LTGuideDisappear();

	void RTGuideAppear();

	void RTGuideDisappear();

	void PoseGuideAppear();

	void PoseGuideDisappear();

	void PlayerHPGuideAppear();
	void PlayerHPGuideDisappear();

	void EnemyHPGuideAppear();
	void EnemyHPGuideDisappear();
	
private:
	/*------メンバ変数------*/
	std::unique_ptr<Sprite> rightStickGuide_; // 右スティックガイドのスプライト
	std::unique_ptr<Sprite> LBGuide_; // LBガイドのスプライト
	std::unique_ptr<Sprite> arcGuide_; // アークガイドのスプライト
	std::unique_ptr<Sprite> LTGuide_; // LTガイドのスプライト
	std::unique_ptr<Sprite> RTGuide_; // RTガイドのスプライト
	std::unique_ptr<Sprite> poseGuide_; // ポーズガイドのスプライト
	std::unique_ptr<Sprite> Success_; // 成功時のスプライト
	std::unique_ptr<Sprite> playerHPGuide_; // プレイヤーHPガイドのスプライト
	std::unique_ptr<Sprite> enemyHPGuide_; // 敵HPガイドのスプライト

	Vector2 rightStickPos_ = { 0.0f, 0.0f }; // 右スティックの位置
	Vector2 rightStickScale_ = { 1.0f, 1.0f }; // 右スティックの拡大率

	Vector2 LBPos_ = { 0.0f, 0.0f }; // LBの位置
	Vector2 LBScale_ = { 1.0f, 1.0f }; // LBの拡大率

	Vector2 arcPos_ = { 0.0f, 0.0f }; // アークの位置
	Vector2 arcScale_ = { 1.0f, 1.0f }; // アークの拡大率

	Vector2 LTPos_ = { 0.0f, 0.0f }; // LTの位置
	Vector2 LTScale_ = { 1.0f, 1.0f }; // LTの拡大率

	Vector2 RTPos_ = { 0.0f, 0.0f }; // RTの位置
	Vector2 RTScale_ = { 1.0f, 1.0f }; // RTの拡大率

	Vector2 posePos_ = { 0.0f, 0.0f }; // ポーズの位置
	Vector2 poseScale_ = { 1.0f, 1.0f }; // ポーズの拡大率

	Vector2 successPos_ = { 0.0f, 0.0f }; // 成功時のスプライトの位置
	Vector2 successScale_ = { 1.0f, 1.0f }; // 成功時のスプライトの拡大率

	Vector2 playerHPPos_ = { 0.0f, 0.0f }; // プレイヤーHPガイドの位置
	Vector2 playerHPScale_ = { 1.0f, 1.0f }; // プレイヤーHPガイドの拡大率

	Vector2 enemyHPPos_ = { 0.0f, 0.0f }; // 敵HPガイドの位置
	Vector2 enemyHPScale_ = { 1.0f, 1.0f }; // 敵HPガイドの拡大率

	/*------アニメーションの変数------*/
	// アニメーションの時間管理
	float rightStickAnimTime_ = 0.0f;
	// 右スティックガイドのアニメーション状態
	bool isRightStickAppearing_ = false;
	bool isRightStickDisappearing_ = false;
	// 右スティックガイドのアニメーションの持続時間
	const float rightStickAnimDuration_ = 1.0f; // 1秒

	// LBの表示タイマー
	float LBAnimTime_ = 0.0f;
	// LBのアニメーション状態
	bool isLBAppearing_ = false;
	bool isLBDisappearing_ = false;
	// LBのアニメーションの持続時間
	const float LBAnimDuration_ = 1.0f; // 1秒

	// アークの表示タイマー
	float arcAnimTime_ = 0.0f;
	// アークのアニメーション状態
	bool isArcAppearing_ = false;
	bool isArcDisappearing_ = false;
	// アークのアニメーションの持続時間
	const float arcAnimDuration_ = 1.0f; // 1秒

	// LTの表示タイマー
	float LTAnimTime_ = 0.0f;
	// LTのアニメーション状態
	bool isLTAppearing_ = false;
	bool isLTDisappearing_ = false;
	// LTのアニメーションの持続時間
	const float LTAnimDuration_ = 1.0f; // 1秒

	// RTの表示タイマー
	float RTAnimTime_ = 0.0f;
	// RTのアニメーション状態
	bool isRTAppearing_ = false;
	bool isRTDisappearing_ = false;
	// RTのアニメーションの持続時間
	const float RTAnimDuration_ = 1.0f; // 1秒

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
	const float successAnimDuration_ = 1.0f; // 1秒


	float successDisplayTimer_ = 0.0f;
	const float successDisplayDuration_ = 1.0f;
	bool isSuccessDisplayWaiting_ = false;

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

};

