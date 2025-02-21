#pragma once
#include "Matrix4x4.h"

///==========================================================
/// TransformationMatrixを拡張
///==========================================================
struct TransformationMatrix final
{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInversedTranspose;
};
///==========================================================
/// TransformationMatrixを拡張
///==========================================================