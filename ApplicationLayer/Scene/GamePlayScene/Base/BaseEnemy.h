#pragma once
#include "BaseCharacter.h"
#include "Vector3.h"
#include <random>
#include <memory>

class ParticleEmitter;
class ParticleManager;
class Collider;

class BaseEnemy : public BaseCharacter {
public:
    BaseEnemy();
    virtual ~BaseEnemy() = default;

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void OnCollision(Collider* other) override;
    virtual Vector3 GetCenterPosition() const override;
    virtual void HitParticle();
    virtual void Move();
    virtual void ShowImGui(const char* name);

    // Getters
    bool GetIsHit() const { return isHit_; }
    const Vector3& GetPosition() const { return worldTransform_.translate_; }
    const bool& GetIsEnemyCameraEffect() const { return isEnemyCameraEffect_; }
    const bool& GetIsCameraEffectEnd() const { return isCameraEffectEnd_; }
    const bool& IsDead() const { return isDead_; }
    const bool& GetIsInvincible() const { return isInvincible_; }
    uint32_t GetSerialNumber() const { return serialNumber_; }

    // Setters
    void SetIsHit(bool isHit) { isHit_ = isHit; }
    void SetIsHitFromAttack(bool isHitFromAttack) { isHitFromAttack_ = isHitFromAttack; }
    void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }
    void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }
    void SetIsEnemyCameraEffect(bool isEnemyCameraEffect) { isEnemyCameraEffect_ = isEnemyCameraEffect; }
    void SetSerialNumber(uint32_t serialNumber) { serialNumber_ = serialNumber; }

protected:
    // 共通メンバ変数
    Vector3 velocity_;
    Vector3 direction_ = {};
    Vector3 maxMoveLimit_ = {};
    Vector3 minMoveLimit_ = {};
	float moveSpeed_;
    std::unique_ptr<ParticleEmitter> particleEmitter_;
    std::unique_ptr<ParticleEmitter> particleEmitter2_;
    std::unique_ptr<ParticleEmitter> particleEmitter3_;
    bool isHit_ = false;
    bool isInvincible_ = false;
    const float invincibleDuration_ = 60.0f;
    float invincibleTime_ = 0;
    bool isHitFromAttack_ = false;
    float hitTime_ = 0.0f;
    float hitMaxTime_ = 60.0f;
    uint32_t serialNumber_ = 0;
    uint32_t nextSerialNumber_ = 0;
    std::random_device seedGenerator;
    std::mt19937 randomEngine;
    float cameraMoveT_ = 0.0f;
    const float cameraMoveMaxT_ = 160.0f;
    bool isEnemyCameraEffect_ = true;
    float enemyCameraEffectT_ = 0.0f;
    bool isCameraEffectEnd_ = false;
    bool isCameraBackEffect_ = false;
    float cameraBackEffectT_ = 0.0f;
    const float cameraBackEffectMaxT_ = 160.0f;
    float rotationStartT_ = 0.0f;
    const float rotationMaxT_ = 160.0f;
    float DeathCameraMoveT_ = 0.0f;
    const float DeathCameraMoveMaxT_ = 80.0f;
    bool isDead_ = false;
};
