#include "BaseEnemy.h"
#include "Collider.h"
#include "ParticleEmitter.h"
#include <algorithm>

BaseEnemy::BaseEnemy() {
    serialNumber_ = nextSerialNumber_;
    nextSerialNumber_++;
    randomEngine.seed(seedGenerator());
}

void BaseEnemy::Initialize() {
    BaseCharacter::Initialize();
    // ColliderやTransformなどの初期化は派生クラスで行うこと
    hitTime_ = 0;
}

void BaseEnemy::Update() {
    BaseCharacter::Update();
    // ヒット時の処理
    if (isHit_) {
        if (isHitFromAttack_) {
            HitParticle();
            hitTime_++;
            if (hitTime_ >= hitMaxTime_) {
                isInvincible_ = false;
                isHit_ = false;
                isHitFromAttack_ = false;
                hitTime_ = 0;
            }
        } else {
            isHit_ = false;
        }
    }
    // 無敵時間のカウントダウン
    if (isInvincible_) {
        invincibleTime_ += 1;
        if (invincibleTime_ > invincibleDuration_) {
            invincibleTime_ = 0;
        }
    }
    Move();
}

void BaseEnemy::Draw() {
    if (!isInvincible_ || static_cast<int>(invincibleTime_) % 2 == 0) {
        BaseCharacter::Draw();
    }
}

void BaseEnemy::OnCollision(Collider* other) {
    isHit_ = true;
}

Vector3 BaseEnemy::GetCenterPosition() const {
    const Vector3 offset = { 0.0f, 0.0f, 0.0f };
    return worldTransform_.translate_ + offset;
}

void BaseEnemy::HitParticle() {
    if (particleEmitter_) {
        particleEmitter_->SetPosition(GetCenterPosition());
        particleEmitter_->Update(1.0f / 60.0f);
    }
}

void BaseEnemy::Move() {
    worldTransform_.translate_ += velocity_;
    worldTransform_.rotate_.y = std::atan2(-direction_.z, -direction_.x);
    worldTransform_.translate_.x = std::clamp(worldTransform_.translate_.x, minMoveLimit_.x, maxMoveLimit_.x);
    worldTransform_.translate_.z = std::clamp(worldTransform_.translate_.z, minMoveLimit_.z, maxMoveLimit_.z);
}

void BaseEnemy::ShowImGui(const char* name) {
    // ImGui描画は派生クラスで拡張
}
