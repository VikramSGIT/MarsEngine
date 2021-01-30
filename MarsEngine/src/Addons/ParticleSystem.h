#pragma once

#include "MarsHeader.h"

#include <vector>
#include <random>

#include "Utils/Timer.h"
#include "RenderAPI/Mesh.h"

namespace ME
{
	namespace Addon
	{
		struct Particle
		{
			Utils::Timer::seconds lifetime;
			glm::vec3 velocity;
			glm::mat4 rotation;
		};

		class ParticleSystem
		{
		public:
			ParticleSystem(const unsigned int& count = 10);
			~ParticleSystem();

			void Emit() { m_Emitting = true; }
			void StopEmit() { m_Emitting = false; }

			inline void SetParent(const Ref<Renderer::Mesh>& parent) { m_Parent = parent; }
			inline void SetRandomEngine(const unsigned int& seed) { m_Seed = seed; }
		private:
			unsigned int m_Count, m_Seed = 0;
			std::vector<Particle> m_Particles;
			Ref<Renderer::Mesh> m_Parent;
			bool m_Emitting = false;
		};

		class ParticleRenderer
		{
		public:
			ParticleRenderer(const std::vector<Ref<ParticleSystem>>& queue)
				:m_Queue(queue) {}

		private:
			std::vector<Ref<ParticleSystem>> m_Queue;
		};
	}
}
