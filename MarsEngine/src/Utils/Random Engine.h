#pragma once

#include <random>

namespace Mars
{
	enum Type
	{
		NON_REPEATING_CYCLING_ADJUSTABLE_MINMAX
	};
	template<typename T, Type type>
	class Random
	{
		Random() = default;
		~Random() = default;

		void SetMin(T min) {}
		void SetMax(T max) {}
		void Seed(T seed) {}
	private:
		T min, max, seed;
		std::default_random_engine drm;
		std::linear_congruential_engine lce;
	};
}