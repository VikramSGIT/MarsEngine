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
				particle.id = i;
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
				particle.id = m_AliveParticles;
				particle.timeleft = m_Lifetime;
				particle.startcolor = m_Color;
				m_Children[m_AliveParticles - 1]->Transulate(veloctiy);
			}
			else if (m_AliveParticles >= m_Count)
			{
				auto it = std::min_element(m_Particles.begin(), m_Particles.end(), [](Particle first, Particle smallest) 
					{
						return first.timeleft.m_Time > smallest.timeleft.m_Time;
					});

				it->timeleft = m_Lifetime;
				it->startcolor = m_Color;
				m_Children[it->id]->TransulateTo(veloctiy);
			}
		}
		void ParticleSystem::OnUpdate()
		{
			if (timer.TimeElapsed()[0] > Utils::Time::milliseconds(1ll))
			{
				for (unsigned int i = 0; i < m_Particles.size(); i++)
				{
					if(m_Particles[i].timeleft > Utils::Time::milliseconds(0))
						m_Particles[i].timeleft.m_Time--;
					m_Children[i]->Transulate({ -1.0f, 0.0f, 0.0f });
				}
				timer.Reset();
			}
		}
	}
}
