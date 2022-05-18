#ifndef ME_PARTICLESYSTEM
#define ME_PARTICLESYSTEM

#pragma once

#include <vector>
#include <algorithm>

#include "Addon.h"
#include "Engine Utilites/Timer.h"
#include "Engine Utilites/Random Engine.h"
#include "Window/Events/Event.h"
#include "RenderAPI/Renderer2D.h"

namespace ME
{
	namespace Addon
	{
		struct ParticleProps
		{
			Utils::Time::milliseconds lifetime = 5;
			unsigned int count = 10;
			Ref<Mesh2D> parent;
			glm::vec2 emitvelocity;
			glm::vec4 color;
			glm::mat4 transform = glm::identity<glm::mat4>();
		};
		class ParticleSystem2D : public MeshAddon2D
		{
		public:
			ParticleSystem2D(const ParticleProps& props);
			~ParticleSystem2D() = default;

			void Emit(const glm::vec2& veloctiy);
			void OnUpdate();

			virtual const std::vector<Ref<Mesh2D>>& GetMeshes() const override { return m_Children; }
			inline const size_t GetCount() const { return m_Count; }
			inline const size_t GetAliveCount() const { return m_AliveParticles; }
		private:
			struct Particle
			{
				size_t id;
				Utils::Time::seconds timeleft;
				glm::vec4 startcolor;
			};

			void RemoveParticle(const unsigned int& index)
			{
				m_Particles.erase(m_Particles.begin() + index);
				m_Children.erase(m_Children.begin() + index);
			}

			size_t m_Count, m_AliveParticles;
			glm::vec2 m_Velocity;
			glm::vec4 m_Color;
			glm::mat4 m_Transform;
			Utils::Time::seconds m_Lifetime;

			Utils::Time::Timer<Utils::Time::Precision::MICRO> timer;
			Utils::Random<float> random;
			std::vector<Particle> m_Particles;
			std::vector<Ref<Mesh2D>> m_Children;
			Ref<Mesh2D> m_Parent;
			bool m_Emitting = false;
		};
	}
}

#endif