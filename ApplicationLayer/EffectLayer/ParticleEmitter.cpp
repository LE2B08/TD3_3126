#include "ParticleEmitter.h"
#include <LogString.h>
#include <DirectXCommon.h>

ParticleEmitter::ParticleEmitter(ParticleManager* manager, const std::string& groupName)
	: particleManager_(manager), groupName_(groupName), position_({ 0.0f,0.0f,0.0f }),
	emissionRate_(10.0f), accumulatedTime_(0.0f)
{
}

void ParticleEmitter::Update(float deltaTime, ParticleEffectType type)
{
    accumulatedTime_ += deltaTime;
    
    // 発生させるパーティクルの数を計算
    if (static_cast<int>(emissionRate_) > 0) {
        particleManager_->Emit(groupName_, position_, static_cast<int>(emissionRate_), type);
        accumulatedTime_ -= static_cast<float>(static_cast<float>(emissionRate_)) / emissionRate_;
    }
}
