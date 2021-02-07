#include "ParticleSystem.h"

namespace ME
{
	namespace Addon
	{
		ParticleSystem::ParticleSystem(const ParticleProps& props)
			:m_Count(props.count), m_Lifetime(props.lifetime), m_AliveParticles(0), m_Velocity({0.0f, 0.0f, 0.0f}), m_Color({0.0f, 0.0f, 0.0f, 0.0f})
			, m_Parent(props.parent) 
		{
			m_Color = props.color;
			m_Velocity = props.emitvelocity;

			Particle particle;
			particle.timeleft = m_Lifetime;
			particle.startcolor = m_Color;

			for (unsigned int i = 0; i < m_Count; i++)
			{
				m_Children.emplace_back(CreateRef<Mesh>(*m_Parent));
				m_Particles.emplace_back(particle);
			}
		}

		void ParticleSystem::Emit(const glm::vec3& veloctiy)
		{
			if (m_AliveParticles < m_Count)
			{
				m_AliveParticles++;

				Particle particle;
				particle.timeleft = m_Lifetime;
				particle.startcolor = m_Color;
			}
			else if (m_AliveParticles >= m_Count)
			{
				auto it = std::min_element(m_Particles.begin(), m_Particles.end(), [](Particle first, Particle smallest) 
					{
						return first.timeleft.m_Time > smallest.timeleft.m_Time;
					});
			}
		}
		void ParticleSystem::OnUpdate()
		{
			for (unsigned int i = 0; i < m_Particles.size(); i++)
				if (m_Particles[i].timeleft.m_Time <= 0)
					RemoveParticle(i);

			for (unsigned int i = 0; i < m_Particles.size(); i++)
			{
				m_Children[i] * (glm::scale(glm::mat4(1.0f), { 0.8f, 0.8f, 1.0f }) * glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), {0.0f, 0.0f, 1.0f})
					*glm::translate(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f}));

				if (timer.IfTime(Utils::Time::seconds(1ll)))
					m_Particles[i].timeleft.m_Time--;
			}
		}
	}
}
