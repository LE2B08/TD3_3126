#include "Player.h"
#include "CollisionTypeIdDef.h"
#include "Enemy.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "ParticleManager.h"
#include "Weapon.h"
#include "Wireframe.h"
#include <Easing.h>
#include <AudioManager.h>
using namespace Easing;

// Playerの静的メンバ変数の定義
uint32_t Player::nextSerialNumber_ = 0;

// -------------------------------------------------------------
// コンストラクタ
// -------------------------------------------------------------
Player::Player() {
    // シリアルナンバーを振る
    serialNumber_ = nextSerialNumber_;
    // 次のシリアルナンバーに1を足す
    ++nextSerialNumber_;
}

// -------------------------------------------------------------
// 初期化処理
// -------------------------------------------------------------
void Player::Initialize() {
    // 基底クラスの初期化
    BasePlayer::Initialize();
    // プレイヤーのコライダーの設定
    Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
    // 3Dオブジェクトの生成・初期化
    object3D_ = std::make_unique<Object3D>();
    object3D_->Initialize("Voxel_Human.gltf");
    // 初期位置をアニメーション用の座標に設定
    worldTransform_.translate_ = startAnimationPos_;
    // パーティクルグループの生成（爆発・リング）
    ParticleManager::GetInstance()->CreateParticleGroup("ExplosionEffect", "circle.png", ParticleEffectType::Explosion);
    ParticleManager::GetInstance()->CreateParticleGroup("RingEffect", "gradationLine.png", ParticleEffectType::Ring);
    // パーティクルエミッターの初期化
    particleEmitter_ = std::make_unique<ParticleEmitter>(ParticleManager::GetInstance(), "ExplosionEffect");
    particleEmitter2_ = std::make_unique<ParticleEmitter>(ParticleManager::GetInstance(), "RingEffect");
    particleEmitter2_->SetEmissionRate(1.0f);
}

// -------------------------------------------------------------
// 更新処理
// -------------------------------------------------------------
void Player::Update() {
    // 無敵時間のカウントダウン
    if (isInvincible_) {
        isEnemyHit_ = true; // 敵に当たったフラグを立てる
        invincibleTime_ += 1; // 1フレームごとにカウントアップ
        if (invincibleTime_ > invincibleDuration_) {
            isInvincible_ = false; // 無敵状態を解除
            invincibleTime_ = 0;   // 無敵時間の初期化
            isEnemyHit_ = false;   // 敵に当たったフラグを解除
        }
    }
    // 武器の更新
    if (weapon_) {
        weapon_->Update();
    }
    // 攻撃ボタンが押されたら攻撃状態へ遷移リクエスト
    if (input_->TriggerButton(8) && behavior_ == Behavior::kRoot) {
        behaviorRequest_ = Behavior::kAttack;
    }
    // ビヘイビア（状態）遷移処理
    if (behaviorRequest_) {
        // 状態を変更
        behavior_ = behaviorRequest_.value();
        // 状態ごとの初期化
        switch (behavior_) {
        case Behavior::kRoot:
        default:
            // 通常行動の初期化
            BehaviorRootInitialize();
            break;
        case Behavior::kAttack:
            // 攻撃行動の初期化
            BehaviorAttackInitialize();
            break;
        }
        // 状態リクエストをリセット
        behaviorRequest_ = std::nullopt;
    }
    // 現在の状態ごとの更新処理
    switch (behavior_) {
    case Behavior::kRoot:
    default:
        // 通常行動の更新
        BehaviorRootUpdate();
        break;
    case Behavior::kAttack:
        // 攻撃行動の更新
        BehaviorAttackUpdate();
        break;
    }
}

// -------------------------------------------------------------
// 描画処理
// -------------------------------------------------------------
void Player::Draw() {
    // 無敵時は点滅（1フレームごとに描画をスキップ）
    if (!isInvincible_ || static_cast<int>(invincibleTime_) % 2 == 0) {
        BasePlayer::Draw();
    }
    // 攻撃中は武器も描画
    if (isAttack_ && weapon_) {
        weapon_->Draw();
    }
#ifdef _DEBUG
    // デバッグ時：プレイヤーの向きを示す線を描画
    Vector3 direction = { cos(worldTransform_.rotate_.y), 0.0f, sin(worldTransform_.rotate_.y) };
    Vector3 endPos = worldTransform_.translate_ + -direction * 5.0f; // 5.0fは線の長さ
    Wireframe::GetInstance()->DrawLine(worldTransform_.translate_, endPos, { 0.0f, 1.0f, 0.0f, 1.0f }); // 緑色の線
#endif
}

