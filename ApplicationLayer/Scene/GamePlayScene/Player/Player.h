#pragma once
#include "BasePlayer.h"
#include <optional>
#include "ContactRecord.h"
#include "PlayerUI.h" // PlayerUIのインクルードを追加

/// -------------------------------------------------------------
/// プレイヤー用調整パラメータ構造体
/// -------------------------------------------------------------
struct PlayerParams {
    static constexpr float kMoveSpeed = 0.25f;      // 移動速度
    static constexpr float kJumpPower = 0.6f;      // ジャンプ力
    static constexpr float kGravity = 0.03f;       // 重力加速度
    static constexpr float kAttackPower = 1.0f;    // 攻撃力
    static constexpr float kInvincibleDuration = 60.0f; // 無敵時間（フレーム）
    static constexpr float kAttackDuration = 30.0f;     // 攻撃持続時間（フレーム）
    static constexpr float kMaxHp = 10.0f;         // 最大HP
    // 必要に応じて他のパラメータも追加
};

/// -------------------------------------------------------------
///                         プレイヤークラス
/// -------------------------------------------------------------
class Player : public BasePlayer
{
private: /// ---------- 構造体 ---------- ///

    /// ---------- 振る舞い ---------- ///
    enum class Behavior
    {
        kRoot,   // 通常状態
        kAttack, // 攻撃状態
    };

public: /// ---------- メンバ関数 ---------- ///

    // コンストラクタ
    Player();

    // 初期化処理
    void Initialize() override;

    // 更新処理
    void Update() override;

    // 描画処理
    void Draw() override;

    // 衝突判定
    void OnCollision(Collider* other) override;

    // 中心座標を取得する純粋仮想関数
    Vector3 GetCenterPosition() const override;

    // ImGui描画処理
    void DrawImGui() override;

    // スタート時の落下してくる挙動
    void FallingAnimation() override;

    // 死亡時の演出
    void DeadEffect() override;

    // 死亡時のカメラ演出
    void DeathCameraMove() override;

    // 出現演出
    void AppearFromAbove(float t) override;

    // 初期化の値をjsonから取得
    void InitializeValues(const std::string& filePath);

private: /// ---------- メンバ関数 ---------- ///

    // 移動処理
    void Move() override;

    // ヒット時のパーティクル
    void HitParticle();

    // 通常行動の初期化処理
    void BehaviorRootInitialize();

    // 通常行動の更新処理
    void BehaviorRootUpdate();

    // 攻撃行動の初期化処理
    void BehaviorAttackInitialize();

    // 攻撃行動の更新処理
    void BehaviorAttackUpdate();

private: /// ---------- メンバ変数 ---------- ///

    ContactRecord contactRecord_;

    // 振る舞い
    Behavior behavior_ = Behavior::kRoot; // 現在の行動

    // 次の振る舞いをリクエスト
    std::optional<Behavior> behaviorRequest_ = std::nullopt;

    // シリアルナンバー
    uint32_t serialNumber_ = 0;
    static uint32_t nextSerialNumber_;
};
