#include "SceneManager.h"

namespace Papyrus
{
	Scene* SceneManager::createScene(const std::string& name)
	{
		m_pScenes.push_back(std::unique_ptr<Scene>(new Scene(name)));
		return m_pScenes.back().get();
	}

	Scene* SceneManager::getCurrentScene() const
	{
		//Todo: this isnt sound logic lol
		for (const auto& scene : m_pScenes)
		{ 
			if(scene->getIsActive() )return scene.get();
		}
		return nullptr;
	}

	void SceneManager::update(float deltaTime)
	{
		for (const auto& scene : m_pScenes)
		{
			scene->update(deltaTime);
		}

	}

	void SceneManager::fixedUpdate(float fixedTimeStep)
	{
		for (const auto& scene : m_pScenes)
		{
			scene->fixedUpdate(fixedTimeStep);
		}

	}

	void SceneManager::render() const
	{
		for (const auto& scene : m_pScenes)
		{
			scene->render();
		}
	}

	void SceneManager::start()
	{
		for (const auto& scene : m_pScenes)
		{
			scene->start();
		}
	}

	void SceneManager::onEnable()
	{
		for (const auto& scene : m_pScenes)
		{
			scene->onEnable();
		}
	}

	void SceneManager::onDisable()
	{
		for (const auto& scene : m_pScenes)
		{
			scene->onDisable(); 
		}
	}
}