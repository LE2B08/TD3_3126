#pragma once
#include "BaseCharacter.h"
#include <memory>
#include <optional>
#include <ParticleEmitter.h>
#include <Vector3.h>
#include <Camera.h>
#include "Enemy.h"
#include "Weapon.h"
class Hook; // 前方宣言を追加

// 共通Player基底クラス
class BasePlayer : public BaseCharacter {
public:
    BasePlayer();
    virtual ~BasePlayer() = default;

    // 共通インターフェース
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void OnCollision(Collider* other) override;
    virtual Vector3 GetCenterPosition() const override;
    virtual void DrawImGui();

    // 共通移動処理
    virtual void Move();

    // 共通演出
    virtual void DeadEffect();
    virtual void FallingAnimation();
    virtual void DeathCameraMove();
    virtual void AppearFromAbove(float t);

    // ゲッター/セッター
    Vector3 GetVelocity() const { return velocity_; }
    Vector3 GetAcceleration() const { return acceleration_; }
    Vector3 GetAngularVelocity() const { return angularVelocity_; }
    bool IsDead() const { return isDead_; }
    bool GetIsAttack() const { return isAttack_; }
    bool GetIsHitEnemy() const { return isEnemyHit_; }
    bool GetIsFallEnd() const { return isFallEnd_; }
    void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
    void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }
    void SetAngularVelocity(const Vector3& angularVelocity) { angularVelocity_ = angularVelocity; }
    void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }
    void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }
    void SetIsHitEnemy(bool isHitEnemy) { isEnemyHit_ = isHitEnemy; }
    void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
    void SetIsGameStart(bool isGameStart) { isGameStart_ = isGameStart; }
    void SetCamera(Camera* camera) { camera_ = camera; }
    void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
    void SetWeapon(Weapon* weapon) { weapon_ = weapon; }
    void SetHook(Hook* hook) { hook_ = hook; }

protected:
    // 共通メンバ変数
    Enemy* enemy_ = nullptr;
    Weapon* weapon_ = nullptr;
    Hook* hook_ = nullptr;
    Camera* camera_ = nullptr;
    Vector3 velocity_{};
    Vector3 acceleration_{};
    Vector3 angularVelocity_{};
    Vector3 maxMoveLimit_ = { 24.0f, 0.0f, 24.0f };
    Vector3 minMoveLimit_ = { -24.0f, 0.0f, -24.0f };
    std::unique_ptr<ParticleEmitter> particleEmitter_;
    std::unique_ptr<ParticleEmitter> particleEmitter2_;
    bool hasEmittedAppearEffect_ = false;
    bool isInvincible_ = false;
    const float invincibleDuration_ = 60.0f;
    float invincibleTime_ = 0;
    bool isDead_ = false;
    bool isAttack_ = false;
    float attackTime_ = 0.0f;
    float attackMaxTime_ = 60.0f;
    bool isGameStart_ = false;
    bool isEnemyHit_ = false;
    float rotationStartT_ = 0.0f;
    float rotationMaxT_ = 160.0f;
    float cameraMoveT_ = 0.0f;
    float cameraMoveMaxT_ = 80.0f;
    float fallingTimer_ = 0;
    const float maxFallingTime = 40;
    bool isFallEnd_ = false;
    Vector3 startAnimationPos_ = { 0.0f, 40.0f, -50.0f };
    Vector3 endAnimationPos_ = { 0.0f, 1.0f, -25.0f };
};
