#include "ParticleEmitter.h"
#include <LogString.h>
#include <DirectXCommon.h>

ParticleEmitter::ParticleEmitter(ParticleManager* manager, const std::string& groupName)
	: particleManager_(manager), groupName_(groupName), position_({ 0.0f,0.0f,0.0f }),
	emissionRate_(10.0f), accumulatedTime_(0.0f)
{
}

void ParticleEmitter::Update(float deltaTime)
{
    accumulatedTime_ += deltaTime;
    
    int particleCount = static_cast<int>(emissionRate_);
    if (particleCount > 0)
    {
        // ← グループに設定された type を取得
        ParticleEffectType type = particleManager_->GetGroupType(groupName_);

        // ← 自動で対応した type を使って射出
        particleManager_->Emit(groupName_, position_, particleCount, type);

        accumulatedTime_ -= static_cast<float>(particleCount) / emissionRate_;
    }
}
