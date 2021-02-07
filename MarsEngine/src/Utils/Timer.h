#pragma once

#include "MarsHeader.h"

#include <chrono>
#include <vector>
namespace ME
{
	namespace Utils
	{
		namespace Time
		{
			enum class Precision
			{
				MILLI, MICRO, SEC
			};
			template<Precision T>
			struct Time
			{
				long long m_Time;
				Time(const long long time = 0)
					:m_Time(time) {}
				~Time() = default;
			};

			//
			// Time point casting
			//
			template<Precision T, Precision Q>
			static long long time_cast(const Time<Q>& time)
			{
				ME_CORE_ERROR("Impossible convertion to the specified format");
				return 0;
			}
			template<> static long long time_cast<Precision::SEC, Precision::SEC>(const Time<Precision::SEC>& time) { return time.m_Time; }
			template<> static long long time_cast<Precision::SEC, Precision::MILLI>(const Time<Precision::MILLI>& time) { return time.m_Time / 1000ll; }
			template<> static long long time_cast<Precision::SEC, Precision::MICRO>(const Time<Precision::MICRO>& time) { return time.m_Time / 1000000ll; }
			template<> static long long time_cast<Precision::MILLI, Precision::SEC>(const Time<Precision::SEC>& time) { return time.m_Time * 1000ll; }
			template<> static long long time_cast<Precision::MILLI, Precision::MILLI>(const Time<Precision::MILLI>& time) { return time.m_Time; }
			template<> static long long time_cast<Precision::MILLI, Precision::MICRO>(const Time<Precision::MICRO>& time) { return time.m_Time / 1000ll; }
			template<> static long long time_cast<Precision::MICRO, Precision::SEC>(const Time<Precision::SEC>& time) { return time.m_Time * 1000000ll; }
			template<> static long long time_cast<Precision::MICRO, Precision::MILLI>(const Time<Precision::MILLI>& time) { return time.m_Time * 1000ll; }
			template<> static long long time_cast<Precision::MICRO, Precision::MICRO>(const Time<Precision::MICRO>& time) { return time.m_Time; }

			template<Precision T>
			class Timer
			{
			public:
				Timer()
					: m_Start(std::chrono::high_resolution_clock::now()) {}
				~Timer() = default;

				std::vector<Time<T>> TimeElapsed()
				{

					ME_PROFILE_TRACE_CALL();

					return std::vector<Time<T>>(m_Laps);
				}
				inline bool IsPaused() { return Paused; }
				template<Precision Q>
				bool IfTime(Time<Q> time) const;

				void Start();
				Time<T> Stop();
				Time<T> Pause();
				Time<T> Lap();
				void Reset();
			private:
				std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
				std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseTime;
				std::vector<Time<T>> m_Laps;
				bool Paused = false, Running = true;
			};
			//
			// LowPrecision Timer
			//
			template<>
			class Timer<Precision::SEC>
			{
			public:
				Timer()
					: m_Start(std::chrono::high_resolution_clock::now()) {}
				~Timer() = default;

				std::vector<Time<Precision::SEC>> TimeElapsed()
				{

					ME_PROFILE_TRACE_CALL();

					return std::vector<Time<Precision::SEC>>(m_Laps);
				}
				inline bool IsPaused() { return Paused; }
				template<Precision Q>
				bool IfTime(Time<Q> time) const
				{

					ME_PROFILE_TRACE_CALL();

					long long delta = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::seconds>(m_Start).time_since_epoch().count();
					if (time_cast<Precision::SEC>(time) <= delta)
						return true;
					return false;
				}

				void Start()
				{

					ME_PROFILE_TRACE_CALL();

					if (Paused)
						m_Start = m_PauseTime;
					else
						m_Start = std::chrono::high_resolution_clock::now();
					Paused = false;
					Running = true;
				}

				Time<Precision::SEC> Stop()
				{

					ME_PROFILE_TRACE_CALL();

					if (Running)
					{
						long long delta = 0ll;
						if (Paused)
							delta = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::seconds>(m_PauseTime).time_since_epoch().count();
						else
							delta = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::seconds>(m_Start).time_since_epoch().count();

						m_Laps.emplace_back(delta);
						Running = false;
						return { delta };
					}
					ME_CORE_WARNING("Clock is not running!!");
					return { 0ll };
				}

				Time<Precision::SEC> Pause()
				{

					ME_PROFILE_TRACE_CALL();

					Paused = true;
					long long delta = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::seconds>(m_Start).time_since_epoch().count();

					return { delta };
				}

				Time<Precision::SEC> Lap()
				{

					ME_PROFILE_TRACE_CALL();

					if (Paused)
					{
						ME_CORE_WARNING("Timer paused need to start to register laps");
						return { -1ll };
					}
					long long delta = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::seconds>(m_Start).time_since_epoch().count();
					m_Laps.emplace_back(delta);
					m_Start = std::chrono::high_resolution_clock::now();

					return { delta };
				}

				void Reset()
				{

					ME_PROFILE_TRACE_CALL();

					m_Start = std::chrono::high_resolution_clock::now();
					Paused = false;
					m_Laps.clear();
				}
			private:
				std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
				std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseTime;
				std::vector<Time<Precision::SEC>> m_Laps;
				bool Paused = false, Running = true;
			};
			//
			// MediumPrecision Timer
			//
			template<>
			class Timer<Precision::MILLI>
			{
			public:
				Timer()
					: m_Start(std::chrono::high_resolution_clock::now()) {}
				~Timer() = default;

