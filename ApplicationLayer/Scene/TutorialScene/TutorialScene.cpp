#include "TutorialScene.h"
#include <DirectXCommon.h>
#include <Input.h>
#include <SceneManager.h>

void TutorialScene::Initialize() {
	input_ = Input::GetInstance();
}

void TutorialScene::Update() {

	if (input_->TriggerKey(DIK_RETURN)) {
		if (sceneManager_) {
			
			sceneManager_->ChangeScene("GamePlayScene");
		}
	}
}

void TutorialScene::Draw() {
}

void TutorialScene::Finalize() {
}

void TutorialScene::DrawImGui() {
}
