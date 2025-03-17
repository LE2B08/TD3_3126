#include "Easing.h"
#include <corecrt_math_defines.h>
#include <corecrt_math.h>

float Easing::Lerp(float start, float end, float t)
{
	return start + (end - start) * t;
}

float Easing::easeInOut(float t) {
	float easedT = -(cosf((float)M_PI * t) - 1.0f) / 2.0f;


	return easedT;
}


float Easing::easeOut(float t) {
	float easedT = 1.0f -
		powf(1.0f - t, 3.0f);
	if (t > 1.0f)
	{
		easedT = 1.0f;
	}
	return easedT;
}


float Easing::easeIn(float t)
{
	float easedT = t * t;
	if (t > 1.0f)
	{
		easedT = 1.0f;
	}

	return easedT;
}





float Easing::easeInElastic(float x) {
	const float c4 = (2.0f * (float)M_PI) / 3.0f;

	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
}
float Easing::easeOutElastic(float x) {
	const float c4 = (2.0f * (float)M_PI) / 3.0f;

	return x == 0
		? 0.0f
		: x == 1.0f
		? 1.0f
		: powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
}

float Easing::easeInOutElastic(float x) {
	const float c5 = (2.0f * (float)M_PI) / 4.5f;

	return x == 0.0f
		? 0.0f
		: x == 1.0f
		? 1.0f
		: x < 0.5f
		? -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f
		: (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
}

float Easing::easeOutBounce(float x)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1.0f / d1) {
		return n1 * x * x;
	} else if (x < 2.0f / d1) {
		x -= 1.5f / d1;
		return n1 * x * x + 0.75f;
	} else if (x < 2.5f / d1) {
		x -= 2.25f / d1;
		return n1 * x * x + 0.9375f;
	} else {
		x -= 2.625f / d1;
		return n1 * x * x + 0.984375f;
	}
}
