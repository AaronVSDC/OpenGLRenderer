#include "GameObject.h"

namespace Papyrus
{

	void GameObject::update(float deltaTime)
	{
		for (auto& component : m_Components)
		{
			component->update(deltaTime); 
		}
	}

	void GameObject::fixedUpdate(float fixedTimeStep)
	{
		for (auto& component : m_Components)
		{
			component->fixedUpdate(fixedTimeStep);
		}
	}

	void GameObject::render() const
	{
		for (auto& component : m_Components)
		{
			component->render();
		}
	}

	void GameObject::start()
	{
		for (auto& component : m_Components)
		{
			component->start();
		}
	}

	void GameObject::onEnable()
	{
		for (auto& component : m_Components) 
		{
			component->onEnable();
		}
	}

	void GameObject::onDisable()
	{
		for (auto& component : m_Components) 
		{
			component->onDisable();
		}
	}

}