#pragma once
#include "BasePlayer.h"
#include <optional>
#include "ContactRecord.h"
#include "PlayerUI.h" // PlayerUIのインクルードを追加

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
