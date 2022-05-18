#pragma once

#include <random>

namespace ME
{
	namespace Utils
	{
		template<typename T>
		class Random
		{
		public:
			Random()
			{
				m_RandomEngine.seed(std::random_device()());
			}
			~Random() = default;

			void SetMin(float range) { m_Range = range; }

			T operator()() { return m_Range * static_cast<T>(m_Distribution(m_RandomEngine)) / static_cast<T>(std::numeric_limits<uint32_t>::max()); }
		private:
			T m_Range = static_cast<T>(1);
			std::mt19937 m_RandomEngine;
			std::uniform_int_distribution<std::mt19937::result_type> m_Distribution;
		};
	}
}