				std::vector<Time<Precision::MILLI>> TimeElapsed()
				{

					ME_PROFILE_TRACE_CALL();

					return std::vector<Time<Precision::MILLI>>(m_Laps);
				}
				inline bool IsPaused() { return Paused; }
				template<Precision Q>
				bool IfTime(Time<Q> time) const
				{

					ME_PROFILE_TRACE_CALL();

					long long delta = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::milliseconds>(m_Start).time_since_epoch().count();
					if (time_cast<Precision::MILLI>(time) <= delta)
						return true;
					return false;
				}

				void Start()
				{

					ME_PROFILE_TRACE_CALL();

					if (Paused)
						m_Start = m_PauseTime;
					else
						m_Start = std::chrono::high_resolution_clock::now();
					Paused = false;
					Running = true;
				}

				Time<Precision::MILLI> Stop()
				{

					ME_PROFILE_TRACE_CALL();

					if (Running)
					{
						long long delta = 0ll;
						if (Paused)
							delta = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::milliseconds>(m_PauseTime).time_since_epoch().count();
						else
							delta = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::milliseconds>(m_Start).time_since_epoch().count();

						m_Laps.emplace_back(delta);
						Running = false;
						return { delta };
					}
					ME_CORE_WARNING("Clock is not running!!");
					return { 0ll };
				}

				Time<Precision::MILLI> Pause()
				{

					ME_PROFILE_TRACE_CALL();

					Paused = true;
					long long delta = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::milliseconds>(m_Start).time_since_epoch().count();

					return { delta };
				}

				Time<Precision::MILLI> Lap()
				{

					ME_PROFILE_TRACE_CALL();

					if (Paused)
					{
						ME_CORE_WARNING("Timer paused need to start to register laps");
						return { -1ll };
					}
					long long delta = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::milliseconds>(m_Start).time_since_epoch().count();
					m_Laps.emplace_back(delta);
					m_Start = std::chrono::high_resolution_clock::now();

					return { delta };
				}

				void Reset()
				{

					ME_PROFILE_TRACE_CALL();

					m_Start = std::chrono::high_resolution_clock::now();
					Paused = false;
					m_Laps.clear();
				}
			private:
				std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
				std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseTime;
				std::vector<Time<Precision::MILLI>> m_Laps;
				bool Paused = false, Running = true;
			};
			//
			// HighPrecision Timer
			//
			template<>
			class Timer<Precision::MICRO>
			{
			public:
				Timer()
					: m_Start(std::chrono::high_resolution_clock::now()) {}
				~Timer() = default;

				template<typename Q>
				std::vector<Time<Precision::MICRO>> TimeElapsed()
				{

					ME_PROFILE_TRACE_CALL();

					return std::vector<Time<Precision::MICRO>>(m_Laps);
				}
				inline bool IsPaused() { return Paused; }
				template<Precision Q>
				bool IfTime(Time<Q> time) const
				{

					ME_PROFILE_TRACE_CALL();

					long long delta = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
					if (time_cast<Precision::MICRO>(time) <= delta)
						return true;
					return false;
				}

				void Start()
				{

					ME_PROFILE_TRACE_CALL();

					if (Paused)
						m_Start = m_PauseTime;
					else
						m_Start = std::chrono::high_resolution_clock::now();
					Paused = false;
					Running = true;
				}

				Time<Precision::MICRO> Stop()
				{

					ME_PROFILE_TRACE_CALL();

					if (Running)
					{
						long long delta = 0ll;
						if (Paused)
							delta = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::microseconds>(m_PauseTime).time_since_epoch().count();
						else
							delta = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();

						m_Laps.emplace_back(delta);
						Running = false;
						return { delta };
					}
					ME_CORE_WARNING("Clock is not running!!");
					return { 0ll };
				}

				Time<Precision::MICRO> Pause()
				{

					ME_PROFILE_TRACE_CALL();

					Paused = true;
					long long delta = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();

					return { delta };
				}

				Time<Precision::MICRO> Lap()
				{

					ME_PROFILE_TRACE_CALL();

					if (Paused)
					{
						ME_CORE_WARNING("Timer paused need to start to register laps");
						return { -1ll };
					}
					long long delta = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count() - std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
					m_Laps.emplace_back(delta);
					m_Start = std::chrono::high_resolution_clock::now();

					return { delta };
				}

				void Reset()
				{

					ME_PROFILE_TRACE_CALL();

					m_Start = std::chrono::high_resolution_clock::now();
					Paused = false;
					m_Laps.clear();
				}
			private:
				std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
				std::chrono::time_point<std::chrono::high_resolution_clock> m_PauseTime;
				std::vector<Time<Precision::MICRO>> m_Laps;
				bool Paused = false, Running = true;
			};
			//
			// Widely used times
			//
			typedef Timer<Precision::MICRO> timer;
			typedef Time<Precision::SEC> seconds;
			typedef Time<Precision::MILLI> millseconds;
			typedef Time<Precision::MICRO> microseconds;
		}
	}
}