// -------------------------------------------------------------
// 衝突判定処理
// -------------------------------------------------------------
void Player::OnCollision(Collider* other) {
    // 衝突相手の種別IDを取得
    uint32_t typeID = other->GetTypeID();
    if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
        // 敵と衝突した場合
        Enemy* enemy = static_cast<Enemy*>(other);
        uint32_t serialNumber = {};
        if (!isInvincible_) {
            // 敵からダメージを受けられる場合のみHP減少
            if (enemy_->CanGiveDamage()) {
                hp_ -= 2;
            }
            // HPが0以下なら死亡フラグ
            if (hp_ <= 0) {
                isEnemyHit_ = false;
            }
            // ダメージSE再生
            AudioManager::GetInstance()->PlaySE("dageki.mp3", 0.4f);
            // 無敵状態にする
            isInvincible_ = true;
            invincibleTime_ = 0;
        }
        // パーティクル演出（ダメージ時）
        if (enemy_->CanGiveDamage()) {
            particleEmitter_->SetPosition(GetCenterPosition());
            particleEmitter_->Update(1.0f / 60.0f);
            particleEmitter2_->SetPosition(GetCenterPosition());
            particleEmitter2_->Update(1.0f / 60.0f);
        }
        // 連続ヒット防止のためのシリアル管理
        if (contactRecord_.Check(serialNumber)) return;
        contactRecord_.Add(serialNumber);
    }
    else if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) {
        // 敵の弾と衝突した場合
        if (!isInvincible_) {
            hp_ -= 1;
            isInvincible_ = true;
            invincibleTime_ = 0;
            // ダメージSE再生
            AudioManager::GetInstance()->PlaySE("dageki.mp3");
        }
        // パーティクル演出
        particleEmitter_->SetPosition(GetCenterPosition());
        particleEmitter_->Update(1.0f / 60.0f);
        particleEmitter2_->SetPosition(GetCenterPosition());
        particleEmitter2_->Update(1.0f / 60.0f);
    }
}

// -------------------------------------------------------------
// 中心座標を取得
// -------------------------------------------------------------
Vector3 Player::GetCenterPosition() const {
    // プレイヤーの中心を考慮した座標を返す
    const Vector3 offset = { 0.0f, 0.0f, 0.0f };
    Vector3 worldPosition = worldTransform_.translate_ + offset;
    return worldPosition;
}

// -------------------------------------------------------------
// ImGui描画処理
// -------------------------------------------------------------
void Player::DrawImGui() {
    ImGui::Begin("Player");
    ImGui::Text("HP: %d", hp_);
    ImGui::DragInt("HP", &hp_, 1, 0, 10);
    ImGui::DragFloat3("Translate", &worldTransform_.translate_.x, 0.1f);
    ImGui::DragFloat3("Rotation", &worldTransform_.rotate_.x, 0.1f);
    ImGui::DragFloat3("Scale", &worldTransform_.scale_.x, 0.1f);
    ImGui::End();
}

// -------------------------------------------------------------
// スタート時の落下してくる挙動
// -------------------------------------------------------------
void Player::FallingAnimation() {
    // 落下アニメーションの進行
    if (fallingTimer_ >= maxFallingTime) {
        fallingTimer_ = maxFallingTime;
    }
    else {
        fallingTimer_ += 0.5f;
    }
    // イージングで落下位置を補間
    worldTransform_.translate_ = Vector3::Lerp(startAnimationPos_, endAnimationPos_, easeOutBounce(fallingTimer_ / maxFallingTime));
    worldTransform_.rotate_.y = -1.55f;
    // 落下が終わったらフラグON
    if (fallingTimer_ == maxFallingTime) {
        isFallEnd_ = true;
    }
}

// -------------------------------------------------------------
// 死亡時のカメラ演出
// -------------------------------------------------------------
void Player::DeathCameraMove() {
    // プレイヤーとカメラの座標を取得
    Vector3 playerPosition = GetPosition();
    Vector3 cameraPosition = camera_->GetTranslate();
    Vector3 moveCameraPosition = cameraPosition;
    Vector3 cameraOffset = playerPosition + Vector3(0.0f, 0.56f, -10.0f); // プレイヤーの後方にカメラを移動
    Vector3 cameraRotation = camera_->GetRotate();
    Vector3 moveCameraRotation = cameraRotation;
    // カメラ移動の進行
    if (cameraMoveT_ >= cameraMoveMaxT_) {
        cameraMoveT_ = cameraMoveMaxT_;
        DeadEffect();
    }
    else {
        cameraMoveT_ += 1.0f;
        worldTransform_.rotate_.y = 1.5f;
    }
    // イージングでカメラ位置・回転を補間
    moveCameraPosition = Vector3::Lerp(cameraPosition, cameraOffset, easeIn(cameraMoveT_ / cameraMoveMaxT_));
    moveCameraRotation = Vector3::Lerp(cameraRotation, Vector3(0.0f, 0.0f, 0.0f), easeIn(cameraMoveT_ / cameraMoveMaxT_));
    camera_->SetTranslate(moveCameraPosition);
    camera_->SetRotate(moveCameraRotation);
}

