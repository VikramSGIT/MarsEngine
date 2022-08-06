#include "MarsHeader.h"
#include "ParticleSystem.h"

namespace ME
{
	namespace Addon
	{
		ParticleSystem2D::ParticleSystem2D(const ParticleProps& props)
			:m_Count(props.count), m_Lifetime(props.lifetime), m_AliveParticles(0), m_Velocity({0.0f, 0.0f}), m_Color({0.0f, 0.0f, 0.0f, 0.0f})
			, m_Parent(props.parent) , m_Transform(props.transform)
		{
			m_Color = props.color;
			m_Velocity = props.emitvelocity;

			Particle particle;
			particle.timeleft = m_Lifetime;
			particle.startcolor = m_Color;

			for (unsigned int i = 0; i < m_Count; i++)
			{
				particle.id = i;
				m_Children.emplace_back(CreateRef<Mesh2D>(*m_Parent));
				m_Particles.emplace_back(particle);
			}
		}

		void ParticleSystem2D::Emit(const glm::vec2& veloctiy)
		{
			if (m_AliveParticles < m_Count)
			{
				m_AliveParticles++;

				Particle particle;
				particle.timeleft = m_Lifetime;
				particle.startcolor = m_Color;
				m_Children[m_AliveParticles - 1]->Reset();
				m_Children[m_AliveParticles - 1]->TranslateTo(veloctiy);
			}
			else if (m_AliveParticles >= m_Count)
			{
				auto it = std::min_element(m_Particles.begin(), m_Particles.end(), [](const Particle& first, const Particle& smallest) 
					{
						return first.timeleft.m_Time < smallest.timeleft.m_Time;
					});

				it->timeleft = m_Lifetime;
				it->startcolor = m_Color;
				m_Children[it->id]->Reset();
				m_Children[it->id]->TranslateTo(veloctiy);
			}
		}
		void ParticleSystem2D::OnUpdate()
		{
			if (timer.TimeElapsed()[0] > Utils::Time::microseconds(1ll))
			{
				for (unsigned int i = 0; i < m_Particles.size(); i++)
				{
					m_Particles[i].timeleft.m_Time--;
					m_Children[i]->Transform(m_Transform);
					m_Children[i]->Rotate(random() * 5.0f);
				}
				timer.Reset();
			}
		}
	}
}
