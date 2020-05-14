#pragma once
#include <DirectXMath.h>
#include <algorithm>
#include <cmath>

namespace MathHelper
{
	struct Ray
	{
		DirectX::XMVECTOR origin;
		DirectX::XMVECTOR direction;
	};
	constexpr float PI = 3.14159265359f;
	constexpr float ToRadians = 0.0174532925f;
	constexpr float ToDegrees = 57.2957795f;

	inline float Lerp (float a, float b, float t)
	{
		return (b - a) * t + a;
	}

	inline float Clamp(const float& value, const float& min, const float& max)
	{
		return std::fmaxf(min, std::fminf(value, max));
	}

	inline float SmoothDamp(const float& current, const float& target, const float& time, const float& deltaTime, float& refVelocity)
	{
		float t_target = target;
		float smoothTime = std::fmaxf(0.0f, time);
		float omega = 2.0f / smoothTime;

		float x = omega * deltaTime;
		float exp = 1.f / (1.f + x + 0.48f * x * x + 0.235f * x * x * x);
		float change = current - t_target;
		float originalTo = t_target;

		const float MAX_SPEED = 10000.0f;

		//clamp
		float maxChange = MAX_SPEED * smoothTime;
		change = Clamp(change, -maxChange, maxChange);
		t_target = current - change;

		float temp = (refVelocity + omega * change) * deltaTime;
		float output = target + (change + temp) * exp;
		refVelocity = (refVelocity - omega * temp) * exp;

		if (originalTo - current > 0.0f == output > originalTo)
		{
			output = originalTo;
			refVelocity = (output - originalTo) / deltaTime;
		}

		return output;
	}
	inline float ShortestRotation(float currentDir, float nextDir)
	{
		float returnValue = 0;

		if (abs(nextDir - currentDir) < MathHelper::PI)
			returnValue = nextDir - currentDir;
		else if (currentDir < nextDir)
			returnValue = nextDir - currentDir - MathHelper::PI * 2.0f;
		else
			returnValue = nextDir - currentDir + MathHelper::PI * 2.0f;
		return returnValue;
	}

}