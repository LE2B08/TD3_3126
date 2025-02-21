#include "Model.h"

#include "ModelManager.h"
#include "DirectXCommon.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

/// -------------------------------------------------------------
///					　初期化処理
/// -------------------------------------------------------------
void Model::Initialize(const std::string& directoryPath, const std::string& filename)
{
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// モデル読み込み
	modelData = ModelManager::LoadObjFile(directoryPath, filename);
	
	// .objファイルの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);

	// 読み込んだテクスチャ番号を取得
	modelData.material.gpuHandle = TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textureFilePath);

	worldTransform.Initialize();
	/*cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-15.0f} };*/

	preInitialize(dxCommon);
}


/// -------------------------------------------------------------
///					　		更新処理
/// -------------------------------------------------------------
void Model::Update()
{
	Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix(worldTransform.scale_, worldTransform.rotate_, worldTransform.translate_);
	//Matrix4x4 camraMatrix = MakeAffineMatrix(cameraTransform.scale_, cameraTransform.rotate_, cameraTransform.translate_);
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(worldMatrix);
	Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;
}



void Model::SetBufferData(ID3D12GraphicsCommandList* commandList)
{
	// 定数バッファビュー (CBV) とディスクリプタテーブルの設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView); // モデル用VBV
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
}



void Model::DrawCall(ID3D12GraphicsCommandList* commandList)
{
	// ディスクリプタテーブルの設定
	commandList->SetGraphicsRootDescriptorTable(2, modelData.material.gpuHandle);

	// モデルの描画
	commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void Model::CameraImGui()
{
	/*ImGui::DragFloat3("cameraTranslate", &cameraTransform.translate_.x, 0.01f);
	ImGui::SliderAngle("CameraRotateX", &cameraTransform.rotate_.x);
	ImGui::SliderAngle("CameraRotateY", &cameraTransform.rotate_.y);
	ImGui::SliderAngle("CameraRotateZ", &cameraTransform.rotate_.z);*/
}

void Model::DrawImGui()
{
	if (ImGui::TreeNode("3DObject"))
	{
		/*ImGui::DragFloat3("cameraTranslate", &cameraTransform.translate_.x, 0.01f);
		ImGui::SliderAngle("CameraRotateX", &cameraTransform.rotate_.x);
		ImGui::SliderAngle("CameraRotateY", &cameraTransform.rotate_.y);
		ImGui::SliderAngle("CameraRotateZ", &cameraTransform.rotate_.z);*/

		ImGui::DragFloat3("scale", &worldTransform.scale_.x, 0.01f);
		ImGui::DragFloat3("rotate", &worldTransform.rotate_.x, 0.01f);
		ImGui::DragFloat3("translate", &worldTransform.translate_.x, 0.01f);
		ImGui::TreePop();
	}
}


void Model::preInitialize(DirectXCommon* dxCommon)
{
	InitializeMaterial(dxCommon);
	InitializeTransfomation(dxCommon);
	ParallelLightSorce(dxCommon);
	InitializeVertexBufferData(dxCommon);
}

/// -------------------------------------------------------------
///					　マテリアルの初期化処理
/// -------------------------------------------------------------
void Model::InitializeMaterial(DirectXCommon* dxCommon)
{
#pragma region マテリアル用のリソースを作成しそのリソースにデータを書き込む処理を行う
	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));
	// マテリアルにデータを書き込む
	Material* materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 今回は赤を書き込んでみる
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->enableLighting = true; // 平行光源を有効にする
	// UVTramsform行列を単位行列で初期化
	materialData->uvTransform = Matrix4x4::MakeIdentity();
#pragma endregion
}


/// -------------------------------------------------------------
///					　座標変換行列の初期化処理
/// -------------------------------------------------------------
void Model::InitializeTransfomation(DirectXCommon* dxCommon)
{
#pragma region WVP行列データを格納するバッファリソースを生成し初期値として単位行列を設定
	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	wvpData->World = Matrix4x4::MakeIdentity();
	wvpData->WVP = Matrix4x4::MakeIdentity();
#pragma endregion
}


/// -------------------------------------------------------------
///					　平行光源の初期化処理
/// -------------------------------------------------------------
void Model::ParallelLightSorce(DirectXCommon* dxCommon)
{
//#pragma region 平行光源のプロパティ 色 方向 強度 を格納するバッファリソースを生成しその初期値を設定
//	//平行光源用のリソースを作る
//	directionalLightResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(DirectionalLight));
//	//書き込むためのアドレスを取得
//	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
//
//	directionalLightData->color = { 1.0f,1.0f,1.0f ,1.0f };
//	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
//	directionalLightData->intensity = 1.0f;
//#pragma endregion
}


/// -------------------------------------------------------------
///				　頂点バッファデータの初期化
/// -------------------------------------------------------------
void Model::InitializeVertexBufferData(DirectXCommon* dxCommon)
{
#pragma region 頂点バッファデータの開始位置サイズおよび各頂点のデータ構造を指定
	// 頂点バッファビューを作成する
	vertexResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * (modelData.vertices.size() + TotalVertexCount));
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();									 // リソースの先頭のアドレスから使う
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * (modelData.vertices.size() + TotalVertexCount));	 // 使用するリソースのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);														 // 1頂点あたりのサイズ
#pragma endregion

	VertexData* vertexData = nullptr;																			 // 頂点リソースにデータを書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));										 // 書き込むためのアドレスを取得

	// モデルデータの頂点データをコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());


	// 球体の頂点データをコピー
	VertexData* sphereVertexData = vertexData + modelData.vertices.size();
	auto calculateVertex = [](float lat, float lon, float u, float v) {
		VertexData vertex;
		vertex.position = { cos(lat) * cos(lon), sin(lat), cos(lat) * sin(lon), 1.0f };
		vertex.texcoord = { u, v };
		vertex.normal = { vertex.position.x, vertex.position.y, vertex.position.z };
		return vertex;
		};

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex; // θ
		float nextLat = lat + kLatEvery;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float u = float(lonIndex) / float(kSubdivision);
			float v = 1.0f - float(latIndex) / float(kSubdivision);
			float lon = lonIndex * kLonEvery; // Φ
			float nextLon = lon + kLonEvery;

			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;

			// 6つの頂点を計算
			sphereVertexData[start + 0] = calculateVertex(lat, lon, u, v);
			sphereVertexData[start + 1] = calculateVertex(nextLat, lon, u, v - 1.0f / float(kSubdivision));
			sphereVertexData[start + 2] = calculateVertex(lat, nextLon, u + 1.0f / float(kSubdivision), v);
			sphereVertexData[start + 3] = calculateVertex(nextLat, nextLon, u + 1.0f / float(kSubdivision), v - 1.0f / float(kSubdivision));
			sphereVertexData[start + 4] = calculateVertex(lat, nextLon, u + 1.0f / float(kSubdivision), v);
			sphereVertexData[start + 5] = calculateVertex(nextLat, lon, u, v - 1.0f / float(kSubdivision));
		}
	}

	// アンマップ
	vertexResource->Unmap(0, nullptr);
}
