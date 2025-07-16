#pragma once
#include "BasePlayer.h"
#include <optional>

/// -------------------------------------------------------------
///                         チュートリアルプレイヤークラス
/// -------------------------------------------------------------
class TutorialPlayer : public BasePlayer {
private:
    enum class Behavior {
        kRoot,   // 通常状態
        kAttack, // 攻撃状態
    };

public:
    TutorialPlayer();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void OnCollision(Collider* other) override;
    Vector3 GetCenterPosition() const override;
    void DrawImGui() override;
    void Move() override;
    void DeadEffect() override;

    
    // 回転しているかの取得
    bool GetIsRotation() const { return isRotation_; }

private:
    // ヒット時のパーティクル
    void HitParticle();
    void BehaviorRootInitialize();
    void BehaviorRootUpdate();
    void BehaviorAttackInitialize();
    void BehaviorAttackUpdate();

    // 振る舞い
    Behavior behavior_ = Behavior::kRoot;
    std::optional<Behavior> behaviorRequest_ = std::nullopt;

    // シリアルナンバー
    uint32_t serialNumber_ = 0;
    static uint32_t nextSerialNumber_;

    // 回転フラグ
    bool isRotation_ = false;
    float prevRotateY_ = 0.0f;
};