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
};

