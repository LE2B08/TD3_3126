#include "BasePlayer.h"

BasePlayer::BasePlayer() = default;

void BasePlayer::Initialize() {
    BaseCharacter::Initialize();
}

void BasePlayer::Update() {
    // 共通のUpdate処理（派生で拡張可）
}

void BasePlayer::Draw() {
    BaseCharacter::Draw();
}

void BasePlayer::OnCollision(Collider* other) {
    // 共通のOnCollision処理（派生で拡張可）
}

Vector3 BasePlayer::GetCenterPosition() const {
    return worldTransform_.translate_;
}

void BasePlayer::DrawImGui() {}
void BasePlayer::Move() {}
void BasePlayer::DeadEffect() {}
void BasePlayer::FallingAnimation() {}
void BasePlayer::DeathCameraMove() {}
void BasePlayer::AppearFromAbove(float) {}
