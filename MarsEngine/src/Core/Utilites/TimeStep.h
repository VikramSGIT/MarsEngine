#pragma once

namespace ME
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			:m_Time(time) {}

		operator float() const { return m_Time; }

		float GetSeconds() { return m_Time; }
		float GetMilliSeconds() { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}