// -------------------------------------------------------------
// 移動処理
// -------------------------------------------------------------
void Player::Move() {
    // 加速度を速度に加算
    velocity_ += acceleration_;
    // 最大速度制限
    const float maxSpeed = 10.0f;
    float speed = Vector3::Length(velocity_);
    if (speed > maxSpeed) {
        velocity_ = (velocity_ / speed) * maxSpeed;
    }
    // 摩擦による減速
    velocity_ *= 0.98f;
    // 速度を座標に加算
    worldTransform_.translate_ += velocity_;
    // 加速度リセット
    acceleration_ = { 0.0f, 0.0f, 0.0f };
    // 右スティック入力で回転
    if (!input_->RStickInDeadZone() && hp_ > 0)
        worldTransform_.rotate_.y = -atan2(input_->GetRightStick().x, input_->GetRightStick().y) - std::numbers::pi_v<float> / 2.0f;
    // 移動制限
    worldTransform_.translate_.x = std::clamp(worldTransform_.translate_.x, minMoveLimit_.x, maxMoveLimit_.x);
    worldTransform_.translate_.z = std::clamp(worldTransform_.translate_.z, minMoveLimit_.z, maxMoveLimit_.z);
    // 3Dオブジェクトの更新
    object3D_->SetScale(worldTransform_.scale_);
    object3D_->SetRotate(worldTransform_.rotate_);
    object3D_->SetTranslate(worldTransform_.translate_);
    object3D_->Update();
}

// -------------------------------------------------------------
// 死亡時の演出
// -------------------------------------------------------------
void Player::DeadEffect() {
    // 死亡時は回転しながら小さくなって消滅
    Vector3 scale = worldTransform_.scale_;
    Vector3 rotation = worldTransform_.rotate_;
    Vector3 rotationEnd = { 0.0f, -17.2f, 0.0f };
    if (rotationStartT_ >= rotationMaxT_) {
        rotationStartT_ = rotationMaxT_;
        isDead_ = true;
    }
    else {
        rotationStartT_ += 1.0f;
    }
    // 回転・スケールをイージングで補間
    worldTransform_.rotate_ = Vector3::Lerp(rotation, rotationEnd, Easing::easeInOut(rotationStartT_ / rotationMaxT_));
    worldTransform_.scale_ = Vector3::Lerp(scale, Vector3(0.0f, 0.0f, 0.0f), Easing::easeInOutElastic(rotationStartT_ / rotationMaxT_));
    // 消滅時に一度だけパーティクル演出
    if (scale.x == 0.0f && scale.y == 0.0f && scale.z == 0.0f) {
        if (!hasEmittedAppearEffect_) {
            particleEmitter_->SetPosition(GetCenterPosition());
            particleEmitter_->SetEmissionRate(5.0f);
            particleEmitter_->Update(1.0f / 60.0f);
            hasEmittedAppearEffect_ = true;
        }
        return;
    }
}

// -------------------------------------------------------------
// 通常行動の初期化処理
// -------------------------------------------------------------
void Player::BehaviorRootInitialize() {
    isAttack_ = false;  // 攻撃フラグをリセット
    attackTime_ = 0.0f; // 攻撃時間をリセット
}

// -------------------------------------------------------------
// 通常行動の更新処理
// -------------------------------------------------------------
void Player::BehaviorRootUpdate() {
    // 通常移動処理
    Move();
}

// -------------------------------------------------------------
// 攻撃行動の初期化処理
// -------------------------------------------------------------
void Player::BehaviorAttackInitialize() {
    attackTime_ = 0.0f; // 攻撃時間をリセット
    isAttack_ = true;   // 攻撃フラグを立てる
}

// -------------------------------------------------------------
// 攻撃行動の更新処理
// -------------------------------------------------------------
void Player::BehaviorAttackUpdate() {
    // 攻撃中も移動できるようにする
    velocity_ *= 0.5f; // 移動速度を半分にする
    Move();
    // 攻撃時間をカウント
    attackTime_++;
    // 攻撃時間が最大値に達したら攻撃フラグをリセット
    if (attackTime_ >= attackMaxTime_) {
        isAttack_ = false;
        behaviorRequest_ = Behavior::kRoot;
    }
    else {
        weapon_->Attack();
    }
}

// -------------------------------------------------------------
// 出現演出
// -------------------------------------------------------------
void Player::AppearFromAbove(float t) {
    // 上空から地面に落下する演出（イージング）
    SetPosition(Vector3::Lerp({ 8.0f, 20.0f, 8.0f }, { 8.0f, 0.0f, 8.0f }, Easing::easeOutBounce(t)));
}
