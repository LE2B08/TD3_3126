#include "CameraManager.h"
#include <imgui.h>

/*------シングルトンインスタンスの生成------*/
CameraManager* CameraManager::GetInstance()
{
	static CameraManager instance;
	return &instance;
}

void CameraManager::Finalize()
{
	cameras_.clear();
}

/*------初期化------*/
void CameraManager::Initialize()
{
	// デフォルトカメラの生成
	AddCamera("DefaultCamera");
	// デフォルトカメラの設定
	SetCurrentCamera("DefaultCamera");
}

/*------更新------*/
void CameraManager::Update()
{
	for (auto& pair : cameras_) {
		if (pair.second) {
			pair.second->Update();
		}
	}
}

/*------ImGui描画------*/
void CameraManager::DrawImGui()
{
	ImGui::Begin("CameraManager");
	ImGui::Text("CurrentCamera : %s", currentCameraName_.c_str());
	for (auto& pair : cameras_) {
		if (pair.second) {
			// カメラの情報を表示
			ImGui::Text("Camera Name: %s", pair.first.c_str());
			ImGui::Text("Position: %.2f, %.2f, %.2f", pair.second->GetTranslate().x, pair.second->GetTranslate().y, pair.second->GetTranslate().z);
			ImGui::Text("Rotation: %.2f, %.2f, %.2f", pair.second->GetRotate().x, pair.second->GetRotate().y, pair.second->GetRotate().z);
			ImGui::Separator();
		}
	}
	ImGui::End();
}

/*------カメラの追加------*/
void CameraManager::AddCamera(const std::string& name)
{
	// カメラを作成
	std::unique_ptr<Camera> camera = std::make_unique<Camera>();
	
	// カメラを登録
	cameras_[name] = std::move(camera);
}

/*------カメラを取得------*/
Camera* CameraManager::GetCamera(const std::string& name) const
{
	auto it = cameras_.find(name);
	if (it != cameras_.end()) {
		return it->second.get();
	}
	return nullptr;
}

/*------現在のカメラを設定------*/
void CameraManager::SetCurrentCamera(const std::string& name)
{
	if (cameras_.find(name) != cameras_.end()) {
		currentCameraName_ = name;
	}
}

/*------現在のカメラを取得------*/
Camera* CameraManager::GetCurrentCamera() const
{
	return GetCamera(currentCameraName_);
}
