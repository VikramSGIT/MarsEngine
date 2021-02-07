#pragma once

#include "MarsHeader.h"

#include <vector>
#include <algorithm>

#include "Addon.h"
#include "Utils/Timer.h"
#include "Utils/Random Engine.h"
#include "Window/Events/Event.h"
#include "RenderAPI/Renderer.h"

namespace ME
{
	namespace Addon
	{
		struct ParticleProps
		{
			Utils::Time::seconds lifetime = 5;
			unsigned int count = 10;
			Ref<Mesh> parent;
			glm::vec3 emitvelocity;
			glm::vec4 color;
		};
		class ParticleSystem : public MeshAddon
		{
		public:
			ParticleSystem(const ParticleProps& props);
			~ParticleSystem() = default;

			void Emit(const glm::vec3& veloctiy);
			void OnUpdate();

			virtual const std::vector<Ref<Mesh>>& GetMeshes() const override { return m_Children; }
			inline const size_t GetCount() const { return m_Count; }
			inline const size_t GetAliveCount() const { return m_AliveParticles; }
		private:
			struct Particle
			{
				Utils::Time::seconds timeleft;
				glm::vec4 startcolor;
			};

			void RemoveParticle(const unsigned int& index)
			{
				m_Particles.erase(m_Particles.begin() + index);
				m_Children.erase(m_Children.begin() + index);
			}

			size_t m_Count, m_AliveParticles;
			glm::vec3 m_Velocity;
			glm::vec4 m_Color;
			Utils::Time::seconds m_Lifetime;

			Utils::Time::Timer<Utils::Time::Precision::SEC> timer;
			Utils::Random<float> random;
			std::vector<Particle> m_Particles;
			std::vector<Ref<Mesh>> m_Children;
			Ref<Mesh> m_Parent;
			bool m_Emitting = false;
		};
	}
}
