#include "Scene.h"

namespace Papyrus
{
	Scene::Scene(const std::string& name)
	{
		m_Name = name; 
	}

	void Scene::add(std::unique_ptr<GameObject> object)
	{
		m_Objects.emplace_back(std::move(object));
	}

	void Scene::removeAll()
	{
		m_Objects.clear();

	}

	void Scene::update(float deltaTime)
	{
		for (auto& object : m_Objects)
		{
			object->update(deltaTime);
		}
	}

	void Scene::fixedUpdate(float fixedTimeStep)
	{
		for (auto& object : m_Objects)
		{
			object->update(fixedTimeStep);
		}
	}

	void Scene::render() const
	{
		for (auto& object : m_Objects)
		{
			object->render();
		}
	}

	void Scene::start()
	{
		for (auto& object : m_Objects)
		{
			object->start();
		}
	}

	void Scene::onEnable()
	{
		for (auto& object : m_Objects)
		{
			object->onEnable();
		}
	}

	void Scene::onDisable()
	{
		for (auto& object : m_Objects)
		{
			object->onEnable();
		}
	}
}