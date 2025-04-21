#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

///==========================================================
/// 頂点データの拡張
///==========================================================
struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
