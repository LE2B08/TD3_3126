#include "Sprite.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ResourceManager.h"
#include "ImGuiManager.h"


/// -------------------------------------------------------------
///							初期化処理
/// -------------------------------------------------------------
void Sprite::Initialize(const std::string& filePath)
{
	dxCommon = DirectXCommon::GetInstance();

	filePath_ = filePath;

	gpuHandle_ = TextureManager::GetInstance()->GetSrvHandleGPU(filePath_);

	// スプライトのインデックスバッファを作成および設定する
	CreateIndexBuffer();

	// スプライト用のマテリアルリソースを作成し設定する処理を行う
	CreateMaterialResource();

	// スプライトの頂点バッファリソースと変換行列リソースを生成
	CreateVertexBufferResource();

	// テクスチャサイズに合わせる
	AdjustTextureSize();
}


/// -------------------------------------------------------------
///							　更新処理
/// -------------------------------------------------------------
void Sprite::Update()
{
	// アンカーポイント
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// 左右反転
	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}

	// 上下反転
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	// メタデータ取得
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(filePath_);

	// テクスチャ範囲指定
	float tex_left = textureLeftTop_.x / metaData.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / metaData.width;
	float tex_top = textureLeftTop_.y / metaData.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metaData.height;

	/// ---------- 頂点データ設定 ---------- ///

	// 左上
	vertexData[0].position = { left, bottom, 0.0f, 1.0f };
	vertexData[0].texcoord = { tex_left, tex_bottom };

	// 左下
	vertexData[1].position = { left, top, 0.0f, 1.0f };
	vertexData[1].texcoord = { tex_left, tex_top };

	// 右下
	vertexData[2].position = { right, bottom, 0.0f, 1.0f };
	vertexData[2].texcoord = { tex_right, tex_bottom };

	// 右上
	vertexData[3].position = { right, top, 0.0f, 1.0f };
	vertexData[3].texcoord = { tex_right, tex_top };

	// ワールド行列の計算
	WorldTransform worldTransform;
	worldTransform.scale_ = { size_.x, size_.y, 1.0f };
	worldTransform.rotate_ = { 0.0f, 0.0f, rotation_ };
	worldTransform.translate_ = { position_.x, position_.y, 0.0f };

	Matrix4x4 worldMatrixSprite = Matrix4x4::MakeAffineMatrix(worldTransform.scale_, worldTransform.rotate_, worldTransform.translate_);

	Matrix4x4 viewMatrixSprite = Matrix4x4::MakeIdentity();
	Matrix4x4 projectionMatrixSprite = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Matrix4x4::Multiply(worldMatrixSprite, Matrix4x4::Multiply(viewMatrixSprite, projectionMatrixSprite));

	transformationMatrixData->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData->World = worldMatrixSprite;

	// 頂点データ更新
	memcpy(vertexData, &vertexData[0], sizeof(VertexData) * kNumVertex);
}


/// -------------------------------------------------------------
///						　スプライト描画
/// -------------------------------------------------------------
void Sprite::Draw()
{
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView); // スプライト用VBV
	dxCommon->GetCommandList()->IASetIndexBuffer(&indexBufferView); // IBVの設定
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource.Get()->GetGPUVirtualAddress());
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource.Get()->GetGPUVirtualAddress());

	// ディスクリプタテーブルの設定
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, gpuHandle_);

	dxCommon->GetCommandList()->DrawIndexedInstanced(kNumVertex, 1, 0, 0, 0);
}


/// -------------------------------------------------------------
///						テクスチャの変更
/// -------------------------------------------------------------
void Sprite::SetTexture(const std::string& filePath)
{
	filePath_ = filePath;

	gpuHandle_ = TextureManager::GetInstance()->GetSrvHandleGPU(filePath_);
}


/// -------------------------------------------------------------
///			　テクスチャの一部を指定して切り抜く処理
/// -------------------------------------------------------------
void Sprite::SetTextureRect(const Vector2& position, const Vector4& rect)
{
	// スプライトの描画位置を設定
	position_ = position;

	// テクスチャの左上座標とサイズを設定
	textureLeftTop_ = { rect.x, rect.y };

	// テクスチャのサイズを設定
	textureSize_ = { rect.z, rect.w };

	// スプライトのサイズを切り抜いたサイズに変更
	size_ = textureSize_;
}


