#pragma once
namespace Easing
{
	/*------Easingクラス------*/
	float Lerp(float start, float end, float t);

	float easeInOut(float t);

	float easeOut(float t);

	float easeIn(float t);

	float easeInElastic(float x);

	float easeOutElastic(float x);

	float easeInOutElastic(float x);

	// easeOutBounce 関数の宣言を追加
	float easeOutBounce(float x);

	// ベジエ曲線の追加
	float bezierCurve(float t, float p0, float p1, float p2, float p3);

	float easeInSine(float t);
};

