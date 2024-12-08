export module simulation:functionfactory;

import std;

auto getConstantFunction(float A)
{
	return [=](float t)
	{
		return A;
	};
}

auto getStepFunction(float A, float t0)
{
	return [=](float t)
	{
		return t < t0 ? 0 : A;
	};
}

auto getSgnFunction(float A, float omega, float phi)
{
	return [=](float t)
	{
		auto val = A * std::sinf(omega * t + phi);
		if (val < 0.0f)
			return -1.0f;
		else if (val > 0.0f)
			return 1.0f;
		else
			return 0.0f;
	};
}

auto getSinFunction(float A, float omega, float phi)
{
	return [=](float t)
	{
		return A * std::sinf(omega * t + phi);
	};
}

export enum class FunctionType
{
	Constant = 0,
	Step = 1,
	Sgn = 2,
	Sin = 3
};

export namespace FunctionFactory
{
	std::function<float(float)> getFunction(FunctionType type, float A, float t0, float omega, float phi)
	{
		switch (type)
		{
		case FunctionType::Constant:
			return getConstantFunction(A);
		case FunctionType::Step:
			return getStepFunction(A, t0);
		case FunctionType::Sgn:
			return getSgnFunction(A, omega, phi);
		case FunctionType::Sin:
			return getSinFunction(A, omega, phi);
		default:
			return getConstantFunction(0.0f);
		}
	}
}