/// -------------------------------------------------------------
///					HPの割合に応じてHPバーを描画
/// -------------------------------------------------------------
void Sprite::SetHPBar(float HPRatio, const Vector2& worldPos, const Vector4& textureRect, DecreaseHpDirection direction)
{
	// HPの割合を 0.0 ~ 1.0 の範囲にクランプ
	HPRatio = std::clamp(HPRatio, 0.0f, 1.0f);

	// 初期設定: 画像の切り抜き座標とサイズ
	textureLeftTop_ = { textureRect.x, textureRect.y };
	textureSize_ = { textureRect.z, textureRect.w };

	// スプライトのワールド座標とサイズ
	Vector2 newWorldPos = worldPos;
	Vector2 newSize = { textureRect.z, textureRect.w };

	// HPの減少方向によって処理を分岐
	switch (direction)
	{
	case DecreaseHpDirection::BottomToTop:// 下から上に減少
		
		textureSize_.y *= HPRatio; // 高さを縮める
		newSize.y *= HPRatio; // スプライトのサイズも縮める
		break;

	case DecreaseHpDirection::TopToBottom:// 上から下に減少

		textureLeftTop_.y += textureRect.w * (1.0f - HPRatio); // テクスチャの上側をずらす
		textureSize_.y *= HPRatio; // 高さを縮める
		newWorldPos.y += newSize.y * (1.0f - HPRatio); // ワールド座標も調整
		newSize.y *= HPRatio; // サイズも縮める
		break;

	case DecreaseHpDirection::LeftToRight:// 左から右に減少
		
		textureLeftTop_.x += textureRect.z * (1.0f - HPRatio); // テクスチャの左上座標をずらす
		textureSize_.x *= HPRatio; // 幅を縮める
		newWorldPos.x += newSize.x * (1.0f - HPRatio); // ワールド座標も調整
		newSize.x *= HPRatio; // スプライトのサイズも縮める
		break;

	case DecreaseHpDirection::RightToLeft:// 右から左に減少

		textureSize_.x *= HPRatio; // 幅を縮める
		newSize.x *= HPRatio; // スプライトのサイズも縮める
		break;
	}

	// 設定を反映
	position_ = newWorldPos;
	size_ = newSize;
}


/// -------------------------------------------------------------
///	 スプライト用のマテリアルリソースを作成し設定する処理を行う
/// -------------------------------------------------------------
void Sprite::CreateMaterialResource()
{
	//スプライト用のマテリアルソースを作る
	materialResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));

	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//UVTramsform行列を単位行列で初期化(スプライト用)
	materialData->uvTransform = Matrix4x4::MakeIdentity();
}


/// -------------------------------------------------------------
///	  スプライトの頂点バッファリソースと変換行列リソースを生成
/// -------------------------------------------------------------
void Sprite::CreateVertexBufferResource()
{
	//Sprite用の頂点リソースを作る
	vertexResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * kNumVertex);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * kNumVertex;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));
	// 座標変換行列リソースにデータを書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	//単位行列を書き込んでおく
	transformationMatrixData->World = Matrix4x4::MakeIdentity();
	transformationMatrixData->WVP = Matrix4x4::MakeIdentity();
}


/// -------------------------------------------------------------
///	   スプライトのインデックスバッファを作成および設定する
/// -------------------------------------------------------------
void Sprite::CreateIndexBuffer()
{
	indexResource = ResourceManager::CreateBufferResource(dxCommon->GetDevice(), sizeof(uint32_t) * kNumVertex);
	//リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス６つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * kNumVertex;
	//インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	// インデックスデータにデータを書き
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;
}


/// -------------------------------------------------------------
///			　テクスチャサイズをイメージに合わせる
/// -------------------------------------------------------------
void Sprite::AdjustTextureSize()
{
	// テクスチャメタデータを取得
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(filePath_);

	textureSize_.x = static_cast<float>(metaData.width);
	textureSize_.y = static_cast<float>(metaData.height);

	// 画像サイズをテクスチャサイズに合わせる
	size_ = textureSize_;
}
